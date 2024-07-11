
#include "battery/imguilua.hpp"
#include "battery/application.hpp"
#include "battery/extern/magic_enum.hpp"
#include "battery/log.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <exception>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>
#include <LuaBridge/detail/Stack.h>

#define ADD_IMGUI_FUNCTION(name) addFunction(#name, &ImGui::name)

#define ADD_FUNCTION(name) addFunction(#name, &name)

#define NS_ADD_ENUM(enum)                                                               \
    for (auto [enumValue, enumKey] : magic_enum::enum_entries<enum>()) {                \
        ns.addVariable(enumKey.data(), enumValue);                                      \
    }

namespace b {

std::expected<std::nullopt_t, std::string> LuaLoader::updateScript(lua_State* luaState)
{
    if (m_haveLuaResource) {
        std::lock_guard const lock(m_luaResourceMutex);
        if (m_luaResourceChanged) {
            b::log::info("Lua Script reloading");
            auto result = b::ImGuiLua::RunLuaString(
                luaState, m_luaResourceString, m_debugFilename);
            if (result) {
                m_luaResourceChanged = false;
            }
            return result;
        }
    }
    return std::nullopt;
}

void Text(const std::string& text)
{
    ImGui::Text("%s", text.c_str());
}

bool Button(const std::string& text, const ImVec2& size)
{
    return ImGui::Button(text.c_str(), size);
}

void ApplyStyleColorPermanent(const std::string& enumValue, const ImColor& color)
{
    auto colorEnum = magic_enum::enum_cast<ImGuiCol_>(enumValue);
    if (!colorEnum) {
        throw std::invalid_argument(b::format(
            "ImGuiLua::ApplyStyleColorPermanent: Unknown ImGuiCol '{}'", enumValue));
    }

    if (*colorEnum < ImGuiCol_COUNT) {
        ImGui::GetStyle().Colors[*colorEnum] = color; // NOLINT
    }
    else {
        throw std::invalid_argument(b::format(
            "ImGuiLua::ApplyStyleColorPermanent: Unknown ImGuiCol '{}'", enumValue));
    }
}

void ApplyStyleVec2Permanent(const std::string& enumValue, const ImVec2& value)
{
    auto stylevarEnum = magic_enum::enum_cast<ImGuiStyleVar_>(enumValue);
    if (!stylevarEnum) {
        throw std::invalid_argument(b::format(
            "ImGuiLua::ApplyStyleVec2Permanent: Unknown ImGuiStyleVar '{}'", enumValue));
    }

    switch (stylevarEnum.value()) {
        //            case ImGuiStyleVar_Alpha: ImGui::GetStyle().Alpha = value; break;
        //            case ImGuiStyleVar_DisabledAlpha: ImGui::GetStyle().DisabledAlpha =
        //            value; break;
    case ImGuiStyleVar_WindowPadding:
        ImGui::GetStyle().WindowPadding = ImVec2(value[0], value[1]);
        break;
        //            case ImGuiStyleVar_WindowRounding: ImGui::GetStyle().WindowRounding
        //            = value; break; case ImGuiStyleVar_WindowBorderSize:
        //            ImGui::GetStyle().WindowBorderSize = value; break;
    case ImGuiStyleVar_WindowMinSize:
        ImGui::GetStyle().WindowMinSize = ImVec2(value[0], value[1]);
        break;
    case ImGuiStyleVar_WindowTitleAlign:
        ImGui::GetStyle().WindowTitleAlign = ImVec2(value[0], value[1]);
        break;
        //            case ImGuiStyleVar_ChildRounding: ImGui::GetStyle().ChildRounding =
        //            value; break; case ImGuiStyleVar_ChildBorderSize:
        //            ImGui::GetStyle().ChildBorderSize = value; break; case
        //            ImGuiStyleVar_PopupRounding: ImGui::GetStyle().PopupRounding =
        //            value; break; case ImGuiStyleVar_PopupBorderSize:
        //            ImGui::GetStyle().PopupBorderSize = value; break;
    case ImGuiStyleVar_FramePadding:
        ImGui::GetStyle().FramePadding = ImVec2(value[0], value[1]);
        break;
        //            case ImGuiStyleVar_FrameRounding: ImGui::GetStyle().FrameRounding =
        //            value; break; case ImGuiStyleVar_FrameBorderSize:
        //            ImGui::GetStyle().FrameBorderSize = value; break;
    case ImGuiStyleVar_ItemSpacing:
        ImGui::GetStyle().ItemSpacing = ImVec2(value[0], value[1]);
        break;
    case ImGuiStyleVar_ItemInnerSpacing:
        ImGui::GetStyle().ItemInnerSpacing = ImVec2(value[0], value[1]);
        break;
        //            case ImGuiStyleVar_IndentSpacing: ImGui::GetStyle().IndentSpacing =
        //            value; break;
    case ImGuiStyleVar_CellPadding:
        ImGui::GetStyle().CellPadding = ImVec2(value[0], value[1]);
        break;
        //            case ImGuiStyleVar_ScrollbarSize: ImGui::GetStyle().ScrollbarSize =
        //            value; break; case ImGuiStyleVar_ScrollbarRounding:
        //            ImGui::GetStyle().ScrollbarRounding = value; break; case
        //            ImGuiStyleVar_GrabMinSize: ImGui::GetStyle().GrabMinSize = value;
        //            break; case ImGuiStyleVar_GrabRounding:
        //            ImGui::GetStyle().GrabRounding = value; break; case
        //            ImGuiStyleVar_TabRounding: ImGui::GetStyle().TabRounding = value;
        //            break;
    case ImGuiStyleVar_ButtonTextAlign:
        ImGui::GetStyle().ButtonTextAlign = ImVec2(value[0], value[1]);
        break;
    case ImGuiStyleVar_SelectableTextAlign:
        ImGui::GetStyle().SelectableTextAlign = ImVec2(value[0], value[1]);
        break;
        //            case ImGuiStyleVar_SeparatorTextBorderSize:
        //            ImGui::GetStyle().SeparatorTextBorderSize = value; break;
    case ImGuiStyleVar_SeparatorTextAlign:
        ImGui::GetStyle().SeparatorTextAlign = ImVec2(value[0], value[1]);
        break;
    case ImGuiStyleVar_SeparatorTextPadding:
        ImGui::GetStyle().SeparatorTextPadding = ImVec2(value[0], value[1]);
        break;
    default:
        throw std::invalid_argument(
            b::format("ImGuiLua::ApplyStyleVec2Permanent: ImGuiStyleVar '{}' needs a "
                      "float, not vec2",
                      enumValue));
    }
}

void ApplyStyleFloatPermanent(const std::string& enumValue, float value)
{
    auto stylevarEnum = magic_enum::enum_cast<ImGuiStyleVar_>(enumValue);
    if (!stylevarEnum) {
        throw std::invalid_argument(
            b::format("ImGuiLua::ApplyStyleFloatPermanent: Unknown ImGuiStyleVar '{}'",
                      enumValue));
    }

    switch (stylevarEnum.value()) {
    case ImGuiStyleVar_Alpha:
        ImGui::GetStyle().Alpha = value;
        break;
    case ImGuiStyleVar_DisabledAlpha:
        ImGui::GetStyle().DisabledAlpha = value;
        break;
        //            case ImGuiStyleVar_WindowPadding: ImGui::GetStyle().WindowPadding =
        //            ImVec2(value[0], value[1]); break;
    case ImGuiStyleVar_WindowRounding:
        ImGui::GetStyle().WindowRounding = value;
        break;
    case ImGuiStyleVar_WindowBorderSize:
        ImGui::GetStyle().WindowBorderSize = value;
        break;
        //            case ImGuiStyleVar_WindowMinSize: ImGui::GetStyle().WindowMinSize =
        //            ImVec2(value[0], value[1]); break; case
        //            ImGuiStyleVar_WindowTitleAlign: ImGui::GetStyle().WindowTitleAlign
        //            = ImVec2(value[0], value[1]); break;
    case ImGuiStyleVar_ChildRounding:
        ImGui::GetStyle().ChildRounding = value;
        break;
    case ImGuiStyleVar_ChildBorderSize:
        ImGui::GetStyle().ChildBorderSize = value;
        break;
    case ImGuiStyleVar_PopupRounding:
        ImGui::GetStyle().PopupRounding = value;
        break;
    case ImGuiStyleVar_PopupBorderSize:
        ImGui::GetStyle().PopupBorderSize = value;
        break;
        //            case ImGuiStyleVar_FramePadding: ImGui::GetStyle().FramePadding =
        //            ImVec2(value[0], value[1]); break;
    case ImGuiStyleVar_FrameRounding:
        ImGui::GetStyle().FrameRounding = value;
        break;
    case ImGuiStyleVar_FrameBorderSize:
        ImGui::GetStyle().FrameBorderSize = value;
        break;
        //            case ImGuiStyleVar_ItemSpacing: ImGui::GetStyle().ItemSpacing =
        //            ImVec2(value[0], value[1]); break; case
        //            ImGuiStyleVar_ItemInnerSpacing: ImGui::GetStyle().ItemInnerSpacing
        //            = ImVec2(value[0], value[1]); break;
    case ImGuiStyleVar_IndentSpacing:
        ImGui::GetStyle().IndentSpacing = value;
        break;
        //            case ImGuiStyleVar_CellPadding: ImGui::GetStyle().CellPadding =
        //            ImVec2(value[0], value[1]); break;
    case ImGuiStyleVar_ScrollbarSize:
        ImGui::GetStyle().ScrollbarSize = value;
        break;
    case ImGuiStyleVar_ScrollbarRounding:
        ImGui::GetStyle().ScrollbarRounding = value;
        break;
    case ImGuiStyleVar_GrabMinSize:
        ImGui::GetStyle().GrabMinSize = value;
        break;
    case ImGuiStyleVar_GrabRounding:
        ImGui::GetStyle().GrabRounding = value;
        break;
    case ImGuiStyleVar_TabRounding:
        ImGui::GetStyle().TabRounding = value;
        break;
        //            case ImGuiStyleVar_ButtonTextAlign:
        //            ImGui::GetStyle().ButtonTextAlign = ImVec2(value[0], value[1]);
        //            break; case ImGuiStyleVar_SelectableTextAlign:
        //            ImGui::GetStyle().SelectableTextAlign = ImVec2(value[0], value[1]);
        //            break;
    case ImGuiStyleVar_SeparatorTextBorderSize:
        ImGui::GetStyle().SeparatorTextBorderSize = value;
        break;
        //            case ImGuiStyleVar_SeparatorTextAlign:
        //            ImGui::GetStyle().SeparatorTextAlign = ImVec2(value[0], value[1]);
        //            break; case ImGuiStyleVar_SeparatorTextPadding:
        //            ImGui::GetStyle().SeparatorTextPadding = ImVec2(value[0],
        //            value[1]); break;
    default:
        throw std::invalid_argument(
            b::format("ImGuiLua::ApplyStyleVec2Permanent: ImGuiStyleVar '{}' needs a "
                      "vec2, not float",
                      enumValue));
    }
}

void PushStyleColor(const std::string& enumValue, const ImColor& color)
{
    auto stylevarEnum = magic_enum::enum_cast<ImGuiCol_>(enumValue);
    if (!stylevarEnum) {
        throw std::invalid_argument(b::format(
            "ImGuiLua::PushStyleColor: Unknown ImGuiStyleCol '{}'", enumValue));
    }
    ImGui::PushStyleColor(*stylevarEnum, color.Value);
}

void PushStyleVarVec2(const std::string& enumValue, const ImVec2& value)
{
    auto stylevarEnum = magic_enum::enum_cast<ImGuiStyleVar_>(enumValue);
    if (!stylevarEnum) {
        throw std::invalid_argument(b::format(
            "ImGuiLua::PushStyleVarVec2: Unknown ImGuiStyleVar '{}'", enumValue));
    }

    // Check if the type is really vec2
    auto* const varInfo = ImGui::GetStyleVarInfo(*stylevarEnum);
    if (varInfo->Type == ImGuiDataType_Float && varInfo->Count == 2) {
        ImGui::PushStyleVar(*stylevarEnum, value);
    }
    else {
        b::log::error("ImGuiLua::PushStyleVarVec2: ImGuiStyleVar '{}' "
                      "needs a vec2, not float",
                      enumValue);
        // We push a value regardless that has no effect, to not break PopStyleVar()
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding);
    }
}

