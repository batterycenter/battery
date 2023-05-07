
#include "battery/graphics/styles.hpp"

namespace b {

    void push_default_style(widget_style& style) {

        // Button widget
        style["button-background-color"] = "#222222";
        style["button-active-color"] = "#FF0000";
        style["button-hover-color"] = "#229922";
        style["button-border-radius"] = "20px";
        style["button-border-width"] = "1px";
        style["button-border-color"] = "#00FF00";
        style["button-padding"] = ImVec2(50, 50);
        style["button-width"] = "0px";
        style["button-height"] = "0px";

    }

}