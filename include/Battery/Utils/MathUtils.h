#pragma once

#include "Battery/pch.h"

namespace Battery {
	namespace MathUtils {

		float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);

		glm::vec2 MapVector(glm::vec2 x, glm::vec2 in_min, glm::vec2 in_max, glm::vec2 out_min, glm::vec2 out_max);

	}
}
