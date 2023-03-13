
namespace battery {
    int run_main(int, const char**);
}

#ifndef BATTERY_CORE_NO_MAIN
int main(int argc, const char** argv) {     // This is the global entry point, the program starts here
    return battery::run_main(argc, argv);
}
#endif