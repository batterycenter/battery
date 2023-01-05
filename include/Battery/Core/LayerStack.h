#pragma once

#include "Battery/common.h"
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
			Log::Core::Trace("Pushing Layer onto Layer Stack");
			auto it = layers.insert(layers.begin() + layerNum, std::move(layer));
			(*it)->OnAttach();
			layerNum++;
		}
		
		void PushOverlay(std::shared_ptr<Layer> overlay) {
            Log::Core::Trace("Pushing Overlay onto Layer Stack");
			layers.push_back(std::move(overlay));
			layers[layers.size() - 1]->OnAttach();
		}

		[[nodiscard]] const std::vector<std::shared_ptr<Layer>>& GetLayers() const {
			return layers;
		}

		void ClearStack() {

			if (layers.empty())
				return;

            Log::Core::Trace("Popping all layers from LayerStack");
			while (!layers.empty()) {
                Log::Core::Trace("Layer OnDetach()");
				layers[layers.size() - 1]->OnDetach();
				layers.pop_back();
			}
		}
	};

}
