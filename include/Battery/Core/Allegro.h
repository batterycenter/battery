#pragma once

#include "Battery/Core/Config.h"

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 5054)
#endif

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_memfile.h>
#include <allegro5/allegro_native_dialog.h>

#ifdef _WIN32
#include <allegro5/allegro_windows.h>
#endif

#ifdef _MSC_VER
#  pragma warning(pop)
#endif

#ifdef RegisterClipboardFormat
#undef RegisterClipboardFormat
#endif

#ifdef RemoveDirectory
#undef RemoveDirectory
#endif

#ifdef MoveFile
#undef MoveFile
#endif
