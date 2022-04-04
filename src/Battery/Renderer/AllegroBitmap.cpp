
#include "Battery/pch.h"
#include "Battery/Renderer/AllegroBitmap.h"
#include "Battery/Core/AllegroContext.h"

namespace Battery {

	AllegroBitmap::AllegroBitmap(AllegroBitmap&& bitmap) noexcept {
		if (this->bitmap) {
			al_destroy_bitmap(this->bitmap);
		}
		this->bitmap = bitmap.bitmap;
		bitmap.bitmap = nullptr;
	}

	AllegroBitmap::~AllegroBitmap() {
		reset();
	}

	void AllegroBitmap::operator=(AllegroBitmap&& bitmap) {
		if (this->bitmap) {
			al_destroy_bitmap(this->bitmap);
		}
		this->bitmap = bitmap.bitmap;
		bitmap.bitmap = nullptr;
	}

	void AllegroBitmap::reset() {
		if (bitmap) {
			al_destroy_bitmap(bitmap);
			bitmap = nullptr;
		}
	}

}
