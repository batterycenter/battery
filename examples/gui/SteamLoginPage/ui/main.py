import b

class MainWindow:
    main_window = b.widgets.window(b.main_window_context)
    main_window.name = "Steam Login Page"
    main_window.left.set(0)
    main_window.top.set(0)
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
    main_vertical_grid.cell_heights = [ b.unit_property("20"), b.unit_property("100"), b.unit_property("10") ]
    main_vertical_grid.left.set("5%")
    main_vertical_grid.top.set("12%")
    main_vertical_grid.width.set("92%")
    main_vertical_grid.height.set("92%")
    main_vertical_grid.frame_border = True
    main_vertical_grid.cell_border = True

    upper_grid = b.widgets.grid()
    upper_grid.cell_widths = [ b.unit_property(2), b.unit_property(1) ]
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
    steam_text.style.font = "SteamText"
    # steam_text.style["ImGuiStyleVar_FramePadding"] = ("0", "23px")
    steam_text_trademark = b.widgets.text()
    steam_text_trademark.label = "®"
    steam_text_trademark.style.font = "SteamTextTrademark"

    sign_in_label = b.widgets.text()
    sign_in_label.label = "SIGN IN WITH ACCOUNT NAME"
    sign_in_label.style.font = "InputLabel"
    sign_in_label.style["ImGuiCol_Text"] = "#3c96fc"
    password_label = b.widgets.text()
    password_label.label = "PASSWORD"
    password_label.style.font = "InputLabel"
    password_label.style["ImGuiCol_Text"] = "#808080"
    username_input = b.widgets.input()
    username_input.name = "##"
    username_input.right.set("10%")
    username_input.style.font = "InputText"
    username_input.style["ImGuiStyleVar_FramePadding"] = ("13px", "13px")
    password_input = b.widgets.input()
    password_input.name = "##"
    password_input.right.set("10%")
    password_input.style.font = "InputText"
    password_input.style["ImGuiStyleVar_FramePadding"] = ("13px", "13px")

    remember_me_checkbox = b.widgets.checkbox_ex()
    remember_me_checkbox.inner_padding_factor = 1.3
    remember_me_checkbox.name = "Remember me"
    remember_me_checkbox.style["ImGuiCol_Text"] = "#808080"
    remember_me_checkbox.style["ImGuiCol_CheckMark"] = "#FFFFFF"
    # remember_me_checkbox.style["ImGuiCol_FrameBgHovered"] = remember_me_checkbox.style["ImGuiCol_FrameBg"]

    login_button = b.widgets.fancy_button()
    login_button.name = "Sign in"
    login_button.left.set("10%")
    login_button.right.set("20%")
    login_button.style.font = "LoginButton"
    login_button.style["button-gradient-color-left"] = "#07bfff"
    login_button.style["button-gradient-color-right"] = "#2d73ff"
    login_button.style["ImGuiStyleVar_FramePadding"] = ("0", "23px")

    qrcode_label = b.widgets.text()
    qrcode_label.label = "OR SIGN IN WITH QR"
    qrcode_label.style.font = "InputLabel"
    qrcode_label.style["ImGuiCol_Text"] = "#3c96fc"

    qrcode_background = b.widgets.rect()
    qrcode_background.fill_color.set("#FFFFFF")
    qrcode_background.left.set("1%")
    qrcode_background.top.set("1%")
    qrcode_background.right.set("1%")
    qrcode_background.bottom.set("1%")
    qrcode_background.style["ImGuiStyleVar_FrameRounding"] = "20px"
    qrcode_image = b.widgets.image()
    qrcode_image.src = main_window.context.qrcode_texture
    qrcode_image.left.set("10%")
    qrcode_image.top.set("10%")
    qrcode_image.right.set("10%")
    qrcode_image.bottom.set("10%")
    qrcode_container = b.widgets.container()
    qrcode_container.left.set("0%")
    qrcode_container.width.set("100%")

    def __init__(self):
        b.load_font("SteamText", "roboto-bold", 44)
        b.load_font("SteamTextTrademark", "roboto-bold", 15)
        b.load_font("InputLabel", "roboto-bold", 18)
        b.load_font("InputText", "roboto-regular", 20)
        b.load_font("LoginButton", "roboto-bold", 30)

    def render(self):

        def title_content_region():   # Render the title Steam logo and label
            self.steam_logo()
            b.sameline()
            self.steam_text()
            b.sameline()
            self.steam_text_trademark()
        def left_main_region():
            # Render the username and password fields
            self.sign_in_label()
            self.username_input()
            b.widgets.vspace(20)
            self.password_label()
            self.password_input()

            b.widgets.vspace(20)

            # Render the remember me checkbox
            self.remember_me_checkbox()

            b.widgets.vspace(20)

            # Render the login button
            self.login_button()

            #b.show_demo_window()

        def right_main_region():
            self.qrcode_label()
            def qrcode():
                self.qrcode_background()
                self.qrcode_image()
            self.qrcode_container.height.set(self.qrcode_container.actual_size.x)
            self.qrcode_container(qrcode)

        def main_content_region():
            def upper_grid(call):
                call(0, 0, left_main_region)
                call(1, 0, right_main_region)
            self.upper_grid(upper_grid)

        def lower_content_region():
            pass

        def main_vertical_grid(cell):
            cell(0, 0, title_content_region)
            cell(0, 1, main_content_region)
            cell(0, 2, lower_content_region)
        self.main_vertical_grid(main_vertical_grid)

    def __call__(self):
        self.main_window.width.set(b.main_window_context.window_size.x)
        self.main_window.height.set(b.main_window_context.window_size.y)
        self.main_window.context.titlebar_hovered = self.main_window.titlebar_hovered
        self.main_window.context.handle_window_dragging()
        if not self.main_window.is_open:
            b.app_context.stop_application()
        self.main_window(self.render)

        b.show_demo_window()

main_window = MainWindow()
b.init_main_window(main_window)