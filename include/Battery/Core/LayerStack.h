#pragma once

#include "Battery/pch.hpp"
#include "Battery/Core/Log.h"
#include "Battery/Core/Layer.h"

namespace Battery {

	class LayerStack {

		std::vector<std::shared_ptr<Layer>> layers;
		size_t layerNum = 0;

	public:
		LayerStack() {
			
		}

		~LayerStack() {
			ClearStack();
		}

		void PushLayer(std::shared_ptr<Layer> layer) {
			LOG_CORE_TRACE("Pushing Layer onto Layer Stack");
			auto it = layers.insert(layers.begin() + layerNum, std::move(layer));
			(*it)->OnAttach();
			layerNum++;
		}
		
		void PushOverlay(std::shared_ptr<Layer> overlay) {
			LOG_CORE_TRACE("Pushing Overlay onto Layer Stack");
			layers.push_back(std::move(overlay));
			layers[layers.size() - 1]->OnAttach();
		}

		const std::vector<std::shared_ptr<Layer>>& GetLayers() const {
			return layers;
		}

		void ClearStack() {

			if (layers.size() == 0)
				return;

			LOG_CORE_TRACE("Popping all layers from LayerStack");
			while (layers.size() > 0) {
				LOG_CORE_TRACE("Layer OnDetach()");
				layers[layers.size() - 1]->OnDetach();
				layers.pop_back();
			}
		}
	};

}
