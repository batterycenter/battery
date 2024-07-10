
local boundingBoxArgs = { "left", "right", "top", "bottom", "width", "height", "margin" }
__id = 0

function math.clamp(x, min, max)
    return math.max(math.min(x, max), min)
end

function getID()
    __id = __id + 1
    return __id
end

function preRender()
    __id = 0
end

function postRender()

end

function Button(args)
    __checkArgs("Button", args, { boundingBoxArgs, { "text" }})

    local text = args.text or "Button"
    local bb = getBoundingBox(args)
    ImGui.SetCursorPos(ImVec2(bb.left, bb.top))
    ImGui.PushIDInt(getID())
    local ret = ImGui.Button(text, ImVec2(bb.right-bb.left, bb.bottom-bb.top))
    ImGui.PopID()
    return ret
end

function Window(args, callback)
    __checkArgs("Window", args, {{ "title", "flags" }})

    ImGui.PushIDInt(getID())
    if ImGui.Begin(args.title or "Window", 0) then
        callback()
    end
    ImGui.End()
    ImGui.PopID()
end

function Dropdown(args)
    local items = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO", "PPPP", "QQQQQQQQQQ", "RRR", "SSSS" }
    local current_item = items[1];

    if (ImGui.BeginCombo("##combo", current_item)) then
        for n = 0..items.length do
            -- ImGui.Selectable(n)
        --     bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
        --     if ImGui.Selectable(items[n], is_selected) then
        --         current_item = items[n];
        --     end
        --     if is_selected then
        --         ImGui.SetItemDefaultFocus()
            -- end
        endfor
        ImGui.EndCombo();
    end
end

function Container(args, callback)
    __checkArgs("Container", args,
            { boundingBoxArgs, { "flags", "nativeBorder", "childArgs1", "childArgs2" }})

    local bb = getBoundingBox(args)
    ImGui.SetCursorPos(ImVec2(bb.left, bb.top))
    ImGui.PushStyleVarVec2("ImGuiStyleVar_WindowPadding", ImVec2(0, 0))
    ImGui.PushIDInt(getID())
    ImGui.BeginChild("##container", ImVec2(bb.right - bb.left, bb.bottom - bb.top),
                     args.nativeBorder or false, args.flags or 0)
    ImGui.PopStyleVarVec2()

    if callback then
        callback(args.childArgs1, args.childArgs2)
    end

    ImGui.EndChild()
    ImGui.PopID()
end

function Grid(args, callback)
    __checkArgs("Grid", args, { boundingBoxArgs, { "rows", "columns", "nativeBorder",
                                              "cellWidths", "cellHeights" }})

    function shareSpaceAmongCells(cellWidths, totalSize, numberOfCells)
        local results = {}

        -- First apply all absolute values
        local absoluteSum = 0;
        for i=1,numberOfCells do
            local unit = toUnit(cellWidths[i] or "1")
            if unit.unit == "px" then
                results[i] = unit.value
                absoluteSum = absoluteSum + unit.value
            end
        end

        -- If all absolute cells already fill the area, we're done
        if absoluteSum >= totalSize then
            return results
        end

        -- Now fill in the relative PERCENT cells
        local percentSum = 0;
        for i=1,numberOfCells do
            local unit = toUnit(cellWidths[i] or "1")
            if unit.unit == "%" then
                local proportion = math.clamp(unit.value / 100, 0, 1)
                results[i] = totalSize * proportion;
                percentSum = percentSum + totalSize * proportion;
            end
        end

        -- Stop if absolute and percent fill the area
        if absoluteSum + percentSum >= totalSize then
            return results
        end

        -- Now fill in the relative EM columns
        local emSum = 0;
        for i=1,numberOfCells do
            local unit = toUnit(cellWidths[i] or "1")
            if unit.unit == "em" then
                results[i] = unit.value * ImGui.GetFontSize()
                emSum = emSum + unit.value * ImGui.GetFontSize()
            end
        end

        -- Stop if absolute, percent and em fill the area
        if absoluteSum + percentSum + emSum >= totalSize then
            return results
        end

        -- And finally, share the remaining space proportionally between the UNITLESS cells
        local unitlessSum = 0;
        for i=1,numberOfCells do
            local unit = toUnit(cellWidths[i] or "1")
            if unit.unit == "unitless" then
                unitlessSum = unitlessSum + unit.value
            end
        end

        if unitlessSum ~= 0 then
            local remaining = totalSize - absoluteSum - percentSum - emSum;
            for i=1,numberOfCells do
                local unit = toUnit(cellWidths[i] or "1")
                if unit.unit == "unitless" then
                    results[i] = remaining * (unit.value / unitlessSum);
                end
            end
        end

        return results
    end

    local bb = getBoundingBox(args)
    local widths = shareSpaceAmongCells(args.cellWidths or {}, bb.width, args.columns or 1)
    local heights = shareSpaceAmongCells(args.cellHeights or {}, bb.height, args.rows or 1)

    local y = 0
    for row=0, args.rows - 1 do
        local x = 0
        for col=0, args.columns - 1 do
            if callback then
                Container({
                        nativeBorder=args.nativeBorder,
                        width=widths[col+1].."px",
                        height=heights[row+1].."px",
                        left=bb.left + x.."px",
                        top=bb.top + y.."px",
                        flags=ImGui.ImGuiWindowFlags_NoScrollbar + ImGui.ImGuiWindowFlags_NoScrollWithMouse,
                        childArgs1=row,
                        childArgs2=col
                    }, callback)
            end
            x = x + widths[col+1]
        end
        y = y + heights[row+1]
    end
end