void PushStyleVarFloat(const std::string& enumValue, float value)
{
    auto stylevarEnum = magic_enum::enum_cast<ImGuiStyleVar_>(enumValue);
    if (!stylevarEnum) {
        throw std::invalid_argument(b::format(
            "ImGuiLua::PushStyleVarFloat: Unknown ImGuiStyleVar '{}'", enumValue));
    }

    // Check if the type is really float
    auto* const varInfo = ImGui::GetStyleVarInfo(*stylevarEnum);
    if (varInfo->Type == ImGuiDataType_Float && varInfo->Count == 1) {
        ImGui::PushStyleVar(*stylevarEnum, value);
    }
    else {
        b::log::error("ImGuiLua::PushStyleVarFloat: ImGuiStyleVar '{}' "
                      "needs a float, not vec2",
                      enumValue);
        // We push a value regardless that has no effect, to not break PopStyleVar()
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha);
    }
}

void PopStyleColor()
{
    ImGui::PopStyleColor();
}

void PopStyleVarVec2()
{
    ImGui::PopStyleVar();
}

void PopStyleVarFloat()
{
    ImGui::PopStyleVar();
}

void RequestAnimationFrameIn(double seconds)
{
    b::Application::get().requestAnimationFrameIn(seconds);
}

void ImGuiLua::DeclareLuaBridge(lua_State* L)
{
    luabridge::getGlobalNamespace(L)
        .beginClass<ImVec2>("ImVec2")
        .addConstructor<void (*)(float, float)>()
        .addProperty("x", &ImVec2::x)
        .addProperty("y", &ImVec2::y)
        .endClass();

    luabridge::getGlobalNamespace(L)
        .beginClass<ImVec4>("ImVec4")
        .addConstructor<void (*)(float, float, float, float)>()
        .addProperty("x", &ImVec4::x)
        .addProperty("y", &ImVec4::y)
        .addProperty("z", &ImVec4::z)
        .addProperty("w", &ImVec4::w)
        .endClass();

    luabridge::getGlobalNamespace(L)
        .beginNamespace("ImGuiWindowFlags")
        .addProperty("None", []() -> int { return ImGuiWindowFlags_None; })
        .addProperty("NoTitleBar", []() -> int { return ImGuiWindowFlags_NoTitleBar; })
        .addProperty("NoResize", []() -> int { return ImGuiWindowFlags_NoResize; })
        .addProperty("NoMove", []() -> int { return ImGuiWindowFlags_NoMove; })
        .addProperty("NoScrollbar", []() -> int { return ImGuiWindowFlags_NoScrollbar; })
        .addProperty("NoScrollWithMouse",
                     []() -> int { return ImGuiWindowFlags_NoScrollWithMouse; })
        .addProperty("NoCollapse", []() -> int { return ImGuiWindowFlags_NoCollapse; })
        .addProperty("AlwaysAutoResize",
                     []() -> int { return ImGuiWindowFlags_AlwaysAutoResize; })
        .addProperty("NoBackground",
                     []() -> int { return ImGuiWindowFlags_NoBackground; })
        .addProperty("NoSavedSettings",
                     []() -> int { return ImGuiWindowFlags_NoSavedSettings; })
        .addProperty("NoMouseInputs",
                     []() -> int { return ImGuiWindowFlags_NoMouseInputs; })
        .addProperty("MenuBar", []() -> int { return ImGuiWindowFlags_MenuBar; })
        .addProperty("HorizontalScrollbar",
                     []() -> int { return ImGuiWindowFlags_HorizontalScrollbar; })
        .addProperty("NoFocusOnAppearing",
                     []() -> int { return ImGuiWindowFlags_NoFocusOnAppearing; })
        .addProperty("NoBringToFrontOnFocus",
                     []() -> int { return ImGuiWindowFlags_NoBringToFrontOnFocus; })
        .addProperty("AlwaysVerticalScrollbar",
                     []() -> int { return ImGuiWindowFlags_AlwaysVerticalScrollbar; })
        .addProperty("AlwaysHorizontalScrollbar",
                     []() -> int { return ImGuiWindowFlags_AlwaysHorizontalScrollbar; })
        .addProperty("AlwaysUseWindowPadding",
                     []() -> int { return ImGuiWindowFlags_AlwaysUseWindowPadding; })
        .addProperty("NoNavInputs", []() -> int { return ImGuiWindowFlags_NoNavInputs; })
        .addProperty("NoNavFocus", []() -> int { return ImGuiWindowFlags_NoNavFocus; })
        .addProperty("UnsavedDocument",
                     []() -> int { return ImGuiWindowFlags_UnsavedDocument; })
        .addProperty("NoNav", []() -> int { return ImGuiWindowFlags_NoNav; })
        .addProperty("NoDecoration",
                     []() -> int { return ImGuiWindowFlags_NoDecoration; })
        .addProperty("NoInputs", []() -> int { return ImGuiWindowFlags_NoInputs; })
        .endNamespace();

    luabridge::getGlobalNamespace(L)
        .beginClass<ImGuiIO>("ImGuiIO")
        // .addProperty("ConfigFlags", &ImGuiIO::ConfigFlags)
        // .addProperty("BackendFlags", &ImGuiIO::BackendFlags)
        .addProperty("DisplaySize", &ImGuiIO::DisplaySize)
        .addProperty("DeltaTime", &ImGuiIO::DeltaTime)
        .addProperty("IniSavingRate", &ImGuiIO::IniSavingRate)
        .addProperty("IniFilename", &ImGuiIO::IniFilename)
        .addProperty("LogFilename", &ImGuiIO::LogFilename)
        // .addProperty("UserData", &ImGuiIO::UserData)
        // .addProperty("Fonts", &ImGuiIO::Fonts)
        .addProperty("FontGlobalScale", &ImGuiIO::FontGlobalScale)
        .addProperty("FontAllowUserScaling", &ImGuiIO::FontAllowUserScaling)
        // .addProperty("FontDefault", &ImGuiIO::FontDefault)
        .addProperty("DisplayFramebufferScale", &ImGuiIO::DisplayFramebufferScale)
        .addProperty("MouseDrawCursor", &ImGuiIO::MouseDrawCursor)
        .addProperty("ConfigMacOSXBehaviors", &ImGuiIO::ConfigMacOSXBehaviors)
        .addProperty("ConfigInputTrickleEventQueue",
                     &ImGuiIO::ConfigInputTrickleEventQueue)
        .addProperty("ConfigInputTextCursorBlink", &ImGuiIO::ConfigInputTextCursorBlink)
        .addProperty("ConfigInputTextEnterKeepActive",
                     &ImGuiIO::ConfigInputTextEnterKeepActive)
        .addProperty("ConfigDragClickToInputText", &ImGuiIO::ConfigDragClickToInputText)
        .addProperty("ConfigWindowsResizeFromEdges",
                     &ImGuiIO::ConfigWindowsResizeFromEdges)
        .addProperty("ConfigWindowsMoveFromTitleBarOnly",
                     &ImGuiIO::ConfigWindowsMoveFromTitleBarOnly)
        .addProperty("ConfigMemoryCompactTimer", &ImGuiIO::ConfigMemoryCompactTimer)
        .addProperty("MouseDoubleClickTime", &ImGuiIO::MouseDoubleClickTime)
        .addProperty("MouseDoubleClickMaxDist", &ImGuiIO::MouseDoubleClickMaxDist)
        .addProperty("MouseDragThreshold", &ImGuiIO::MouseDragThreshold)
        .addProperty("KeyRepeatDelay", &ImGuiIO::KeyRepeatDelay)
        .addProperty("KeyRepeatRate", &ImGuiIO::KeyRepeatRate)
        .addProperty("ConfigDebugBeginReturnValueOnce",
                     &ImGuiIO::ConfigDebugBeginReturnValueOnce)
        .addProperty("ConfigDebugBeginReturnValueLoop",
                     &ImGuiIO::ConfigDebugBeginReturnValueLoop)
        .addProperty("ConfigDebugIgnoreFocusLoss", &ImGuiIO::ConfigDebugIgnoreFocusLoss)
        .addProperty("ConfigDebugIniSettings", &ImGuiIO::ConfigDebugIniSettings)
        .addProperty("BackendPlatformName", &ImGuiIO::BackendPlatformName)
        .addProperty("BackendRendererName", &ImGuiIO::BackendRendererName)
        .addProperty("BackendPlatformUserData", &ImGuiIO::BackendPlatformUserData)
        // .addProperty("GetClipboardTextFn", &ImGuiIO::GetClipboardTextFn)
        // .addProperty("SetClipboardTextFn", &ImGuiIO::SetClipboardTextFn)
        .addProperty("ClipboardUserData", &ImGuiIO::ClipboardUserData)
        .addProperty("SetPlatformImeDataFn", &ImGuiIO::SetPlatformImeDataFn)
        .addProperty("WantCaptureMouse", &ImGuiIO::WantCaptureMouse)
        .addProperty("WantCaptureKeyboard", &ImGuiIO::WantCaptureKeyboard)
        .addProperty("WantTextInput", &ImGuiIO::WantTextInput)
        .addProperty("WantSetMousePos", &ImGuiIO::WantSetMousePos)
        .addProperty("WantSaveIniSettings", &ImGuiIO::WantSaveIniSettings)
        .addProperty("NavActive", &ImGuiIO::NavActive)
        .addProperty("NavVisible", &ImGuiIO::NavVisible)
        .addProperty("Framerate", &ImGuiIO::Framerate)
        .addProperty("MetricsRenderVertices", &ImGuiIO::MetricsRenderVertices)
        .addProperty("MetricsRenderIndices", &ImGuiIO::MetricsRenderIndices)
        .addProperty("MetricsRenderWindows", &ImGuiIO::MetricsRenderWindows)
        .addProperty("MetricsActiveWindows", &ImGuiIO::MetricsActiveWindows)
        .addProperty("MetricsActiveAllocations", &ImGuiIO::MetricsActiveAllocations)
        .addProperty("MouseDelta", &ImGuiIO::MouseDelta)
        .endClass();

    luabridge::getGlobalNamespace(L)
        .beginClass<ImGuiStyle>("ImGuiStyle")
        .addProperty("Alpha", &ImGuiStyle::Alpha)
        .addProperty("DisabledAlpha", &ImGuiStyle::DisabledAlpha)
        .addProperty("WindowPadding", &ImGuiStyle::WindowPadding)
        .addProperty("WindowRounding", &ImGuiStyle::WindowRounding)
        .addProperty("WindowBorderSize", &ImGuiStyle::WindowBorderSize)
        .addProperty("WindowMinSize", &ImGuiStyle::WindowMinSize)
        .addProperty("WindowTitleAlign", &ImGuiStyle::WindowTitleAlign)
        .addProperty("WindowMenuButtonPosition", &ImGuiStyle::WindowMenuButtonPosition)
        .addProperty("ChildRounding", &ImGuiStyle::ChildRounding)
        .addProperty("ChildBorderSize", &ImGuiStyle::ChildBorderSize)
        .addProperty("PopupRounding", &ImGuiStyle::PopupRounding)
        .addProperty("PopupBorderSize", &ImGuiStyle::PopupBorderSize)
        .addProperty("FramePadding", &ImGuiStyle::FramePadding)
        .addProperty("FrameRounding", &ImGuiStyle::FrameRounding)
        .addProperty("FrameBorderSize", &ImGuiStyle::FrameBorderSize)
        .addProperty("ItemSpacing", &ImGuiStyle::ItemSpacing)
        .addProperty("ItemInnerSpacing", &ImGuiStyle::ItemInnerSpacing)
        .addProperty("CellPadding", &ImGuiStyle::CellPadding)
        .addProperty("TouchExtraPadding", &ImGuiStyle::TouchExtraPadding)
        .addProperty("IndentSpacing", &ImGuiStyle::IndentSpacing)
        .addProperty("ColumnsMinSpacing", &ImGuiStyle::ColumnsMinSpacing)
        .addProperty("ScrollbarSize", &ImGuiStyle::ScrollbarSize)
        .addProperty("ScrollbarRounding", &ImGuiStyle::ScrollbarRounding)
        .addProperty("GrabMinSize", &ImGuiStyle::GrabMinSize)
        .addProperty("GrabRounding", &ImGuiStyle::GrabRounding)
        .addProperty("LogSliderDeadzone", &ImGuiStyle::LogSliderDeadzone)
        .addProperty("TabRounding", &ImGuiStyle::TabRounding)
        .addProperty("TabBorderSize", &ImGuiStyle::TabBorderSize)
        .addProperty("TabMinWidthForCloseButton", &ImGuiStyle::TabMinWidthForCloseButton)
        .addProperty("ColorButtonPosition", &ImGuiStyle::ColorButtonPosition)
        .addProperty("ButtonTextAlign", &ImGuiStyle::ButtonTextAlign)
        .addProperty("SelectableTextAlign", &ImGuiStyle::SelectableTextAlign)
        .addProperty("SeparatorTextBorderSize", &ImGuiStyle::SeparatorTextBorderSize)
        .addProperty("SeparatorTextAlign", &ImGuiStyle::SeparatorTextAlign)
        .addProperty("SeparatorTextPadding", &ImGuiStyle::SeparatorTextPadding)
        .addProperty("DisplayWindowPadding", &ImGuiStyle::DisplayWindowPadding)
        .addProperty("DisplaySafeAreaPadding", &ImGuiStyle::DisplaySafeAreaPadding)
        .addProperty("MouseCursorScale", &ImGuiStyle::MouseCursorScale)
        .addProperty("AntiAliasedLines", &ImGuiStyle::AntiAliasedLines)
        .addProperty("AntiAliasedLinesUseTex", &ImGuiStyle::AntiAliasedLinesUseTex)
        .addProperty("AntiAliasedFill", &ImGuiStyle::AntiAliasedFill)
        .addProperty("CurveTessellationTol", &ImGuiStyle::CurveTessellationTol)
        .addProperty("CircleTessellationMaxError",
                     &ImGuiStyle::CircleTessellationMaxError)
        // .addProperty("Colors", &ImGuiStyle::Colors)
        .addProperty("HoverStationaryDelay", &ImGuiStyle::HoverStationaryDelay)
        .addProperty("HoverDelayShort", &ImGuiStyle::HoverDelayShort)
        .addProperty("HoverDelayNormal", &ImGuiStyle::HoverDelayNormal)
        .addProperty("HoverFlagsForTooltipMouse", &ImGuiStyle::HoverFlagsForTooltipMouse)
        .addProperty("HoverFlagsForTooltipNav", &ImGuiStyle::HoverFlagsForTooltipNav)
        .endClass();

    luabridge::getGlobalNamespace(L)
        .beginClass<ImColor>("ImColor")
        .addConstructor<void (*)(int, int, int, int)>()
        .addProperty("Value", &ImColor::Value)
        .endClass();

    luabridge::getGlobalNamespace(L)
        .beginNamespace("b")
        .ADD_FUNCTION(RequestAnimationFrameIn)
        .addFunction("GetGlobalFontSize",
                     []() { return ImGui::GetDefaultFont()->FontSize; })
        .endNamespace();

    auto ns = luabridge::getGlobalNamespace(L).beginNamespace("ImGui");
    NS_ADD_ENUM(ImGuiWindowFlags_);
    NS_ADD_ENUM(ImGuiButtonFlags_);
    NS_ADD_ENUM(ImGuiColorEditFlags_);
    NS_ADD_ENUM(ImGuiInputFlags_);
    NS_ADD_ENUM(ImGuiPopupFlags_);
    NS_ADD_ENUM(ImGuiScrollFlags_);
    NS_ADD_ENUM(ImGuiSelectableFlags_);
    NS_ADD_ENUM(ImGuiSeparatorFlags_);
    NS_ADD_ENUM(ImGuiSliderFlags_);
    NS_ADD_ENUM(ImGuiTabBarFlags_);
    NS_ADD_ENUM(ImGuiTabItemFlags_);
    NS_ADD_ENUM(ImGuiTextFlags_);

    ns.addFunction("Begin", [](const std::string& name, int flags) {
        return ImGui::Begin(name.c_str(), nullptr, flags);
    });
    ns.addFunction("End", &ImGui::End);
    ns.addFunction(
        "BeginChild",
        [](const std::string& name, const ImVec2& size, bool border, int flags) -> bool {
            return ImGui::BeginChild(name.c_str(), size, border, flags);
        });
    ns.addFunction("EndChild", &ImGui::EndChild);
    ns.addFunction("SameLine", []() { ImGui::SameLine(); });
    ns.addFunction("PushIDInt", [](int id) { ImGui::PushID(id); });
    ns.addFunction("PushIDStr",
                   [](const std::string& id) { ImGui::PushID(id.c_str()); });
    ns.addFunction("PopID", &ImGui::PopID);
    ns.addFunction("ShowDemoWindow", []() { ImGui::ShowDemoWindow(); });
    ns.addFunction("BeginCombo",
                   [](const std::string& name, const std::string& currentItem) {
                       return ImGui::BeginCombo(name.c_str(), currentItem.c_str());
                   });
    ns.addFunction("EndCombo", []() { ImGui::EndCombo(); });
    ns.addFunction("SetNextWindowPos",
                   [](const ImVec2& pos) { ImGui::SetNextWindowPos(pos); });
    ns.addFunction("SetNextWindowSize",
                   [](const ImVec2& size) { ImGui::SetNextWindowSize(size); });
    ns.addFunction("Selectable", [](const std::string& text, bool isSelected) {
        return ImGui::Selectable(text.c_str(), isSelected);
    });
    ns.addFunction("SetItemDefaultFocus", []() { return ImGui::SetItemDefaultFocus(); });
    ns.addFunction("GetIO", []() { return ImGui::GetIO(); });

    ns.ADD_FUNCTION(Text);
    ns.ADD_FUNCTION(Button);

    ns.ADD_FUNCTION(ApplyStyleColorPermanent);
    ns.ADD_FUNCTION(ApplyStyleVec2Permanent);
    ns.ADD_FUNCTION(ApplyStyleFloatPermanent);

    ns.ADD_FUNCTION(PushStyleColor);
    ns.ADD_FUNCTION(PushStyleVarVec2);
    ns.ADD_FUNCTION(PushStyleVarFloat);

    ns.ADD_FUNCTION(PopStyleColor);
    ns.ADD_FUNCTION(PopStyleVarVec2);
    ns.ADD_FUNCTION(PopStyleVarFloat);

    ns.ADD_IMGUI_FUNCTION(GetFontSize);
    ns.ADD_IMGUI_FUNCTION(SetCursorPos);
    ns.ADD_IMGUI_FUNCTION(GetCursorPosX);
    ns.ADD_IMGUI_FUNCTION(GetCursorPosY);
    ns.ADD_IMGUI_FUNCTION(GetStyle);

    ns.ADD_IMGUI_FUNCTION(SetNextItemWidth);
    ns.ADD_IMGUI_FUNCTION(GetWindowContentRegionMin);
    ns.ADD_IMGUI_FUNCTION(GetWindowContentRegionMax);

    ns.endNamespace();
}

