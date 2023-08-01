# Battery

<img src="assets/battery.png" width="30%" align="right">

<a href="https://www.flaticon.com/free-icon/battery_3165660" title="battery icons">Battery icon is provided by Freepik - Flaticon</a>  
Library Documentation WIP: https://batterycenter.github.io/battery/

# Disclaimer

This library is currently under heavy development and is not ready for production use whatsoever. Everything is subject to change.

`include/Battery` and `src/Battery` are subject to be removed entirely.

# The Pitch

Battery is a modern C++ library that aims to provide a simple, yet powerful API for creating cross-platform desktop applications. It tries to hold your hand all the way through your process of developing a new open-source desktop app, from project setup over building, all the way to releasing it on GitHub Releases, including One-Click installers and automatic updates.

It fills a lot of gaps in the standard library like guaranteed UTF-8 support on every supported platform, or useful utilities like cache files, or the ability to embed a resource file directly into your executable at compile time.

The frontend is built on top of Dear ImGui, but it offers a very good default look and many different default themes, as well as the ability to easily create complex custom Widgets using ImGui. The UI loop can either be written in C++ directly, or in Python, which allows live-reloading on your ImGui UI.

Battery aims to be the next big thing in the world of desktop applications after Qt has entered the market. It tries to breath fresh air into the desktop application market, allowing you to have a modern, beautiful, cross-platform application like with Electron.js, less than 10Mb hello world filesize, the native performance of C++, customizability like in Qt, but not gigabytes of dependencies like Qt, the simplicity of one-click installers, and automatic updates like in VS-Code.

Let's make the world a better place by writing software that is fast and just works ❤️

# License

Battery is licensed under the Apache License 2.0.  
You ARE allowed to:
 - Use the library and all provided tools for any purpose personally or commercially ✅
 - Modify them, as long as the license remains unchanged and all modifications are clearly marked ✅
 - Distribute any software that depends upon battery as a dependency ✅
 - Redistribute battery in source or binary form as part of another project, while retaining the license ✅
 - Use battery for developing closed source applications commercially ✅

But you are NOT allowed to:
 - Redistribute the battery library itself or large portions of it, claiming you are the original author ❌
 - Sell the code without major modifications, with the goal of making money off of my work ❌
 - Battery is distributed without any warranty whatsoever ❌

Battery is striving to become a healthy community of developers. Everything is allowed that contributes
to the goal of supporting teams in developing and distributing modern, free software.  
We love open source solutions that just work ❤️ 

# WIP stuff

```
sudo apt install git cmake build-essential libx11-dev
```

# Embedded dependencies

https://github.com/gabime/spdlog -> MIT License
https://github.com/DaanDeMeyer/reproc -> MIT License
https://github.com/nemtrif/utfcpp -> Boost Software License 1.0
https://github.com/jothepro/doxygen-awesome-css -> MIT License

# Other stuff

https://google.github.io/styleguide/cppguide.html

https://github.com/TartanLlama/expected -> CC0 1.0 License

https://github.com/zserge/tray -> modified for use with modern C++

https://github.com/CLIUtils/CLI11 -> BSD-Clause, modified to use Battery::FS::ifstream instead of std::ifstream

https://github.com/agauniyal/rang -> Unlicensed

https://github.com/Neargye/semver -> MIT License

https://github.com/cpm-cmake/CPM.cmake -> MIT License

https://github.com/Soundux/traypp -> MIT License

https://www.reichwein.it/unicode/ -> CC0 1.0 Universal (Like MIT)

https://github.com/JuliaStrings/utf8proc -> MIT "expat"

https://github.com/reunanen/cpp-exclusive-lock-file -> MIT License

TODO: Add unit testing

TODO: Add pipelines and automate testing

TODO: Fix build system and the dependency problem (what includes what and what the examples need)

TODO: Setup examples so that they have their own copy of battery

TODO: Properly implement install scripts for installing the CMake library and the resulting binaries

TODO: Check support for network drives (filesystem)

TODO: Allow battery_embed() CMake command take absolute paths

# Inspiration

![Battery1](assets/Battery1.png)