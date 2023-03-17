
#ifndef BATTERY_CORE_NO_TRAY
#include <battery/core/tray/core/traybase.hpp>
#include <battery/core/constants.h>

Tray::BaseTray::BaseTray(std::string identifier, std::string tooltip, MouseButton clickAction)
    : identifier(std::move(identifier)),
      clickAction(std::move(clickAction)),
      tooltip(std::move(tooltip)),
      icon(battery::resource::from_base64(battery::constants::BATTERY_DEFAULT_WINDOW_ICON_BASE64))
{
}

std::vector<std::shared_ptr<Tray::TrayEntry>> Tray::BaseTray::getEntries()
{
    return entries;
}

void Tray::BaseTray::addClickCallback(MouseButton button, std::function<void()> callback) {
    clickCallbacks.push_back({ button, callback });
}
#endif // BATTERY_CORE_NO_TRAY