lua_State* ImGuiLua::CreateLuaState()
{
    lua_State* state = luaL_newstate();
    luaL_openlibs(state);
    DeclareLuaBridge(state);
    return state;
}

std::expected<std::nullopt_t, std::string>
ImGuiLua::RunLuaString(lua_State* L,
                       const std::string& lua,
                       const std::string& luaDebugFilename)
{
    int loadStatus
        = luaL_loadbuffer(L, lua.c_str(), lua.size(), luaDebugFilename.c_str());
    if (loadStatus == LUA_OK) {
        int execStatus = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (execStatus != LUA_OK) {
            const char* errorMessage = lua_tostring(L, -1);
            auto error = b::format("{}", errorMessage);
            b::log::error("{}", error);
            return std::unexpected(error);
            lua_pop(L, 1);
        }
    }
    else {
        const char* errorMessage = lua_tostring(L, -1);
        auto error = b::format("{}", errorMessage);
        b::log::error("{}", error);
        return std::unexpected(error);
        lua_pop(L, 1);
    }
    return std::nullopt;
}

std::expected<std::nullopt_t, std::string>
ImGuiLua::CallLuaFunction(lua_State* L,
                          const std::string& functionName,
                          const std::string& luaDebugFilename)
{
    luabridge::LuaRef const func = luabridge::getGlobal(L, functionName.c_str());
    if (func.isFunction()) {
        luabridge::LuaResult const res = func();
        if (!res) {
            if (!luaDebugFilename.empty()) {
                auto error = b::format(
                    "{}(): {}", luaDebugFilename, functionName, res.errorMessage());
                b::log::error("{}", error);
                return std::unexpected(error);
            }
            else {
                auto error = b::format("{}(): {}", functionName, res.errorMessage());
                b::log::error("{}", error);
                return std::unexpected(error);
            }
        }
    }
    else {
        auto error = b::format("{}(): Function does not exist", functionName);
        b::log::error("{}", error);
        return std::unexpected(error);
    }
    return std::nullopt;
}

} // namespace b
