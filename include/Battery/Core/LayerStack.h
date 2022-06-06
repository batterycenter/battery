#pragma once

#include "Battery/pch.h"
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
			if (layers.size() > 0) {
				ClearStack();
			}
		}

		void PushLayer(std::shared_ptr<Layer> layer) {
			LOG_CORE_TRACE("Pushing Layer '" + layer->GetDebugName() + "' onto Layer Stack");
			auto it = layers.insert(layers.begin() + layerNum, std::move(layer));
			LOG_CORE_TRACE("Layer '{}' OnAttach()", (*it)->GetDebugName().c_str());
			(*it)->OnAttach();
			layerNum++;
		}
		
		void PushOverlay(std::shared_ptr<Layer> overlay) {
			LOG_CORE_TRACE("Pushing Overlay '" + overlay->GetDebugName() + "' onto Layer Stack");
			layers.push_back(std::move(overlay));
			LOG_CORE_TRACE("Layer '{}' OnAttach()", layers[layers.size() - 1]->GetDebugName().c_str());
			layers[layers.size() - 1]->OnAttach();
		}

		const std::vector<std::shared_ptr<Layer>>& GetLayers() const {
			return layers;
		}

		void ClearStack() {
			LOG_CORE_TRACE("Popping all left over layers from Layer Stack");

			while (layers.size() > 0) {
				LOG_CORE_TRACE("Layer '{}' OnDetach()", layers[layers.size() - 1]->GetDebugName().c_str());
				layers[layers.size() - 1]->OnDetach();
				layers.pop_back();
			}
		}
	};

}
