
#include "Battery/pch.h"
#include "Battery/Utils/MathUtils.h"

namespace Battery {
	namespace MathUtils {

		float MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}

		glm::vec2 MapVector(glm::vec2 x, glm::vec2 in_min, glm::vec2 in_max, glm::vec2 out_min, glm::vec2 out_max) {
			return { MapFloat(x.x, in_min.x, in_max.x, out_min.x, out_max.x), 
					 MapFloat(x.y, in_min.y, in_max.y, out_min.y, out_max.y) };
		}

	}
}
