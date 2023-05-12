import widgets

button = widgets.button()
button.style["border-radius"] = "5px"

fancy_button = widgets.fancy_button()
fancy_button.style["button-gradient-color-left"] = "#06bfff"
fancy_button.style["button-gradient-color-right"] = "#2d73ff"

text = widgets.text()
text.label = "Click me!"
text.style["text-color"] = "#06bfff"

input = widgets.input("##")
input.hint = "Enter your username"
input.style["border-radius"] = "10px"
input.style["text-color"] = "#06bfff"
input.style["text-color-disabled"] = "#aaaaaa"
input.style["input-color"] = "#32353c"

def ui_loop():
    button()
    widgets.vspace(50)
    fancy_button()
    widgets.vspace(50)
    text()
    widgets.vspace(50)
    input()
    widgets.vspace(50)
    widgets.render_image()
    #print(input.active)

widgets.init_ui(ui_loop)