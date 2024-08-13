
#include "battery/filedialog.hpp"
#include "battery/battery.hpp"
#include <source_location>

#ifdef B_OS_WINDOWS
#include "battery/internal/windows.hpp"
#include <shobjidl.h>
#endif

#include <source_location>

namespace b {

#ifdef B_OS_WINDOWS
static void SetDefaultFolder(IFileDialog* dialog, const b::fs::path& defaultPath)
{
    if (defaultPath.empty()) {
        return;
    }
    b::log::info("Setting default folder to {}", defaultPath.native_string());

    IShellItem* folder = nullptr;
    const HRESULT result = SHCreateItemFromParsingName(
        b::widen(defaultPath.native_string()).c_str(), nullptr, IID_PPV_ARGS(&folder));

    if (!SUCCEEDED(result)) { // NOLINTNEXTLINE
        throw std::runtime_error(
            b::format("{}: Failed to show file dialog: Error creating ShellItem: {}",
                      std::source_location::current().function_name(),
                      internal::get_last_win32_error()));
    }

    dialog->SetDefaultFolder(folder);
    folder->Release();
}

static void SetFolder(IFileDialog* dialog, const b::fs::path& path)
{
    if (path.empty()) {
        return;
    }

    IShellItem* folder = nullptr;
    HRESULT const result = SHCreateItemFromParsingName(
        b::widen(path.string()).c_str(), nullptr, IID_PPV_ARGS(&folder));

    if (!SUCCEEDED(result)) { // NOLINTNEXTLINE
        throw std::runtime_error(
            b::format("{}: Failed to show file dialog: Error creating ShellItem: {}",
                      std::source_location::current().function_name(),
                      internal::get_last_win32_error()));
    }

    dialog->SetFolder(folder);
    folder->Release();
}

static void SetFilters(IFileDialog* dialog,
                       std::vector<std::pair<std::string, std::string>> filters)
{
    if (filters.empty()) {
        return;
    }

    std::vector<std::pair<std::wstring, std::wstring>> osFilters;
    for (auto& filter : filters) {
        osFilters.emplace_back(b::widen(filter.first), b::widen(filter.second));
    }

    std::vector<COMDLG_FILTERSPEC> speclist;
    for (auto& filter : osFilters) {
        speclist.push_back({});
        speclist.back().pszName = filter.first.c_str();
        speclist.back().pszSpec = filter.second.c_str();
    }

    dialog->SetFileTypes(speclist.size(), speclist.data());
}

static void
SetOptions(IFileDialog* dialog, bool confirm_overwrite, bool folders_only, bool multiple)
{

    FILEOPENDIALOGOPTIONS options = {};
    dialog->GetOptions(&options);

    options |= FOS_STRICTFILETYPES; // NOLINT
    if (confirm_overwrite) {
        options |= FOS_OVERWRITEPROMPT; // NOLINT
    }
    if (folders_only) {
        options |= FOS_PICKFOLDERS; // NOLINT
    }
    if (multiple) {
        options |= FOS_ALLOWMULTISELECT; // NOLINT
    }

    dialog->SetOptions(options);
}

static std::vector<b::fs::path> Win32OpenDialog(const b::filedialog& dialog,
                                                bool multiple,
                                                std::string default_extension)
{
    if (!SUCCEEDED(
            CoInitializeEx(nullptr,
                           COINIT_APARTMENTTHREADED
                               | COINIT_DISABLE_OLE1DDE))) { // NOLINT NOLINTNEXTLINE
        throw std::runtime_error(
            b::format("{}: Failed to show file dialog: CoInitializeEx failed",
                      std::source_location::current().function_name()));
    }

    IFileOpenDialog* pFileOpen = nullptr; // NOLINTNEXTLINE
    if (!SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog,
                                    nullptr,
                                    CLSCTX_ALL,
                                    IID_IFileOpenDialog,
                                    reinterpret_cast<void**>(&pFileOpen)))) {
        CoUninitialize(); // NOLINTNEXTLINE
        throw std::runtime_error(
            b::format("{}: Failed to show file dialog: CoCreateInstance failed",
                      std::source_location::current().function_name()));
    }

    if (b::fs::exists(b::Folders::SystemDocumentsDir())) {
        SetDefaultFolder(pFileOpen, b::Folders::SystemDocumentsDir());
    }
    if (b::fs::exists(dialog.initial_folder)) {
        SetFolder(pFileOpen, dialog.initial_folder);
    }
    if (!dialog.filters.empty()) {
        SetFilters(pFileOpen, dialog.filters);
    }
    if (!default_extension.empty()) {
        pFileOpen->SetDefaultExtension(b::widen(default_extension).c_str());
    }
    SetOptions(pFileOpen, dialog.confirm_overwrite, dialog.directory, multiple);

    // Now show the dialog
    if (!SUCCEEDED(pFileOpen->Show(nullptr))) { // Dialog was closed
        pFileOpen->Release();
        CoUninitialize();
        return {};
    }

    IShellItemArray* pItemArray = nullptr;
    pFileOpen->GetResults(&pItemArray);

    DWORD numItems = {};
    pItemArray->GetCount(&numItems);

    std::vector<b::fs::path> paths;
    for (size_t i = 0; i < numItems; i++) {
        IShellItem* pItem = nullptr;
        pItemArray->GetItemAt(i, &pItem);

        PWSTR pszFilePath = nullptr;
        pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

        paths.emplace_back(b::narrow(std::wstring(pszFilePath)));
        CoTaskMemFree(pszFilePath);

        pItem->Release();
    }

    pItemArray->Release();
    pFileOpen->Release();
    CoUninitialize();

    return paths;
}

