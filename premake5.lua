
-- Utility functions
function appendTable(tableA, tableB)
    for _,v in ipairs(tableB) do 
        table.insert(tableA, v) 
    end
end

function fileExists(name)
    local f=io.open(name,"r")
    if f~=nil then io.close(f) return true else return false end
end


-- Check if OpenSSL is available
if not fileExists("C:/Program Files/OpenSSL-Win64/include/openssl/err.h") then
    term.pushColor(term.red)
    printf("Warning: OpenSSL was not found! Disabling support for networking...")
    BATTERYENGINE_DISABLE_NETWORKING=true
    term.popColor()
end



-- Dependencies (git submodules)
include "modules/allegro5-binaries"

-- Main framework project
project "BatteryEngine"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"
    location "build"
    targetdir "bin/%{cfg.buildcfg}"
    targetname "%{prj.name}"
    
    -- Platforms
    filter { "platforms:x86" }
        architecture "x86"

    filter { "platforms:x64" }
        architecture "x86_64"
    
    -- Per-configuration defines
    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG", "NDEPLOY" }
        runtime "Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NDEBUG", "NDEPLOY" }
        runtime "Release"
        symbols "Off"
        optimize "On"

    filter "configurations:Deploy"
        defines { "NDEBUG", "DEPLOY" }
        runtime "Release"
        symbols "Off"
        optimize "On"

    filter {}

    -- Common defines
    defines { "ALLEGRO_STATICLINK", "SPDLOG_COMPILED_LIB" }
    if BATTERYENGINE_DISABLE_NETWORKING == true then
        defines { "BATTERYENGINE_DISABLE_NETWORKING" }
    end




    -- Include directories
    local _includedirs = { 
        _SCRIPT_DIR .. "/include", 
        _SCRIPT_DIR .. "/modules", 
        _SCRIPT_DIR .. "/modules/imgui",
        _SCRIPT_DIR .. "/modules/imgui/backends",
        _SCRIPT_DIR .. "/modules/implot",
        _SCRIPT_DIR .. "/modules/spdlog/include",
        _SCRIPT_DIR .. "/modules/serial/include",
        _SCRIPT_DIR .. "/modules/clip",
        _SCRIPT_DIR .. "/modules/allegro5-binaries/include",
        "C:/Program Files/OpenSSL-Win64/include/"
    }
    includedirs (_includedirs)




    -- Main source files
    files ({ "include/**", "src/**" })

    -- Serial library
    files ({ "modules/serial/src/serial.cc" })
    files ({ "modules/serial/src/impl/win.cc" })
    files ({ "modules/serial/src/impl/list_ports/list_ports_win.cc" })

    -- Clip library
    files ({ "modules/clip/clip.cpp" })
    files ({ "modules/clip/clip_win.cpp" })
    files ({ "modules/clip/image.cpp" })

    -- spdlog library
    files ({ "modules/spdlog/src/async.cpp" })
    files ({ "modules/spdlog/src/cfg.cpp" })
    files ({ "modules/spdlog/src/color_sinks.cpp" })
    files ({ "modules/spdlog/src/file_sinks.cpp" })
    files ({ "modules/spdlog/src/fmt.cpp" })
    files ({ "modules/spdlog/src/spdlog.cpp" })
    files ({ "modules/spdlog/src/stdout_sinks.cpp" })

    -- ImGui/ImPlot library
    files ({ "modules/imgui/imgui.cpp" })
    files ({ "modules/imgui/imgui_demo.cpp" })
    files ({ "modules/imgui/imgui_draw.cpp" })
    files ({ "modules/imgui/imgui_tables.cpp" })
    files ({ "modules/imgui/imgui_widgets.cpp" })
    files ({ "modules/imgui/backends/imgui_impl_allegro5.cpp" })
    files ({ "modules/implot/implot.cpp" })
    files ({ "modules/implot/implot_demo.cpp" })
    files ({ "modules/implot/implot_items.cpp" })



    -- Precompiled headers
    pchheader "Battery/pch.h"
    pchsource "src/pch.cpp"
    filter { "files:include/glm/detail/glm.cpp or files:modules/**" }
        flags { 'NoPCH' }
    filter { "files:include/asio/impl/src.cpp" }
        flags { 'ExcludeFromBuild' }
    filter {}



    -- Warning suppressions
    filter { "files:modules/clip/clip_win.cpp" }            -- Ignore signed/unsigned warning from 'clip' library 
        disablewarnings { "4018", "4267" }
    filter { "files:modules/serial/src/impl/win.cc" }       -- Ignore possible loss of data warning from 'serial' library 
        disablewarnings { "4244" }
    filter { "files:modules/serial/src/serial.cc" }         -- Ignore unreferenced local variable warning from 'serial' library 
        disablewarnings { "4101" }
    filter {}




    -- Include and linker information for premake projects using this library
    BATTERYENGINE_INCLUDE_DIRS = {}
    appendTable(BATTERYENGINE_INCLUDE_DIRS, _includedirs)
    appendTable(BATTERYENGINE_INCLUDE_DIRS, ALLEGRO5_INCLUDE_DIRS)

    BATTERYENGINE_LINK_DIRS = { _SCRIPT_DIR .. "/bin/%{cfg.buildcfg}/", "C:/Program Files/OpenSSL-Win64/lib/VC/static/" }
    appendTable(BATTERYENGINE_LINK_DIRS, ALLEGRO5_LINK_DIRS)

    BATTERYENGINE_LINKS = { "BatteryEngine", "opengl32", "winmm", "shlwapi" }
    appendTable(BATTERYENGINE_LINKS, ALLEGRO5_LINKS)
    if not BATTERYENGINE_DISABLE_NETWORKING == true then
        appendTable(BATTERYENGINE_LINKS, { "libcrypto64MTd", "libssl64MTd" } )
    end
