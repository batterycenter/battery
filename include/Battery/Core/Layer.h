#pragma once

#include "Battery/common.h"
#include "Battery/Core/log.hpp"

namespace Battery {

	class Layer {
	public:
		Layer() {}
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnEvent(sf::Event e, bool& handled) {}
	};

}