static b::fs::path Win32SaveDialog(const b::filedialog& dialog,
                                   std::string default_extension)
{
    if (!SUCCEEDED(
            CoInitializeEx(nullptr,
                           COINIT_APARTMENTTHREADED
                               | COINIT_DISABLE_OLE1DDE))) { // NOLINT NOLINTNEXTLINE
        throw std::runtime_error(
            b::format("{}: Failed to show file dialog: CoInitializeEx failed",
                      std::source_location::current().function_name()));
    }

    IFileSaveDialog* pFileSave = nullptr; // NOLINTNEXTLINE
    if (!SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog,
                                    nullptr,
                                    CLSCTX_ALL,
                                    IID_IFileSaveDialog,
                                    reinterpret_cast<void**>(&pFileSave)))) {
        CoUninitialize(); // NOLINTNEXTLINE
        throw std::runtime_error(
            b::format("{}: Failed to show file dialog: CoCreateInstance failed",
                      std::source_location::current().function_name()));
    }

    if (b::fs::exists(b::Folders::SystemDocumentsDir())) {
        SetDefaultFolder(pFileSave, b::Folders::SystemDocumentsDir());
    }
    if (b::fs::exists(dialog.initial_folder)) {
        SetFolder(pFileSave, dialog.initial_folder);
    }
    if (!dialog.filters.empty()) {
        SetFilters(pFileSave, dialog.filters);
    }
    if (!default_extension.empty()) {
        pFileSave->SetDefaultExtension(b::widen(default_extension).c_str());
    }
    SetOptions(pFileSave, dialog.confirm_overwrite, dialog.directory, false);

    // Now show the dialog
    if (!SUCCEEDED(pFileSave->Show(nullptr))) { // Dialog was closed
        pFileSave->Release();
        CoUninitialize();
        return {};
    }

    IShellItem* pItem = nullptr;
    pFileSave->GetResult(&pItem);

    PWSTR pszFilePath = nullptr;
    pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

    b::fs::path const path = b::narrow(std::wstring(pszFilePath));
    CoTaskMemFree(pszFilePath);

    pItem->Release();
    pFileSave->Release();
    CoUninitialize();

    return path;
}
#endif

b::fs::path filedialog::sync_open()
{
#ifdef B_OS_WINDOWS
    auto paths = Win32OpenDialog(*this, false, default_extension.string());
    auto path = paths.size() > 0 ? paths[0] : b::fs::path();
#else
#warning FILEDIALOG NOT IMPLEMENTED
    auto path = b::fs::path();
#endif
    if (!default_extension.empty() && path.extension().empty()) {
        path.replace_extension(default_extension);
    }
    return path;
}

std::vector<b::fs::path> filedialog::sync_open_multiple()
{
#ifdef B_OS_WINDOWS
    auto paths = Win32OpenDialog(*this, true, default_extension.string());
#else
#warning FILEDIALOG NOT IMPLEMENTED
    auto paths = std::vector<b::fs::path>();
#endif
    if (!default_extension.empty()) {
        for (auto& path : paths) {
            if (path.extension().empty()) {
                path.replace_extension(default_extension);
            }
        }
    }
    return paths;
}

b::fs::path filedialog::sync_save()
{
#ifdef B_OS_WINDOWS
    auto path = Win32SaveDialog(*this, default_extension.string());
#else
#warning FILEDIALOG NOT IMPLEMENTED
    auto path = b::fs::path();
#endif
    if (!default_extension.empty() && path.extension().empty()) {
        path.replace_extension(default_extension);
    }
    return path;
}

} // namespace b
