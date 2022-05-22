
#include "Battery/Graphics.h"
/*
namespace Battery {
	namespace Graphics {


		// =============================================================
		// These functions are only available for within this .cpp file!
		// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

		ALLEGRO_COLOR ConvertAllegroColor(glm::vec3 color) {
			return al_map_rgb(color.r, color.g, color.b);
		}

		ALLEGRO_COLOR ConvertAllegroColor(glm::vec4 color) {
			return al_map_rgba(color.r, color.g, color.b, color.a);
		}

		glm::vec4 ConvertAllegroColor(ALLEGRO_COLOR color) {
			unsigned char r, g, b, a;
			al_unmap_rgba(color, &r, &g, &b, &a);
			return glm::vec4(r, g, b, a);
		}
		// =====================================================




		glm::vec3 __fillcolor = { 255, 255, 255 };
		glm::vec3 __strokecolor = { 0, 0, 0 };
		double __thickness = 3;
		bool __fillenabled = true;
		bool __strokeenabled = true;
		enum LINECAP __linecap = LINECAP::ROUND;
		enum LINEJOIN __linejoin = LINEJOIN::ROUND;

		void DrawBackground(glm::vec3 color) {
			al_clear_to_color(ConvertAllegroColor(color));
		}

		void DrawBackground(glm::vec4 color) {
			al_clear_to_color(ConvertAllegroColor(color));
		}






		void Fill(glm::vec3 color) {
			__fillcolor = color;
			__fillenabled = true;
		}

		void Stroke(glm::vec3 color, double thickness) {
			__strokecolor = color;
			__thickness = thickness;
			__strokeenabled = true;
		}

		void NoFill() {
			__fillenabled = false;
		}

		void NoStroke() {
			__strokeenabled = false;
		}

		void UseLineCap(enum LINECAP linecap) {
			__linecap = linecap;
		}

		void UseLineJoin(enum LINEJOIN linejoin) {
			__linejoin = linejoin;
		}

		void DrawLine(glm::vec2 p1, glm::vec2 p2) {
			if (__strokeenabled) {
				al_draw_line(p1.x, p1.y, p2.x, p2.y, ConvertAllegroColor(__strokecolor), static_cast<float>(__thickness));
			}
		}

		void DrawTriangle(glm::vec2 p1, glm::vec2 p2, double thickness, glm::vec3 color) {
			al_draw_line(p1.x, p1.y, p2.x, p2.y, ConvertAllegroColor(color), static_cast<float>(thickness));
		}

		void DrawFilledTriangle(glm::vec2 p1, glm::vec2 p2, double thickness, glm::vec3 color) {
			al_draw_line(p1.x, p1.y, p2.x, p2.y, ConvertAllegroColor(color), static_cast<float>(thickness));
		}

		void DrawRectangle(glm::vec2 p1, glm::vec2 p2, double thickness, glm::vec3 color) {
			al_draw_line(p1.x, p1.y, p2.x, p2.y, ConvertAllegroColor(color), static_cast<float>(thickness));
		}

		void DrawFilledRectangle(glm::vec2 p1, glm::vec2 p2, double thickness, glm::vec3 color) {
			al_draw_line(p1.x, p1.y, p2.x, p2.y, ConvertAllegroColor(color), static_cast<float>(thickness));
		}

		void DrawRoundedRectangle(glm::vec2 p1, glm::vec2 p2, double thickness, glm::vec3 color) {
			al_draw_line(p1.x, p1.y, p2.x, p2.y, ConvertAllegroColor(color), static_cast<float>(thickness));
		}

		void DrawFilledRoundedRectangle(glm::vec2 p1, glm::vec2 p2, double thickness, glm::vec3 color) {
			al_draw_line(p1.x, p1.y, p2.x, p2.y, ConvertAllegroColor(color), static_cast<float>(thickness));
		}

		void DrawCircle(glm::vec2 p1, glm::vec2 p2, double thickness, glm::vec3 color) {
			al_draw_line(p1.x, p1.y, p2.x, p2.y, ConvertAllegroColor(color), static_cast<float>(thickness));
		}

		void DrawFilledCircle(glm::vec2 p1, glm::vec2 p2, double thickness, glm::vec3 color) {
			al_draw_line(p1.x, p1.y, p2.x, p2.y, ConvertAllegroColor(color), static_cast<float>(thickness));
		}

	}
}
*/