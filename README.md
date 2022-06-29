# BatteryEngine

https://www.flaticon.com/free-icon/battery_3165660?term=battery&page=1&position=31&page=1&position=31&related_id=3165660&origin=search

This is a simple framework for desktop applications or small games, based on Allegro5. It allows low-level programming with direct access to OpenGL.
 - Very easy to set up
 - Hardware-accelerated primitive shape rendering
 - Application layers for well-structured projects
 - Easy access to the clipboard for implementing copy/paste
 - ImGui immediate-mode user interface for top performance GUIs
 - Serial module for interfacing with Arduino
 - Fancy command line logging based on spdlog
 - ASIO Networking for direct UDP access
 - Windows 64-bit only

## Requirements

 - [git](https://git-scm.com/downloads) - For cloning the repository
 - [Microsoft Visual Studio 2019](https://visualstudio.microsoft.com/de/downloads/) - Other IDEs can be used by editing `generate-win.bat`
 - [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html) - Direct [download link for Windows-64bit](https://slproweb.com/download/Win64OpenSSL-1_1_1L.exe)
   - Make sure to install to `C:\Program Files\OpenSSL-Win64` (default path) and
   - Install to /bin of the folder (NOT DEFAULT!)

## How to use

### Linux

Install all dependencies:
```
sudo apt install git cmake build-essential libx11-dev
```
