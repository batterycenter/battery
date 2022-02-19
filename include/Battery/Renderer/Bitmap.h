#pragma once

#include "Battery/Renderer/AllegroBitmap.h"
#include "Battery/Log/Log.h"
#include "Battery/Utils/TypeUtils.h"
#include "clip.h"

#ifdef LoadBitmap
#undef LoadBitmap
#endif

namespace Battery {

	class Bitmap {
	public:
		Bitmap();
		Bitmap(ALLEGRO_BITMAP* bitmap);
		Bitmap(const std::string& path, int flags = 0);
		Bitmap(const Bitmap& bitmap);
		Bitmap(Bitmap&& bitmap) noexcept;
		Bitmap(int width, int height, int flags = 0);
		Bitmap(const clip::image& image, int flags = 0);
		~Bitmap();

		void operator=(const Bitmap& bitmap);
		void operator=(Bitmap&& bitmap) noexcept;

		bool LoadFromFile(const std::string& path, int flags = 0);
		bool SaveToFile(const std::string& file) const;
		bool SetSamples(int samples);
		int GetSamples() const;
		bool SetFlags(int flags);
		int GetFlags() const;
		int GetWidth() const;
		int GetHeight() const;
		glm::ivec2 GetSize() const;

		std::optional<std::pair<std::vector<uint32_t>, clip::image_spec>> GetClipImage() const;
		bool LoadEmbeddedResource(int id);

		ALLEGRO_BITMAP* GetAllegroBitmap() const;
		bool CreateBitmap(int width, int height, int flags = 0);
		void Reset();

		operator bool() const {
			return (bool)allegroBitmap;
		}

	private:
		AllegroBitmap allegroBitmap;
	};
}
