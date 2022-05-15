from conans import ConanFile, CMake, tools

class BatteryConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    
    requires = \
        "serial/1.2.1", \
        "asio/1.22.1", \
        "spdlog/1.10.0", \
        "allegro5/5.2.7", \
        "glm/0.9.9.8", \
        "nlohmann_json/3.10.5", \
        "imgui/1.87", \
        "implot/0.13", \
        "zlib/1.2.12"
    generators = "cmake_find_package"

    def generate(self):
        pass

    def add_find_package_case(self, file_name, name, toNames):
        appendix = ""
        
        if not isinstance(toNames, list):
            toNames = [toNames]
            
        for toName in toNames:

            # Replace all occurrences in reverse, if generated file is inconsistent
            tools.replace_in_file(file_name, toName, name)

            # Append key
            for key in ["FOUND", "INCLUDE_DIRS", "INCLUDES",
                        "DEFINITIONS", "LIBRARIES", "LIBRARIES_TARGETS",
                        "LIBS", "LIBRARY_LIST", "LIB_DIRS"]:
                appendix = appendix + "set(" + toName + "_" + key + " ${" + name + "_" + key + "})\n"
        
        with open(file_name, 'r') as f:
            content = f.read()

        content = content + "\n\n" + appendix
        
        with open(file_name, "w") as handle:
            handle.write(content)
        
        # For case-sensitive file-systems, keep all known casings available
        for toName in toNames:
            try:
                shutil.copy(file_name, os.path.join(os.path.dirname(file_name), "Find" + toName + ".cmake"))
            except:
                pass