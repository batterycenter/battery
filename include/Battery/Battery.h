#pragma once

#include "Battery/pch.h"

#include "Battery/Core.h"
#include "Battery/StringUtils.h"
#include "Battery/Graphics.h"

#include "Battery/Core/Application.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Core/Window.h"
#include "Battery/Core/ImGuiLayer.h"
#include "Battery/Core/ImGuiPanel.h"
#include "Battery/Core/Event.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/Utils/FileUtils.h"
#include "Battery/Utils/MathUtils.h"
#include "Battery/Utils/NetUtils.h"
#include "Battery/Utils/ImGuiUtils.h"
#include "Battery/Platform/Dialog.h"
#include "Battery/Log/Log.h"
#include "Battery/Renderer/Renderer2D.h"
#include "Battery/Renderer/Bitmap.h"
#include "Battery/Renderer/ShaderProgram.h"

// Serial library
#include "serial/serial.h"

#pragma warning( disable : 4244 )	// Always suppress warning about possible loss of data
