import b

class MainWindow:
    main_window = b.widgets.panel(b.MainWindowContext)

    main_window.name = "Steam Login Page"
    main_window.left = 0
    main_window.top = 0
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
    main_vertical_grid.left = "5%"
    main_vertical_grid.top = "12%"
    main_vertical_grid.width = "92%"
    main_vertical_grid.height = "92%"
    # main_vertical_grid.frame_border = True
    # main_vertical_grid.cell_border = True

    upper_grid = b.widgets.grid()
    upper_grid.cell_widths = [ b.unit_property(2), b.unit_property(1) ]
    upper_grid.cell_heights = [ b.unit_property(1) ]
    upper_grid.left = "0"
    upper_grid.top = "0"
    upper_grid.width = "100%"
    upper_grid.height = "100%"
    # upper_grid.frame_border = True
    # upper_grid.cell_border = True

    steam_logo = b.widgets.image()
    steam_logo.left = "0"
    steam_logo.top = "0"
    steam_logo.src = main_window.context.steam_logo

    steam_text = b.widgets.text()
    steam_text.label = "STEAM"
    steam_text.style.font = "SteamText"
    steam_text.height = "64"
    steam_text_trademark = b.widgets.text()
    steam_text_trademark.label = "®"
    steam_text_trademark.top = "10"
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
    username_input.right = "10%"
    username_input.style.font = "InputText"
    username_input.style["ImGuiStyleVar_FramePadding"] = ("13px", "13px")
    password_input = b.widgets.input()
    password_input.name = "##"
    password_input.right = "10%"
    password_input.style.font = "InputText"
    password_input.style["ImGuiStyleVar_FramePadding"] = ("13px", "13px")
    password_input.flags = b.ImGuiInputTextFlags.Password

    remember_me_checkbox = b.widgets.checkbox_ex()
    remember_me_checkbox.inner_padding_factor = 1.3
    remember_me_checkbox.name = "Remember me"
    remember_me_checkbox.style["ImGuiCol_Text"] = "#808080"
    remember_me_checkbox.style["ImGuiCol_CheckMark"] = "#FFFFFF"
    # remember_me_checkbox.style["ImGuiCol_FrameBgHovered"] = remember_me_checkbox.style["ImGuiCol_FrameBg"]

    login_button = b.widgets.button_ex()
    login_button.name = "Sign in"
    login_button.left = "10%"
    login_button.right = "20%"
    login_button.style.font = "LoginButton"
    login_button.style["button-gradient-color-left"] = "#07bfff"
    login_button.style["button-gradient-color-right"] = "#2d73ff"
    login_button.style["ImGuiStyleVar_FramePadding"] = ("0", "23px")

    qrcode_label = b.widgets.text()
    qrcode_label.label = "OR SIGN IN WITH QR"
    qrcode_label.style.font = "InputLabel"
    qrcode_label.style["ImGuiCol_Text"] = "#3c96fc"

    qrcode_background = b.widgets.rect()
    qrcode_background.fill_color = "#FFFFFF"
    qrcode_background.left = "1%"
    qrcode_background.top = "1%"
    qrcode_background.right = "1%"
    qrcode_background.bottom = "1%"
    qrcode_background.style["ImGuiStyleVar_FrameRounding"] = "20px"
    qrcode_image = b.widgets.image()
    qrcode_image.src = main_window.context.qrcode_texture
    qrcode_image.left = "10%"
    qrcode_image.top = "10%"
    qrcode_image.right = "10%"
    qrcode_image.bottom = "10%"
    qrcode_container = b.widgets.container()
    qrcode_container.left = "0%"
    qrcode_container.width = "100%"

    under_qrcode_text_1 = b.widgets.text()
    under_qrcode_text_1.label = "Use the"
    under_qrcode_text_1.style["ImGuiCol_Text"] = "#808080"
    under_qrcode_text_1.left = "16%"

    under_qrcode_text_2 = b.widgets.text()
    under_qrcode_text_2.label = "Steam Mobile App"
    under_qrcode_text_2.style["ImGuiCol_Text"] = "#808080"
    under_qrcode_text_2.hyperlink = True;
    under_qrcode_text_2.underline = True;

    under_qrcode_text_3 = b.widgets.text()
    under_qrcode_text_3.label = "to sign"
    under_qrcode_text_3.style["ImGuiCol_Text"] = "#808080"

    under_qrcode_text_4 = b.widgets.text()
    under_qrcode_text_4.label = "in via QR code"
    under_qrcode_text_4.style["ImGuiCol_Text"] = "#808080"
    under_qrcode_text_4.width = "100%"
    under_qrcode_text_4.alignh = b.AlignH.Center

    help_text = b.widgets.text()
    help_text.label = "Help, I can't sign in"
    help_text.style["ImGuiCol_Text"] = "#808080"
    help_text.hyperlink = True;
    help_text.underline = True;

    create_account_text = b.widgets.text()
    create_account_text.label = "Create a Free Account"
    create_account_text.style["ImGuiCol_Text"] = "#808080"
    create_account_text.hyperlink = True;
    create_account_text.underline = True;
    create_account_text.right = "0"

    dont_have_account_text = b.widgets.text()
    dont_have_account_text.label = "Don't have a Steam account?"
    dont_have_account_text.style["ImGuiCol_Text"] = "#808080"

    def __init__(self):
        b.LoadFont("SteamText", "roboto-bold", 44)
        b.LoadFont("SteamTextTrademark", "roboto-bold", 15)
        b.LoadFont("InputLabel", "roboto-bold", 18)
        b.LoadFont("InputText", "roboto-regular", 20)
        b.LoadFont("LoginButton", "roboto-bold", 30)
        b.LoadFont("InfoText", "roboto-regular", 18)

    def render(self):

        def title_content_region():   # Render the title Steam logo and label
            self.steam_logo()
            b.Sameline()
            self.steam_text()
            b.Sameline()
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
            self.qrcode_container.height = self.qrcode_container.actual_size.x
            self.qrcode_container(qrcode)

            self.under_qrcode_text_1()
            b.Sameline()
            self.under_qrcode_text_2()
            b.Sameline()
            self.under_qrcode_text_3()
            self.under_qrcode_text_4()

            self.main_window.context.steam_hyperlink_clicked = self.under_qrcode_text_2.hyperlink_clicked

        def main_content_region():
            def upper_grid(call):
                call(0, 0, left_main_region)
                call(1, 0, right_main_region)
            self.upper_grid(upper_grid)

        def lower_content_region():
            self.help_text()
            b.Sameline()
            self.dont_have_account_text.right = self.create_account_text.actual_size.x + 10
            self.dont_have_account_text()
            b.Sameline()
            self.create_account_text()

            self.main_window.context.help_hyperlink_clicked = self.help_text.hyperlink_clicked
            self.main_window.context.create_account_hyperlink_clicked = self.create_account_text.hyperlink_clicked

        def main_vertical_grid(cell):
            cell(0, 0, title_content_region)
            cell(0, 1, main_content_region)
            cell(0, 2, lower_content_region)
        self.main_vertical_grid(main_vertical_grid)

    def __call__(self):
        self.main_window.width = b.MainWindowContext.window_size.x
        self.main_window.height = b.MainWindowContext.window_size.y
        self.main_window.context.titlebar_hovered = self.main_window.titlebar_hovered
        self.main_window.context.handle_window_dragging()
        if not self.main_window.is_open:
            b.StopApplication()
        self.main_window(self.render)

        # b.show_demo_window()

main_window = MainWindow()
main_window.main_window.context.init(main_window.__call__)