#pragma once

#include "Battery/pch.h"

#include "Battery/Core/Log.h"
#include "Battery/Core/Application.h"
#include "Battery/Core/ImGuiPanel.h"
#include "Battery/Core/Exception.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/Utils/FileUtils.h"
#include "Battery/Utils/MathUtils.h"
#include "Battery/Utils/ImGuiUtils.h"
#include "Battery/Utils/StringUtils.h"

#include "SFML/Main.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Window.hpp"

#include "imgui-SFML.h"

#pragma warning( disable : 4244 )	// Always suppress warning about possible loss of data
