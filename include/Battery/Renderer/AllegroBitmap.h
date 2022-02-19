#pragma once

struct ALLEGRO_BITMAP;

namespace Battery {

	class AllegroBitmap {
	public:
		AllegroBitmap() {}
		AllegroBitmap(ALLEGRO_BITMAP* bitmap) : bitmap(bitmap) {}
		AllegroBitmap(AllegroBitmap&& bitmap) noexcept;
		~AllegroBitmap();

		void operator=(AllegroBitmap&& bitmap);

		ALLEGRO_BITMAP* get() const {
			return bitmap;
		}

		ALLEGRO_BITMAP* operator->() const {
			return get();
		}

		operator bool() const {
			return (bool)bitmap;
		}

		void reset();

		void reset(ALLEGRO_BITMAP* bitmap) {
			reset();

			this->bitmap = bitmap;
		}

		void reset(const AllegroBitmap& bitmap) {
			reset(bitmap.bitmap);
		}

		AllegroBitmap(const AllegroBitmap& bitmap) = delete;
		void operator=(const AllegroBitmap& bitmap) = delete;

	private:
		ALLEGRO_BITMAP* bitmap = nullptr;
	};

}
