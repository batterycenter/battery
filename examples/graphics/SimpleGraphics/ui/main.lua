
function checkArgs(func, args, expectedList)
    local expected = {}
    for _, v in ipairs(expectedList) do
        expected[v] = true
    end
    for key, _ in pairs(args) do
        if not expected[key] then
            print("[WARNING] Function " .. func .. "() has no parameter '"..key.."'")
        end
    end
end

function to_unitvalue(string)
    if string == nil then
        return nil
    end
    function getValueFromString(string, unit)
        local value = tonumber(string:sub(1, -1 - #unit))
        if value == nil then
            error("unit '"..string.."': Expected a number")
        end
        return {value=value, unit=unit}
    end
    function getColorFromString(string)
        if #string == 9 then                -- #RRGGBBAA
            local r = tonumber(string:sub(2, 3), 16)
            local g = tonumber(string:sub(4, 5), 16)
            local b = tonumber(string:sub(6, 7), 16)
            local a = tonumber(string:sub(8, 9), 16)
            return {value={r=r, g=g, b=b, a=a}, unit="color"}
        elseif #string == 7 then            -- #RRGGBB
            local r = tonumber(string:sub(2, 3), 16)
            local g = tonumber(string:sub(4, 5), 16)
            local b = tonumber(string:sub(6, 7), 16)
            return {value={r=r, g=g, b=b, a=255}, unit="color"}
        else
            error("unit '"..string.."': Expected a color in the format #RRGGBBAA or #RRGGBB")
        end
    end

    if type(string) == "string" then    -- String input
        if string:sub(-2) == "px" then
            return getValueFromString(string, "px")
        elseif string:sub(-1) == "%" then
            return getValueFromString(string, "%")
        elseif string:sub(-2) == "em" then
            return getValueFromString(string, "em")
        elseif string:sub(1,1) == "#" then
            return getColorFromString(string)
        else
            local value = tonumber(string)
            if value then
                return {value=value, unit="unitless"}
            else
                error("unit '"..string.."': No known unit found")
            end
        end

    elseif type(string) == "number" then    -- Number input
        local value = tonumber(string)
        if value == nil then
            error("unit '"..string.."': Expected a number")
        end
        return {value=value, unit="unitless"}

    else                                -- Invalid input datatype
        error("unit '"..string.."': Expected a string")
    end
    return result
end

function getHorizontalPx(unitvalue)
    if unitvalue == nil then
        return nil
    end
    local px = 0
    if unitvalue.unit == "px" then
        px = unitvalue.value
    elseif unitvalue.unit == "%" then
        px = unitvalue.value * (ImGui.GetWindowContentRegionMax().x - ImGui.GetWindowContentRegionMin().x) / 100.0
    elseif unitvalue.unit == "em" then
        px = unitvalue.value * ImGui.GetFontSize()
    else
        px = unitvalue.value
    end
    return px
end

function getVerticalPx(unitvalue)
    if unitvalue == nil then
        return nil
    end
    local px = 0
    if unitvalue.unit == "px" then
        px = unitvalue.value
    elseif unitvalue.unit == "%" then
        px = unitvalue.value * (ImGui.GetWindowContentRegionMax().y - ImGui.GetWindowContentRegionMin().y) / 100.0
    elseif unitvalue.unit == "em" then
        px = unitvalue.value * ImGui.GetFontSize()
    else
        px = unitvalue.value
    end
    return px
end

function getBoundingBox(args)
    local left_px = getHorizontalPx(to_unitvalue(args.left or nil))
    local right_px = getHorizontalPx(to_unitvalue(args.right or nil))
    local width_px = getHorizontalPx(to_unitvalue(args.width or nil))
    local top_px = getVerticalPx(to_unitvalue(args.top or nil))
    local bottom_px = getVerticalPx(to_unitvalue(args.bottom or nil))
    local height_px = getVerticalPx(to_unitvalue(args.height or nil))

    local cursor_left = ImGui.GetCursorPosX()
    local cursor_width = 0
    if width_px and right_px then
        cursor_left = ImGui.GetWindowContentRegionMax().x - right_px - width_px
        cursor_width = width_px
    elseif left_px and right_px then
        cursor_left = left_px
        cursor_width = ImGui.GetWindowContentRegionMax().x - right_px - left_px
    elseif left_px and width_px then
        cursor_left = left_px
        cursor_width = width_px
    elseif right_px then
        cursor_width = ImGui.GetWindowContentRegionMax().x - right_px - cursor_left
    elseif left_px then
        cursor_left = left_px
    elseif width_px then
        cursor_width = width_px
    end

    local cursor_top = ImGui.GetCursorPosY()
    local cursor_height = 0
    if height_px and bottom_px then
        cursor_top = ImGui.GetWindowContentRegionMax().y - bottom_px - height_px
        cursor_height = height_px
    elseif top_px and bottom_px then
        cursor_top = top_px
        cursor_height = ImGui.GetWindowContentRegionMax().y - bottom_px - top_px
    elseif top_px and height_px then
        cursor_top = top_px
        cursor_height = height_px
    elseif bottom_px then
        cursor_height = ImGui.GetWindowContentRegionMax().y - bottom_px - cursor_top
    elseif top_px then
        cursor_top = top_px
    elseif height_px then
        cursor_height = height_px
    end

    return {left=cursor_left, top=cursor_top, right=cursor_left+cursor_width, bottom=cursor_top+cursor_height}
end

function loadStyle(style)
    for key, value in pairs(style) do
        if key:sub(1, 9) == "ImGuiCol_" then
            local unitvalue = to_unitvalue(value)
            local color = ImColor(unitvalue.value.r, unitvalue.value.g, unitvalue.value.b, unitvalue.value.a)
            ImGui.ApplyStyleColorPermanent(key, color)
        elseif key:sub(1, 14) == "ImGuiStyleVar_" then
            if type(value) == "string" then
                if unitvalue.unit == "color" then
                else
                    ImGui.ApplyStyleFloatPermanent(key, unitvalue.value)
                end
            elseif type(value) == "table" then
                ImGui.ApplyStyleVec2Permanent(key, ImVec2(value[1], value[2]))
            end
        else
            error("Unknown style key '"..key.."'")
        end
    end
end

function loadDefaultBatteryStyle()
    local style = {
            ImGuiCol_Text = "#FFFFFF",
            ImGuiCol_TextDisabled = "#7F7F7F",
            ImGuiCol_WindowBg = "#202020EE",
            ImGuiCol_ChildBg = "#00000000",
            ImGuiCol_PopupBg = "#222222EF",
            ImGuiCol_Border = "#6D6D7F7F",
            ImGuiCol_BorderShadow = "#00000000",
            ImGuiCol_FrameBg = "#6060607A",
            ImGuiCol_FrameBgHovered = "#969696CC",
            ImGuiCol_FrameBgActive = "#A0A0A0",
            ImGuiCol_TitleBg = "#0A0A0A",
            ImGuiCol_TitleBgActive = "#3D3D3D",
            ImGuiCol_TitleBgCollapsed = "#00000082",
            ImGuiCol_MenuBarBg = "#232323",
            ImGuiCol_ScrollbarBg = "#05050587",
            ImGuiCol_ScrollbarGrab = "#4F4F4F",
            ImGuiCol_ScrollbarGrabHovered = "#686868",
            ImGuiCol_ScrollbarGrabActive = "#828282",
            ImGuiCol_CheckMark = "#54FF00",
            ImGuiCol_SliderGrab = "#B2B2B2CC",
            ImGuiCol_SliderGrabActive = "#C1C1C1",
            ImGuiCol_Button = "#8C8C8C60",
            ImGuiCol_ButtonHovered = "#969696CC",
            ImGuiCol_ButtonActive = "#A0A0A0",
            ImGuiCol_Header = "#8C8C8C60",
            ImGuiCol_HeaderHovered = "#969696CC",
            ImGuiCol_HeaderActive = "#A0A0A0",
            ImGuiCol_Separator = "#6D6D7F7F",
            ImGuiCol_SeparatorHovered = "#54FF00",
            ImGuiCol_SeparatorActive = "#78fc38",
            ImGuiCol_ResizeGrip = "#4296F93F",
            ImGuiCol_ResizeGripHovered = "#4296F9AA",
            ImGuiCol_ResizeGripActive = "#4296F9F2",
            ImGuiCol_Tab = "#4E4E4ED5",
            ImGuiCol_TabHovered = "#969696CC",
            ImGuiCol_TabActive = "#6B6B6B",
            ImGuiCol_TabUnfocused = "#1F1F1FD5",
            ImGuiCol_TabUnfocusedActive = "#3C3C3C",
            ImGuiCol_PlotLines = "#9B9B9B",
            ImGuiCol_PlotLinesHovered = "#FF6D59",
            ImGuiCol_PlotHistogram = "#E5B200",
            ImGuiCol_PlotHistogramHovered = "#FF9900",
            ImGuiCol_TextSelectedBg = "#A0A0A059",
            ImGuiCol_DragDropTarget = "#FFFF00E5",
            ImGuiCol_NavHighlight = "#4296F9",
            ImGuiCol_NavWindowingHighlight = "#FFFFFFB2",
            ImGuiCol_NavWindowingDimBg = "#CCCCCC33",
            ImGuiCol_ModalWindowDimBg = "#CCCCCC59",
            ImGuiStyleVar_FramePadding = { 5, 5 },
            ImGuiStyleVar_WindowRounding = 8,
            ImGuiStyleVar_FrameRounding = 6,
            ImGuiStyleVar_PopupRounding = 6,
            ImGuiStyleVar_GrabRounding = 6,
            ImGuiStyleVar_ChildRounding = 6,
            ImGuiStyleVar_ItemSpacing = { 10, 4 }
    }
    loadStyle(style)
end

function Button(args)
    checkArgs("Button", args, { "text", "left", "right", "top", "bottom", "width", "height" })

    local text = args.text or "Button"
    local bb = getBoundingBox(args)
    ImGui.SetCursorPos(ImVec2(bb.left, bb.top))
    ImGui.Button(text, ImVec2(bb.right-bb.left, bb.bottom-bb.top))
end

function Window(args, callback)
    checkArgs("Window", args, { "title" })

    ImGui.Begin(args.title or "Window")
    callback()
    ImGui.End()
end

function render()
    ImGui.ShowDemoWindow()

    function Test1()
--         Button({text="Click me!"})
    end

    function Test2()
        Button({text="Click me 2!", width="100%", left="0"})
    end

    Window({title="Test1"}, Test1)
    Window({title="Test2"}, Test2)

end

loadDefaultBatteryStyle()