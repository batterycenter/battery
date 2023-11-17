
#include <gtest/gtest.h>
#include "battery/main.hpp"
#include "battery/log.hpp"

#include <battery/python/python.hpp>
#include <pybind11/stl.h>   // We need this to be able to send and receive STL containers like std::vector from python

int add_numbers(int a, int b) {
    return a + b;
}

std::vector<b::string> reverse_array(const std::vector<b::string>& array) {
    std::vector<b::string> reversed_array(array);
    std::reverse(reversed_array.begin(), reversed_array.end());
    return reversed_array;
}

PYBIND11_EMBEDDED_MODULE(python_example, m) {   // This packs the C++ functions into a python module which can be imported
    // Variables
    m.attr("cpp_number") = 187;
    m.attr("cpp_string") = "Hello World";

    // Functions
    m.def("add_numbers", &add_numbers);
    m.def("reverse_array", &reverse_array);
}

TEST(BatteryPython, Python) {
    b::py::scoped_interpreter guard{};
    try {
        b::py::exec("import python_example");
        EXPECT_EQ(b::py::eval("python_example.cpp_number").cast<int>(), 187);
        EXPECT_EQ(b::py::eval("python_example.cpp_string").cast<b::string>(), b::string("Hello World"));
        EXPECT_EQ(b::py::eval("python_example.add_numbers(1, 2)").cast<int>(), 3);
        EXPECT_EQ(b::py::eval("python_example.reverse_array(['a', 'b', 'c'])").cast<std::vector<b::string>>(), std::vector<b::string>({"c", "b", "a"}));
    } catch (const std::exception& e) {               
        b::log::error("Python error: {}", e.what());
    }
}
