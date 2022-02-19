#pragma once

#include "glm/glm.hpp"
#include "Battery/AllegroDeps.h"
#include "Battery/Graphics.h"

namespace Battery {
	
	struct Vertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 color;

		Vertex(glm::vec3 _position, glm::vec2 _uv, glm::vec3 _color) {
			position = _position;
			uv = _uv;
			color = _color;
		}

		operator ALLEGRO_VERTEX() const {
			ALLEGRO_VERTEX v;
			v.x = position.x;
			v.y = position.y;
			v.z = position.z;
			v.u = uv.x;
			v.v = uv.y;
			v.color = Battery::Graphics::ConvertAllegroColor(color);
			return v;
		}
	};

}
