import b

class MainWindow:
    main_window = b.widgets.window(b.main_window_context)
    main_window.name = "Steam Login Page"
    main_window.border_width = 0
    main_window.position = b.ImVec2(0, 0)
    main_window.style["window-titlebar-color"] = "#202227"
    main_window.style["window-titlebar-color-active"] = "#202227"
    main_window.style["window-titlebar-color-collapsed"] = "#202227"
    main_window.style["window-background-color"] = "#191a1e"
    main_window.style["ImGuiStyleVar_FramePadding"] = ("10px", "10px")
    main_window.style["ImGuiStyleVar_WindowRounding"] = "0px"
    main_window.flags = \
        b.ImGuiWindowFlags.NoResize | \
        b.ImGuiWindowFlags.NoMove | \
        b.ImGuiWindowFlags.NoCollapse | \
        b.ImGuiWindowFlags.NoBringToFrontOnFocus

    main_vertical_grid = b.widgets.grid()
    main_vertical_grid.cell_widths = [ b.unit_property(1) ]
    main_vertical_grid.cell_heights = [ b.unit_property("100"), b.unit_property("10") ]
    main_vertical_grid.left.set("5%")
    main_vertical_grid.top.set("12%")
    main_vertical_grid.width.set("92%")
    main_vertical_grid.height.set("92%")
    main_vertical_grid.frame_border = True
    main_vertical_grid.cell_border = True

    upper_grid = b.widgets.grid()
    upper_grid.cell_widths = [ b.unit_property(1), b.unit_property(1) ]
    upper_grid.cell_heights = [ b.unit_property(1) ]
    upper_grid.left.set("0")
    upper_grid.top.set("0")
    upper_grid.width.set("100%")
    upper_grid.height.set("100%")
    upper_grid.frame_border = True
    upper_grid.cell_border = True

    steam_logo = b.widgets.image()
    steam_logo.left.set("0")
    steam_logo.top.set("0")
    steam_logo.src = main_window.context.steam_logo

    steam_text = b.widgets.text()
    steam_text.label = "STEAM"
    steam_text_trademark = b.widgets.text()
    steam_text_trademark.label = "®"

    sign_in_label = b.widgets.text()
    sign_in_label.label = "SIGN IN WITH ACCOUNT NAME"
    password_label = b.widgets.text()
    password_label.label = "PASSWORD"
    username_input = b.widgets.input()
    username_input.name = "##"
    password_input = b.widgets.input()
    password_input.name = "##"

    remember_me_checkbox = b.widgets.checkbox()
    remember_me_checkbox.name = "Remember me"

    login_button = b.widgets.fancy_button()
    login_button.name = "Sign in"
    login_button.style["button-gradient-color-left"] = "#07bfff"
    login_button.style["button-gradient-color-right"] = "#2d73ff"

    def __init__(self):
        b.load_font("SteamText", "roboto-bold", 70)
        b.load_font("SteamTextTrademark", "roboto-bold", 25)
        b.load_font("InputLabel", "roboto-bold", 20)
        b.load_font("InputText", "roboto-bold", 20)
        pass

    def left_upper_region(self):

        # Render the title Steam logo and label
        self.steam_logo()
        b.sameline()
        b.push_font("SteamText")
        self.steam_text()
        b.pop_font()
        b.sameline()
        b.push_font("SteamTextTrademark")
        self.steam_text_trademark()
        b.pop_font()

        b.widgets.vspace(20)

        # Render the username and password fields
        b.push_font("InputLabel")
        self.sign_in_label()
        b.pop_font()
        b.push_font("InputText")
        self.username_input()
        b.pop_font()

        b.widgets.vspace(20)

        b.push_font("InputLabel")
        self.password_label()
        b.pop_font()
        b.push_font("InputText")
        self.password_input()
        b.pop_font()

        b.widgets.vspace(20)

        # Render the remember me checkbox
        self.remember_me_checkbox()

        b.widgets.vspace(20)

        # Render the login button
        self.login_button()

        #b.show_demo_window()

    def right_upper_region(self):
        pass

    def upper_content_region(self):
        def upper_grid(call):
            call(0, 0, self.left_upper_region)
            call(1, 0, self.right_upper_region)
        self.upper_grid(upper_grid)

    def lower_content_region(self):
        pass

    def render(self):
        def main_vertical_grid(cell):
            cell(0, 0, self.upper_content_region)
            cell(0, 1, self.lower_content_region)
        self.main_vertical_grid(main_vertical_grid)
        pass

    def __call__(self):
        self.main_window.size = b.main_window_context.window_size
        self.main_window.context.titlebar_hovered = self.main_window.titlebar_hovered
        self.main_window.context.handle_window_dragging()
        if not self.main_window.is_open:
            b.app_context.stop_application()
        self.main_window(self.render)

main_window = MainWindow()
b.init_main_window(main_window)