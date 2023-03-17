#ifndef BATTERY_CORE_NO_TRAY
#include <stdexcept>
#if defined(_WIN32)
#include <battery/core/tray/core/image.hpp>

static std::wstring widen(const std::string& s) {
    size_t wsz = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), 0, 0);
    if (!wsz) return {};
    std::wstring out(wsz, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str (), (int)s.size(), &out[0], (int)wsz);
    return out;
}

Tray::Image::Image(HBITMAP image) : image(image) {}
Tray::Image::Image(const char *path) : Image(std::string(path)) {}
Tray::Image::Image(const std::string &path)
    : image(reinterpret_cast<HBITMAP>(LoadImage(nullptr, widen(path).c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)))
{
    if (image == nullptr)
    {
        throw std::runtime_error("Failed to load bitmap");
    }
}

Tray::Image::operator HBITMAP()
{
    return image;
}
#endif
#endif // BATTERY_CORE_NO_TRAY