#include <battery/core/tray/components/submenu.hpp>
#include <battery/core/tray/core/entry.hpp>

Tray::Submenu::Submenu(std::string text) : TrayEntry(std::move(text)) {}
void Tray::Submenu::update()
{
    if (parent)
    {
        parent->update();
    }
}

std::vector<std::shared_ptr<Tray::TrayEntry>> Tray::Submenu::getEntries()
{
    return entries;
}