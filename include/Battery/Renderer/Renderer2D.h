#pragma once

#include "Battery/pch.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Core/AllegroWindow.h"
#include "Battery/Renderer/ShaderProgram.h"
#include "Battery/Renderer/Bitmap.h"
#include "Battery/DefaultShaders.h"

struct ALLEGRO_VERTEX;

namespace Battery {

	struct VertexData {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec4 color;

		VertexData(const glm::vec3& p, const glm::vec2& u, const glm::vec4& c) {
			position = p;
			uv = u;
			color = c;
		}

		VertexData(const glm::vec3& p, const glm::vec4& c) {
			position = p;
			color = c;
		}

		operator ALLEGRO_VERTEX() const;
	};

	struct Scene {

		Scene(std::reference_wrapper<AllegroWindow> window) {
			this->window = window;
			LOG_CORE_TRACE(__FUNCTION__"(): Constructed Battery::Scene, loading shaders");
			LoadShaders();
		}

		Scene(std::reference_wrapper<AllegroWindow> window, std::reference_wrapper<Battery::Bitmap> texture) {
			this->window = window;
			this->texture = texture;
			LOG_CORE_TRACE(__FUNCTION__"(): Constructed Battery::Scene, loading shaders");
			LoadShaders();
		}

		~Scene() {
			LOG_CORE_TRACE(__FUNCTION__"(): Destroying Battery::Scene");
		}

	private:
		void LoadShaders() {

			lineShader = std::make_unique<ShaderProgram>();
			circleShader = std::make_unique<ShaderProgram>();
			arcShader = std::make_unique<ShaderProgram>();
			rectangleShader = std::make_unique<ShaderProgram>();

			ALLEGRO_DISPLAY* display = window.value().get().allegroDisplayPointer;

			// Load the shader for drawing antialiased lines
			lineShader->LoadSource(display,
				BATTERY_SHADER_SOURCE_VERTEX_SIMPLE, BATTERY_SHADER_SOURCE_FRAGMENT_LINE);

			circleShader->LoadSource(display,
				BATTERY_SHADER_SOURCE_VERTEX_SIMPLE, BATTERY_SHADER_SOURCE_FRAGMENT_CIRCLE);

			arcShader->LoadSource(display,
				BATTERY_SHADER_SOURCE_VERTEX_SIMPLE, BATTERY_SHADER_SOURCE_FRAGMENT_ARC);

			rectangleShader->LoadSource(display,
				BATTERY_SHADER_SOURCE_VERTEX_SIMPLE, BATTERY_SHADER_SOURCE_FRAGMENT_COLOR_GRADIENT);
		}

	public:
		// The Renderer2D class is allowed to access
		friend class Renderer2D;

		std::vector<Bitmap> textures;

	protected:
		std::unique_ptr<ShaderProgram> lineShader;
		std::unique_ptr<ShaderProgram> circleShader;
		std::unique_ptr<ShaderProgram> arcShader;
		std::unique_ptr<ShaderProgram> rectangleShader;

		std::optional<std::reference_wrapper<AllegroWindow>> window;
		std::optional<std::reference_wrapper<Battery::Bitmap>> texture;
	};

	class Renderer2D {
	public:

		// These 2 functions are called automatically
		static void Setup();
		static void Shutdown();

		static void BeginScene(Scene* scene);
		static void EndScene();
		static void EndUnfinishedScene();

		static void DrawQuad(const VertexData& v1, const VertexData& v2, const VertexData& v3, const VertexData& v4,
			ShaderProgram* shaderProgram, int textureID = -1);
		
		static void DrawLine(const glm::vec2& p1, const glm::vec2& p2, float thickness, const glm::vec4& color,
			float falloff = BATTERY_ANTIALIASING_LINE_FALLOFF);
		
		static void DrawArc(const glm::vec2& center, float radius, float startAngle, float endAngle, float thickness,
			const glm::vec4& color, float falloff = BATTERY_ANTIALIASING_LINE_FALLOFF);

		// Set outlineThickness or outlineColor alpha to 0 for no line and set fillColor alpha to 0 for no fill
		static void DrawCircle(const glm::vec2& center, float radius, float outlineThickness,
			const glm::vec4& outlineColor, const glm::vec4& fillColor, float falloff = BATTERY_ANTIALIASING_LINE_FALLOFF);

		// Set outlineThickness or outlineColor alpha to 0 for no line and set fillColor alpha to 0 for no fill
		static void DrawRectangle(const glm::vec2& point1, const glm::vec2& point2, float outlineThickness, 
			const glm::vec4& outlineColor, const glm::vec4& fillColor, float falloff = BATTERY_ANTIALIASING_LINE_FALLOFF);

		// Primitive drawing routines
		static void DrawBackground(const glm::vec4& color);
		static void DrawPrimitiveLine(const glm::vec2& p1, const glm::vec2& p2, float thickness, const glm::vec4& color);
	};

}
