#pragma once

#include "Battery/pch.h"
#include "Battery/Core/Log.h"
//#include "Battery/Core/ApplicationEvents.h"

namespace Battery {

	// Forward declaration
	class Application;
	class Event;


	class Layer {

		std::string layerName;

	public:
		Layer() {
			layerName = "Unnamed layer";
			LOG_CORE_TRACE("Layer 'Unnamed layer' constructed");
		}

		Layer(std::string debugName) {
			this->layerName = debugName;
			LOG_CORE_TRACE("Layer '{}' constructed", this->layerName);
		}

		virtual ~Layer() {
			LOG_CORE_TRACE("Destructing Layer '{}': OnDetach()", this->layerName);
		}

		virtual void OnAttach() {}

		virtual void OnDetach() {}

		virtual void OnUpdate() {}

		virtual void OnRender() {}

		virtual void OnEvent(sf::Event e, bool& handled) {}

		const std::string& GetDebugName() const {
			return layerName;
		}
	};

}
