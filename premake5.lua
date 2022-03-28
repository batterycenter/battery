
-- Utility functions
function appendTable(tableA, tableB)
    for _,v in ipairs(tableB) do 
        table.insert(tableA, v) 
    end
end

newoption {
    trigger = "ENABLE_NETWORKING",
    value = "on",
    description = "Enable or disable ASIO TCP/UDP networking",
    allowed = {
       "on",
       "off"
    },
    default = "on"
}




-- Include the generated dependency paths from Conan
include "conanbuildinfo.premake5.lua"
conan_basic_setup()




-- Define the solution
workspace "Battery"
    configurations { "Debug", "Release", "Deploy" }

    platforms { "x64" }
    defaultplatform "x64"
    startproject "Battery"



-- Defines the static library project
project "Battery"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"
    location "build"
    targetdir "bin/%{cfg.buildcfg}"
    targetname "%{prj.name}"
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
        symbols "On"
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

    -- Clip library
    files ({ "modules/clip/clip.cpp" })
    files ({ "modules/clip/clip_win.cpp" })
    files ({ "modules/clip/image.cpp" })




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
    BATTERY_INCLUDE_DIRS = {}
    appendTable(BATTERY_INCLUDE_DIRS, _includedirs)
    appendTable(BATTERY_INCLUDE_DIRS, ALLEGRO5_INCLUDE_DIRS)

    BATTERY_LINK_DIRS = { _SCRIPT_DIR .. "/bin/%{cfg.buildcfg}/", "C:/Program Files/OpenSSL-Win64/lib/VC/static/" }
    appendTable(BATTERY_LINK_DIRS)

    BATTERY_LINKS = { "Battery" }
    BATTERY_SYSTEM_LINKS = { "opengl32", "winmm", "shlwapi" }
    appendTable(BATTERY_LINKS, ALLEGRO5_LINKS)
    if not BATTERY_DISABLE_NETWORKING == true then
        appendTable(BATTERY_LINKS, { "libcrypto64MTd", "libssl64MTd" } )
    end
