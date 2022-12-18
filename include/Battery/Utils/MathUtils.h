#pragma once

namespace Battery {

	template<typename T>
	T Map(T x, T in_min, T in_max, T out_min, T out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	static glm::vec2 MapVec2(glm::vec2 x, glm::vec2 in_min, glm::vec2 in_max, glm::vec2 out_min, glm::vec2 out_max) {
		return { Map(x.x, in_min.x, in_max.x, out_min.x, out_max.x), 
				 Map(x.y, in_min.y, in_max.y, out_min.y, out_max.y) };
	}

}
