#ifndef BATTERY_CORE_NO_TRAY
#include <battery/core/tray/core/entry.hpp>
#include <battery/core/tray/core/traybase.hpp>

Tray::TrayEntry::TrayEntry(std::string text) : text(std::move(text)) {}

Tray::BaseTray *Tray::TrayEntry::getParent()
{
    return parent;
}

void Tray::TrayEntry::setParent(BaseTray *newParent)
{
    parent = newParent;
}

std::string Tray::TrayEntry::getText()
{
    return text;
}

void Tray::TrayEntry::setText(std::string newText)
{
    text = std::move(newText);
    if (parent)
    {
        parent->update();
    }
}

bool Tray::TrayEntry::isDisabled() const
{
    return disabled;
}

void Tray::TrayEntry::setDisabled(bool state)
{
    disabled = state;
    if (parent)
    {
        parent->update();
    }
}
#endif // BATTERY_CORE_NO_TRAY