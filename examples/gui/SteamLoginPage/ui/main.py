import b

main_window = b.widgets.window(b.main_window_context)
main_window.name = "Steam Login Page"
main_window.size = b.ImVec2(400, 600)
main_window.border_width = 0
main_window.position = b.ImVec2(0, 0)
main_window.style["window-titlebar-color"] = "#202227"
main_window.style["window-titlebar-color-active"] = "#202227"
main_window.style["window-titlebar-color-collapsed"] = "#202227"
main_window.style["window-background-color"] = "#191a1e"
# main_window.style["ImGuiStyleVar_FramePadding"] = { "10px", "10px" }

button = b.widgets.button()
button.style["border-radius"] = "5px"

fancy_button = b.widgets.fancy_button()
fancy_button.style["button-gradient-color-left"] = "#06bfff"
fancy_button.style["button-gradient-color-right"] = "#2d73ff"

text = b.widgets.text()
text.label = "Click me!"
text.style["text-color"] = "#06bfff"

input = b.widgets.input("##")
input.hint = "Enter your username"
input.style["border-radius"] = "10px"
input.style["text-color"] = "#06bfff"
input.style["text-color-disabled"] = "#aaaaaa"
input.style["input-color"] = "#32353c"

def ui_loop():

    def main_ui():
        button()
        b.widgets.vspace(50)
        fancy_button()
        b.widgets.vspace(50)
        text()
        b.widgets.vspace(50)
        input()

    main_window(main_ui)

b.init(ui_loop)