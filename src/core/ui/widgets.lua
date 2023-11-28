
function Button(args)
    __checkArgs("Button", args, { "text", "left", "right", "top", "bottom", "width", "height" })

    local text = args.text or "Button"
    local bb = __getBoundingBox(args)
    ImGui.SetCursorPos(ImVec2(bb.left, bb.top))
    return ImGui.Button(text, ImVec2(bb.right-bb.left, bb.bottom-bb.top))
end

function Window(args, callback)
    __checkArgs("Window", args, { "title" })

    ImGui.Begin(args.title or "Window")
    callback()
    ImGui.End()
end
