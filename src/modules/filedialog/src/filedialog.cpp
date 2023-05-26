
#include "battery/filedialog/filedialog.hpp"
#include "battery/core/all.hpp"

#ifdef BATTERY_ARCH_WINDOWS
#include "battery/core/internal/windows.hpp"
#include <shobjidl.h>
#endif

namespace b {

#ifdef BATTERY_ARCH_WINDOWS
    static void setDefaultFolder(IFileDialog *dialog, const b::fs::path& defaultPath) {
        if (defaultPath.empty()) return;
        b::log::info("Setting default folder to {}", defaultPath);

        IShellItem *folder;
        HRESULT result = SHCreateItemFromParsingName(defaultPath.string().wstr().c_str(), NULL, IID_PPV_ARGS(&folder));

        if (!SUCCEEDED(result)) {
            throw std::runtime_error(b::format("{}: Failed to show file dialog: Error creating ShellItem: {}", std::source_location::current().function_name(), internal::get_last_win32_error()));
        }

        dialog->SetDefaultFolder(folder);
        folder->Release();
    }

    static void setFolder(IFileDialog *dialog, const b::fs::path& path) {
        if (path.empty()) return;

        IShellItem *folder;
        HRESULT result = SHCreateItemFromParsingName(path.string().wstr().c_str(), NULL, IID_PPV_ARGS(&folder));

        if (!SUCCEEDED(result)) {
            throw std::runtime_error(b::format("{}: Failed to show file dialog: Error creating ShellItem: {}", std::source_location::current().function_name(), internal::get_last_win32_error()));
        }

        dialog->SetFolder(folder);
        folder->Release();
    }

    static void setFilters(IFileDialog *dialog, std::vector<std::pair<b::string,b::string>> filters) {
        if (filters.empty()) return;

        std::vector<std::pair<std::wstring,std::wstring>> os_filters;
        for (auto& filter : filters) {
            os_filters.push_back({filter.first.wstr(), filter.second.wstr()});
        }

        std::vector<COMDLG_FILTERSPEC> speclist;
        for (auto& filter : os_filters) {
            speclist.push_back({});
            speclist.back().pszName = filter.first.c_str();
            speclist.back().pszSpec = filter.second.c_str();
        }

        dialog->SetFileTypes(speclist.size(), speclist.data());
    }

    static void setOptions(IFileDialog *dialog, bool confirm_overwrite, bool folders_only, bool multiple) {

        FILEOPENDIALOGOPTIONS options;
        dialog->GetOptions(&options);

        if (confirm_overwrite) options |= FOS_OVERWRITEPROMPT;
        options |= FOS_STRICTFILETYPES;
        if (folders_only) options |= FOS_PICKFOLDERS;
        if (multiple) options |= FOS_ALLOWMULTISELECT;

        dialog->SetOptions(options);
    }

    static std::vector<b::fs::path> win32_open_dialog(const b::filedialog& dialog, bool multiple, b::string default_extension) {
        if (!SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) {
            throw std::runtime_error(b::format("{}: Failed to show file dialog: CoInitializeEx failed", std::source_location::current().function_name()));
        }

        IFileOpenDialog *pFileOpen;
        if (!SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen)))) {
            CoUninitialize();
            throw std::runtime_error(b::format("{}: Failed to show file dialog: CoCreateInstance failed", std::source_location::current().function_name()));
        }

        if (b::fs::exists(b::folders::get_global_documents())) setDefaultFolder(pFileOpen, b::folders::get_global_documents());
        if (b::fs::exists(dialog.initial_folder)) setFolder(pFileOpen, dialog.initial_folder);
        if (!dialog.filters.empty()) setFilters(pFileOpen, dialog.filters);
        if (!default_extension.empty()) pFileOpen->SetDefaultExtension(default_extension.wstr().c_str());
        setOptions(pFileOpen, dialog.confirm_overwrite, dialog.directory, multiple);

        // Now show the dialog
        if (!SUCCEEDED(pFileOpen->Show(NULL))) {    // Dialog was closed
            pFileOpen->Release();
            CoUninitialize();
            return {};
        }

        IShellItemArray *pItemArray;
        pFileOpen->GetResults(&pItemArray);

        DWORD numItems;
        pItemArray->GetCount(&numItems);

        std::vector<b::fs::path> paths;
        for (size_t i = 0; i < numItems; i++) {
            IShellItem *pItem;
            pItemArray->GetItemAt(i, &pItem);

            PWSTR pszFilePath;
            pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

            paths.emplace_back(b::string(std::wstring(pszFilePath)));
            CoTaskMemFree(pszFilePath);

            pItem->Release();
        }

        pItemArray->Release();
        pFileOpen->Release();
        CoUninitialize();

        return paths;
    }

    static b::fs::path win32_save_dialog(const b::filedialog& dialog, b::string default_extension) {
        if (!SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) {
            throw std::runtime_error(b::format("{}: Failed to show file dialog: CoInitializeEx failed", std::source_location::current().function_name()));
        }

        IFileSaveDialog *pFileSave;
        if (!SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave)))) {
            CoUninitialize();
            throw std::runtime_error(b::format("{}: Failed to show file dialog: CoCreateInstance failed", std::source_location::current().function_name()));
        }

        if (b::fs::exists(b::folders::get_global_documents())) setDefaultFolder(pFileSave, b::folders::get_global_documents());
        if (b::fs::exists(dialog.initial_folder)) setFolder(pFileSave, dialog.initial_folder);
        if (!dialog.filters.empty()) setFilters(pFileSave, dialog.filters);
        if (!default_extension.empty()) pFileSave->SetDefaultExtension(default_extension.wstr().c_str());
        setOptions(pFileSave, dialog.confirm_overwrite, dialog.directory, false);

        // Now show the dialog
        if (!SUCCEEDED(pFileSave->Show(NULL))) {    // Dialog was closed
            pFileSave->Release();
            CoUninitialize();
            return {};
        }

        IShellItem *pItem;
        pFileSave->GetResult(&pItem);

        PWSTR pszFilePath;
        pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

        b::fs::path path = b::string(std::wstring(pszFilePath));
        CoTaskMemFree(pszFilePath);

        pItem->Release();
        pFileSave->Release();
        CoUninitialize();

        return path;
    }
#endif

    b::fs::path filedialog::sync_open() {
        auto path = win32_open_dialog(*this, false, default_extension)[0];
        if (!default_extension.empty() && path.extension().empty()) path.replace_extension(default_extension);
        return path;
    }

    std::vector<b::fs::path> filedialog::sync_open_multiple() {
        auto paths = win32_open_dialog(*this, true, default_extension);
        if (!default_extension.empty()) {
            for (auto& path : paths) {
                if (path.extension().empty()) path.replace_extension(default_extension);
            }
        }
        return paths;
    }

    b::fs::path filedialog::sync_save() {
        auto path = win32_save_dialog(*this, default_extension);
        if (!default_extension.empty() && path.extension().empty()) path.replace_extension(default_extension);
        return path;
    }

}