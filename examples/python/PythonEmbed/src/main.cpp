
#include "battery/main.hpp"
#include "battery/log.hpp"
#include <battery/python/python.hpp>

#include "resources/script_py.hpp"

#include <pybind11/stl.h>   // We need this to be able to send and receive STL containers like std::vector from python

static b::py::function py_callback;

int add_numbers(int a, int b) {
    b::log::debug("Adding {} and {} in C++ ...", a, b);
    return a + b;
}

std::vector<b::string> reverse_array(const std::vector<b::string>& array) {
    b::log::debug("Reversing string array in C++ ...");
    std::vector<b::string> reversed_array(array);
    std::reverse(reversed_array.begin(), reversed_array.end());
    return reversed_array;
}

void set_callback(const b::py::function& callback) {
    b::log::debug("Capturing callback function in C++ ...");
    py_callback = callback;
}

PYBIND11_EMBEDDED_MODULE(python_example, m) {   // This packs the C++ functions into a python module which can be imported
    // Variables
    m.attr("cpp_number") = 187;
    m.attr("cpp_string") = "Hello World";

    // Functions
    m.def("add_numbers", &add_numbers);
    m.def("reverse_array", &reverse_array);
    m.def("set_callback", &set_callback);
}

int b::main([[maybe_unused]] const std::vector<b::string>& args) {

    b::log::info("Running Example PythonEmbed. Have fun!");

    b::py::scoped_interpreter guard{};
    try {
        b::log::debug("Invoking Python script from C++");
        b::py::exec(resources::script_py.str());        // The file 'resources/script.py' is embedded into the binary and then
                                                        // executed by the embedded python interpreter here.
        
        b::log::debug("Python script finished. Now, calling the Python callback function from C++ 5 times...");
        for (int i = 0; i < 5; i++) {
            py_callback(i);
        }

        b::print("\n");
        int number = b::py::eval("5**3").cast<int>();
        b::log::debug("5**3 = {}", number);

        b::print("\n");
        std::vector<b::string> array = b::py::eval("['a', 'b', 'c']").cast<std::vector<b::string>>();
        b::log::debug("['a', 'b', 'c'] in python is in C++:");
        for (size_t i = 0; i < array.size(); i++) {
            b::log::debug("  [{}] = {}", i, array[i]);
        }

    } catch (const std::exception& e) {               
        b::log::error("Python error: {}", e.what());
    }

    b::print("\n");
    b::log::info("Example PythonEmbed finished. Please take a look at how it is implemented. Happy coding!");

    return 0;
}
