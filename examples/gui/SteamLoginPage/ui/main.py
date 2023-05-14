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
main_window.flags = \
    b.ImGuiWindowFlags.NoResize | \
    b.ImGuiWindowFlags.NoMove | \
    b.ImGuiWindowFlags.NoCollapse | \
    b.ImGuiWindowFlags.NoBringToFrontOnFocus
main_window.style["ImGuiStyleVar_FramePadding"] = ("10px", "10px")

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

image = b.widgets.image()
image.src = main_window.context.steam_logo
image.width.set(64)
image.height.set(64)

grid = b.widgets.grid()
grid.width = b.unit_property(300)
grid.height = b.unit_property(300)
grid.cell_widths = [ b.unit_property(1), b.unit_property(2), b.unit_property(3) ]
grid.cell_heights = [ b.unit_property(1), b.unit_property(2), b.unit_property(3) ]

checkbox = b.widgets.checkbox()
checkbox.name = "My Checkbox"

def ui_loop():

    def main_ui():

        main_window.context.titlebar_hovered = main_window.titlebar_hovered
        main_window.context.handle_window_dragging()
        main_window.size = main_window.context.window_size

        if not main_window.is_open:
            b.app_context.stop_application()

        # button()
        # b.widgets.vspace(50)
        fancy_button()
        # b.widgets.vspace(50)
        text()
        # b.widgets.vspace(50)
        input()
        # b.widgets.vspace(50)
        image()
        # b.widgets.vspace(50)

        grid.frame_border = True
        grid.cell_border = True
        button.width.set("100%")
        button.height.set("100%")

        def render_grid(cell):
            cell(0, 0, lambda: button())
            cell(1, 0, lambda: button())
            cell(2, 0, lambda: button())
            cell(0, 1, lambda: button())
            cell(1, 1, lambda: button())
            cell(2, 1, lambda: button())
            cell(0, 2, lambda: button())
            cell(1, 2, lambda: button())
            cell(2, 2, lambda: button())

        grid(render_grid)
        checkbox()

    main_window(main_ui)
    b.show_demo_window()

b.init_main_window(ui_loop)