#pragma once

#include "Battery/pch.h"

#include "Battery/AllegroDeps.h"
#include "Battery/Core/Config.h"
//#include "Battery/ShaderProgram.h"
#include "Battery/RenderUtils.h"
#include "Battery/DefaultShaders.h"
#include "glm/glm.hpp"

namespace Battery {

	class PrimitiveRenderer {

		ALLEGRO_DISPLAY** display;

		ShaderProgram lineShader;
		//std::vector<Battery::Vertex> lineVertices;
		//std::vector<int> lineIndices;

	public:
		PrimitiveRenderer(ALLEGRO_DISPLAY** display) {
			this->display = display;
		}

		void Load() {
			if (!lineShader.Loaded()) {
				if (!lineShader.LoadSource(*display, BATTERY_SHADER_SOURCE_VERTEX_LINE_ANTIALIASED, BATTERY_SHADER_SOURCE_FRAGMENT_LINE_ANTIALIASED)) {
					throw Battery::Exception("[PrimitiveRenderer::AddLine]: ERROR: Line Shader failed to load!");
				}
			}
		}

		void Unload() {
			lineShader.Unload();
		}

		void DrawLine(glm::vec2 p1, glm::vec2 p2, float thickness, glm::vec3 color) {
			DrawLine(p1, p2, thickness, color, color);
		}

		void DrawLine(glm::vec2 pa, glm::vec2 pb, float thickness, glm::vec3 color1, glm::vec3 color2) {
			using namespace Battery::Graphics;

			glm::vec2 atob = glm::normalize(pb - pa);
			glm::vec2 anorm = glm::vec2(atob.y, -atob.x);

			float r = thickness / 2;

			glm::vec2 p1 = pa - atob * r + anorm * r;
			glm::vec2 p2 = pa - atob * r - anorm * r;
			glm::vec2 p3 = pb + atob * r - anorm * r;
			glm::vec2 p4 = pb + atob * r + anorm * r;

			ALLEGRO_VERTEX v[] = {
				Vertex({ p1, 0 }, {0, 0}, color1),
				Vertex({ p2, 0 }, {0, 0}, color1),
				Vertex({ p3, 0 }, {0, 0}, color2),
				Vertex({ p4, 0 }, {0, 0}, color2)
			};

			int indices[] = {
				0, 1, 2,
				0, 2, 3
			};

			if (!lineShader.Loaded()) {
				throw Battery::Exception("[PrimitiveRenderer::AddLine]: ERROR: Renderer shader was not loaded!");
			}
			lineShader.Use();

			lineShader.SetUniformFloat("lineP1", pa);
			lineShader.SetUniformFloat("lineP2", pb);
			lineShader.SetUniformFloat("lineThickness", thickness - BATTERY_ANTIALIASING_LINE_FALLOFF);
			lineShader.SetUniformFloat("lineOutline", thickness);

			al_draw_indexed_prim(v, NULL, NULL, indices, 6, ALLEGRO_PRIM_TRIANGLE_LIST);

			lineShader.reset();
		}

	};

}
