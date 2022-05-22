
#include "Battery/Renderer/Bitmap.h"
#include "Battery/Graphics.h"

#undef LoadBitmap

// TODO: Message when destructed too late, not just crash

namespace Battery {

	/*Bitmap::Bitmap() {}

	Bitmap::Bitmap(ALLEGRO_BITMAP* bitmap) {
		allegroBitmap.reset(bitmap);
	}

	Bitmap::Bitmap(const std::string& path, int flags) {
		LoadFromFile(path, flags);
	}

	Bitmap::Bitmap(const Bitmap& bitmap) {
		if (bitmap) {
			al_set_new_bitmap_samples(bitmap.GetSamples());
			al_set_new_bitmap_flags(bitmap.GetFlags());
			this->allegroBitmap.reset(al_clone_bitmap(bitmap.GetAllegroBitmap()));
		}
	}

	Bitmap::Bitmap(Bitmap&& bitmap) noexcept {
		if (bitmap.allegroBitmap) {
			allegroBitmap = std::move(bitmap.allegroBitmap);
		}
	}

	Bitmap::Bitmap(int width, int height, int flags) {
		CreateBitmap(width, height, flags);
	}

	Bitmap::Bitmap(const clip::image& image, int flags) {
		const clip::image_spec& spec = image.spec();

		if (!CreateBitmap(spec.width, spec.height, flags))
			throw Battery::Exception(std::string(__FUNCTION__) + ": Can't load image: Allegro Bitmap could not be created!");

		LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Loading Battery::Bitmap from clip::image now!");

		// Now load the image data
		ALLEGRO_BITMAP* oldBuffer = al_get_target_bitmap();
		al_set_target_bitmap(allegroBitmap.get());
		al_lock_bitmap(allegroBitmap.get(), al_get_bitmap_format(allegroBitmap.get()), ALLEGRO_LOCK_WRITEONLY);

		uint8_t* data = (uint8_t*)image.data();
		size_t row_offset = 0;

		for (unsigned long y = 0; y < spec.height; y++) {

			size_t byte_offset = 0;
			for (unsigned long x = 0; x < spec.width; x++) {

				uint32_t pixel = 0;
				memcpy(&pixel, data + row_offset + byte_offset, spec.bits_per_pixel / 8);

				uint32_t red = (pixel & spec.red_mask) >> spec.red_shift;
				uint32_t green = (pixel & spec.green_mask) >> spec.green_shift;
				uint32_t blue = (pixel & spec.blue_mask) >> spec.blue_shift;
				uint32_t alpha = (pixel & spec.alpha_mask) >> spec.alpha_shift;

				byte_offset += spec.bits_per_pixel / 8;

				if (spec.alpha_mask == 0) alpha = 255;

				al_put_pixel(x, y, { (float)red, (float)green, (float)blue, (float)alpha });
			}
			row_offset += spec.bytes_per_row;
		}

		al_unlock_bitmap(allegroBitmap.get());
		al_set_target_bitmap(oldBuffer);
	}

	Bitmap::~Bitmap() {
		if (allegroBitmap) {
			if (AllegroContext::GetInstance()->IsInitialized()) {
				allegroBitmap.reset();
			}
			else {
				LOG_CORE_CRITICAL("{}: {}", __FUNCTION__, "Can't unload texture: The Allegro Context is not loaded anymore!"
					" Make sure to unload the Bitmap object before Allegro is shut down!");
			}
		}
	}

	void Bitmap::operator=(const Bitmap& bitmap) {
		if (bitmap) {
			al_set_new_bitmap_samples(bitmap.GetSamples());
			al_set_new_bitmap_flags(bitmap.GetFlags());
			this->allegroBitmap.reset(al_clone_bitmap(bitmap.GetAllegroBitmap()));
		}
		else {
			allegroBitmap.reset();
		}
	}

	void Bitmap::operator=(Bitmap&& bitmap) noexcept {
		if (bitmap.allegroBitmap) {
			allegroBitmap = std::move(bitmap.allegroBitmap);
		}
	}




	bool Bitmap::LoadFromFile(const std::string& path, int flags) {

		// Now load the new texture
		al_set_new_bitmap_flags(flags);
		allegroBitmap.reset(al_load_bitmap(path.c_str()));

		if (!allegroBitmap) {
			LOG_CORE_ERROR("Failed to load Allegro bitmap: '" + path + "'");
			return false;
		}

		return true;
	}

	bool Bitmap::SaveToFile(const std::string& file) const {

		if (!allegroBitmap) {
			return false;
		}

		return al_save_bitmap(file.c_str(), allegroBitmap.get());
	}

	bool Bitmap::SetSamples(int samples) {

		if (allegroBitmap) {
			al_set_new_bitmap_samples(samples);
			allegroBitmap.reset(al_clone_bitmap(allegroBitmap.get()));
		}

		return (bool)allegroBitmap;
	}

	int Bitmap::GetSamples() const {
		if (allegroBitmap) {
			return al_get_bitmap_samples(allegroBitmap.get());
		}

		return 0;
	}

	bool Bitmap::SetFlags(int flags) {

		if (allegroBitmap) {
			al_set_new_bitmap_flags(flags);
			allegroBitmap.reset(al_clone_bitmap(allegroBitmap.get()));
		}

		return (bool)allegroBitmap;
	}

	int Bitmap::GetFlags() const {
		if (allegroBitmap) {
			return al_get_bitmap_flags(allegroBitmap.get());
		}

		return 0;
	}

	int Bitmap::GetWidth() const {

		if (!allegroBitmap)
			return 0;

		return al_get_bitmap_width(allegroBitmap.get());
	}

	int Bitmap::GetHeight() const {

		if (!allegroBitmap)
			return 0;

		return al_get_bitmap_height(allegroBitmap.get());
	}

	glm::ivec2 Bitmap::GetSize() const {

		if (!allegroBitmap)
			return { 0, 0 };

		return { GetWidth(), GetHeight() };
	}





	std::optional<std::pair<std::vector<uint32_t>, clip::image_spec>> Bitmap::GetClipImage() const {

		struct ByteColor {
			uint8_t r, g, b, a;
			ByteColor(ALLEGRO_COLOR color) {
				r = color.r * 255;
				g = color.g * 255;
				b = color.b * 255;
				a = color.a * 255;
			}

			uint32_t getHex() {
				return  (uint32_t)a << 24 | (uint32_t)b << 16 |
						(uint32_t)g << 8 | (uint32_t)r;
			}
		};

		if (!allegroBitmap)
			return std::nullopt;

		LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Generating clip::image now!");

		uint64_t width = GetWidth();
		uint64_t height = GetHeight();

		// Create an image with 8-bit RGBA values (32 bit per pixel)
		std::vector<uint32_t> data(width * height, 0);

		// Now load the image data
		ALLEGRO_BITMAP* oldBuffer = al_get_target_bitmap();
		al_set_target_bitmap(allegroBitmap.get());
		al_lock_bitmap(allegroBitmap.get(), al_get_bitmap_format(allegroBitmap.get()), ALLEGRO_LOCK_READONLY);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				ByteColor color = al_get_pixel(allegroBitmap.get(), x, y);
				data[y * width + x] = color.getHex();
			}
		}

		al_unlock_bitmap(allegroBitmap.get());
		al_set_target_bitmap(oldBuffer);

		// Now setup the image specs
		clip::image_spec spec;
		spec.width = width;
		spec.height = height;
		spec.bits_per_pixel = 32;
		spec.bytes_per_row = width * 4;
		spec.red_mask		= 0x000000FF;
		spec.green_mask		= 0x0000FF00;
		spec.blue_mask		= 0x00FF0000;
		spec.alpha_mask		= 0xFF000000;
		spec.red_shift		= 0;
		spec.green_shift	= 8;
		spec.blue_shift		= 16;
		spec.alpha_shift	= 24;

		return std::make_optional<std::pair<std::vector<uint32_t>, clip::image_spec>>(std::make_pair(std::move(data), spec));
	}

	bool Bitmap::LoadEmbeddedResource(int id) {
		// TODO !!!!!!!!!!!!!!!!!!!!!!!
		throw Battery::NotImplementedException();
	}






	ALLEGRO_BITMAP* Bitmap::GetAllegroBitmap() const {
		if (!allegroBitmap)
			throw Battery::Exception(std::string(__FUNCTION__) + ": Cannot return Allegro Bitmap pointer: allegroBitmap is null!");
		
		return allegroBitmap.get();
	}

	bool Bitmap::CreateBitmap(int width, int height, int flags) {

		al_set_new_bitmap_flags(flags);
		allegroBitmap.reset(al_create_bitmap(width, height));

		return allegroBitmap;
	}

	void Bitmap::Reset() {
		if (!AllegroContext::GetInstance()->IsInitialized()) {
			throw Battery::Exception(std::string(__FUNCTION__) + ": Can't destroy texture: The Allegro Context is not loaded anymore!"
				" Make sure to unload or destroy the Bitmap object before Allegro is shut down!");
		}

		if (allegroBitmap) {
			allegroBitmap.reset();
		}
	}*/

}
