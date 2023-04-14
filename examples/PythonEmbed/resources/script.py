import sys

import python_example   # This module was defined in C++

print("Hello from Python! This is Python v" + sys.version)

print("\nValue of the C++ variable 'cpp_number' = " + str(python_example.cpp_number))
print("Value of the C++ variable 'cpp_string' = \"" + str(python_example.cpp_string) + "\"")

print("\nCalling 'add_numbers', a C++ function, from Python")
print("Result: python_example.add_numbers(1, 2) = " + str(python_example.add_numbers(1, 2)))

print("\nCalling 'reverse_array', a C++ function, from Python")
print("Result: python_example.reverse_array([\"One\", \"Two\", \"Three\", \"Four\"]) = " + str(python_example.reverse_array(["One", "Two", "Three", "Four"])))

def callback(i):
    print("Python function called from C++ with value: " + str(i))

print("\nSending python function to C++")
python_example.set_callback(callback)

print("\nPython script is finished\n")