#pragma once

#include "Battery/pch.hpp"
#include "Battery/Core/Log.h"

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
