
#include "Battery/pch.h"
#include "Battery/Renderer/Renderer2D.h"
#include "Battery/Core/Exception.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Log/Log.h"
#include "Battery/Core/Config.h"
#include "Battery/Core/AllegroContext.h"

#ifdef BATTERY_DEBUG // TODO
#define CHECK_INIT() \
	if (data == nullptr) { \
		LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Renderer is not initialized!"); \
		return;	\
	}
#else
#define CHECK_INIT()
#endif

namespace Battery {

	// This function is local within this .cpp file
	static ALLEGRO_COLOR ConvertAllegroColor(const glm::vec4& color) {
		return al_map_rgba(color.r, color.g, color.b, color.a);
	}



	VertexData::operator ALLEGRO_VERTEX() const {
		ALLEGRO_VERTEX v;

		v.x = position.x;
		v.y = position.y;
		v.z = position.z;
		v.u = uv.x;
		v.v = uv.y;
		v.color = ConvertAllegroColor(color);

		return v;
	}

	struct QuadData {
		VertexData v1;
		VertexData v2;
		VertexData v3;
		VertexData v4;
		int texture;
	};

	struct LineData {
		glm::vec2 p1;
		glm::vec2 p2;
		float thickness;
		float falloff;
		glm::vec4 color;
	};

	struct CircleData {
		glm::vec2 center;
		float diameter;
		float falloff;
		glm::vec4 color;
	};

	struct Renderer2DData {
		Scene* currentScene = nullptr;	// This is a Scene reference, do not delete

		std::vector<LineData> lineBuffer;
		bool linesActive = false;

		std::vector<CircleData> circleBuffer;
		bool circlesActive = false;

		ShaderProgram* quadShader = nullptr;
		std::vector<QuadData> quadBuffer;
		int quadTextureID = -1;
		bool quadsActive = false;
	};

	static Renderer2DData* data = nullptr;






	void Renderer2D::Setup() {
		if (data == nullptr) {
			data = new Renderer2DData();
		}
		else {
			LOG_CORE_CRITICAL("Can't setup Renderer2D: Already initialized!");
		}
	}

	void Renderer2D::Shutdown() {
		if (data != nullptr) {
			delete data;
			data = nullptr;
		}
		else {
			LOG_CORE_CRITICAL("Can't shutdown Renderer2D: Not initialized!");
		}
	}

	void Renderer2D::BeginScene(Scene* scene) {
		CHECK_INIT();

		// Some sanity checks
		if (data->currentScene != nullptr) {
			LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Can't begin scene, another scene is still active!");
			return;
		}

		if (scene == nullptr) {
			LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Can't load scene: Supplied scene pointer is null!");
			return;
		}

		if (!scene->window.has_value()) {
			LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Can't load scene: AllegroWindow pointer has no value!");
			return;
		}

		// Change pointer to the new scene
		data->currentScene = scene;

		if (scene->texture.has_value()) {	// Render to texture
			// Initialize the canvas for the scene
			al_set_target_bitmap(scene->texture.value().get().GetAllegroBitmap());
		}
		else {			// Render to screen normally
			// Initialize the canvas for the scene
			al_set_target_backbuffer(scene->window.value().get().allegroDisplayPointer);
		}
	}

	void Renderer2D::EndScene() {
		CHECK_INIT();

		if (data->currentScene == nullptr) {
			LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Can't end scene: No scene is currently active!");
			return;
		}

		// Let go of the reference to the scene object
		data->currentScene = nullptr;
	}

	void Renderer2D::EndUnfinishedScene() {
		CHECK_INIT();

		if (data->currentScene != nullptr) {
			LOG_CORE_WARN("{}: {}", __FUNCTION__, "The most recent scene is still active, make sure to call Renderer2D::EndScene()!");
			EndScene();
		}
	}





	

	void Renderer2D::DrawQuad(const VertexData& v1, const VertexData& v2, const VertexData& v3, const VertexData& v4, 
			ShaderProgram* shaderProgram, int textureID) {
		CHECK_INIT();
		LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Rendering a quad now!");

		if (!shaderProgram->IsLoaded()) {
			LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Can't render quad: The supplied shader is not loaded!");
			return;
		}

		ALLEGRO_VERTEX vertices[4] = { v1, v2, v3, v4 };
		int indices[6] = { 0, 1, 2, 
						   0, 2, 3 };

		// Load the texture
		shaderProgram->Use();
		ALLEGRO_BITMAP* texture = nullptr;

		// Find the selected texture
		if (textureID < data->currentScene->textures.size() && textureID >= 0) {
			texture = data->currentScene->textures[textureID].GetAllegroBitmap();

			if (texture == nullptr) {
				LOG_WARN("{}: {}", __FUNCTION__, "The specified texture is not valid");
			}
		}
		else {
			if (textureID != -1) {
				LOG_WARN("{}: {}", __FUNCTION__, "Invalid texture ID specified!");
			}
		}

		// Render the quad
		al_draw_indexed_prim(vertices, NULL, texture, indices, 6, ALLEGRO_PRIM_TRIANGLE_LIST);
		data->quadShader->Release();
	}

	void Renderer2D::DrawLine(const glm::vec2& p1, const glm::vec2& p2, float thickness, const glm::vec4& color, float falloff) {
		CHECK_INIT();
		LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Drawing line");

		thickness = std::max(thickness, 0.f);
		falloff = std::max(falloff, 0.f);

		glm::vec2 atob = glm::normalize(p2 - p1);
		glm::vec2 anorm = glm::vec2(atob.y, -atob.x);

		float r = thickness / 2;

		data->currentScene->lineShader->Use();
		data->currentScene->lineShader->SetUniformFloat("line_p1", p1);
		data->currentScene->lineShader->SetUniformFloat("line_p2", p2);
		data->currentScene->lineShader->SetUniformFloat("line_thickness", thickness);
		data->currentScene->lineShader->SetUniformFloat("line_falloff", falloff);

		VertexData v1 = VertexData(glm::vec3(p1 - atob * r + anorm * r, 0), color);
		VertexData v2 = VertexData(glm::vec3(p1 - atob * r - anorm * r, 0), color);
		VertexData v3 = VertexData(glm::vec3(p2 + atob * r - anorm * r, 0), color);
		VertexData v4 = VertexData(glm::vec3(p2 + atob * r + anorm * r, 0), color);

		// Line
		if (color.w != 0.f)
			DrawQuad(v1, v2, v3, v4, data->currentScene->lineShader.get());
		else
			LOG_CORE_TRACE("Line color alpha is 0: Skipping line");
	}

	void Renderer2D::DrawArc(const glm::vec2& center, float radius, float startAngle, float endAngle, float thickness,
		const glm::vec4& color, float falloff) {
		CHECK_INIT();
		LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Rendering arc");

		thickness = std::max(thickness, 0.f);
		falloff = std::max(falloff, 0.f);
		radius = std::max(radius, 0.f);
		startAngle = glm::radians(startAngle - 360.f * floor(startAngle / 360.f));
		endAngle = glm::radians(endAngle - 360.f * floor(endAngle / 360.f));

		float margin = thickness / 2.f + falloff;

		glm::vec2 toTop = glm::vec2(0, 1) * radius + glm::vec2(0, margin);
		glm::vec2 toRight = glm::vec2(1, 0) * radius + glm::vec2(margin, 0);

		if (!data->currentScene->arcShader->IsLoaded()) {
			LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Can't render circle: Circle shader is not loaded!"
				" Make sure a valid scene is active!");
			return;
		}

		data->currentScene->arcShader->Use();
		data->currentScene->arcShader->SetUniformFloat("arc_center", center);
		data->currentScene->arcShader->SetUniformFloat("arc_radius", radius);
		data->currentScene->arcShader->SetUniformFloat("arc_start_angle", startAngle);
		data->currentScene->arcShader->SetUniformFloat("arc_end_angle", endAngle);
		data->currentScene->arcShader->SetUniformFloat("arc_thickness", thickness);
		data->currentScene->arcShader->SetUniformFloat("arc_falloff", falloff);

		VertexData v1 = VertexData(glm::vec3(center - toRight + toTop, 0), color);
		VertexData v2 = VertexData(glm::vec3(center + toRight + toTop, 0), color);
		VertexData v3 = VertexData(glm::vec3(center + toRight - toTop, 0), color);
		VertexData v4 = VertexData(glm::vec3(center - toRight - toTop, 0), color);

		// Arc
		if (color.w != 0.f)
			DrawQuad(v1, v2, v3, v4, data->currentScene->arcShader.get());
		else
			LOG_CORE_TRACE("Arc color alpha is 0: Skipping arc");
	}

	void Renderer2D::DrawCircle(const glm::vec2& center, float radius, float outlineThickness,
		const glm::vec4& outlineColor, const glm::vec4& fillColor, float falloff) {
		CHECK_INIT();
		LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Rendering circle");

		radius = std::max(radius, 0.f);
		falloff = std::max(falloff, 0.f);

		glm::vec2 toTop = glm::vec2(0, 1) * radius;
		glm::vec2 toRight = glm::vec2(1, 0) * radius;

		if (!data->currentScene->circleShader->IsLoaded()) {
			LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Can't render circle: Circle shader is not loaded!"
				" Make sure a valid scene is active!");
			return;
		}

		data->currentScene->circleShader->Use();
		data->currentScene->circleShader->SetUniformFloat("circle_center", center);
		data->currentScene->circleShader->SetUniformFloat("circle_radius", radius);
		data->currentScene->circleShader->SetUniformFloat("circle_falloff", falloff);

		VertexData v1 = VertexData(glm::vec3(center - toRight + toTop, 0), fillColor);
		VertexData v2 = VertexData(glm::vec3(center + toRight + toTop, 0), fillColor);
		VertexData v3 = VertexData(glm::vec3(center + toRight - toTop, 0), fillColor);
		VertexData v4 = VertexData(glm::vec3(center - toRight - toTop, 0), fillColor);

		// Fill
		if (fillColor.w != 0.f)
			DrawQuad(v1, v2, v3, v4, data->currentScene->circleShader.get());
		else
			LOG_CORE_TRACE("Circle fillColor alpha is 0: Skipping fill");

		// Outline
		if (outlineColor.w != 0.f)
			DrawArc(center, radius, 0, 360, outlineThickness, outlineColor, falloff);
		else
			LOG_CORE_TRACE("Circle outlineColor alpha is 0: Skipping outline");
	}

	void Renderer2D::DrawRectangle(const glm::vec2& point1, const glm::vec2& point2, float outlineThickness,
			const glm::vec4& outlineColor, const glm::vec4& fillColor, float falloff) {
		CHECK_INIT();
		LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Rendering rectangle");

		if (!data->currentScene->rectangleShader->IsLoaded()) {
			LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Can't render rectangle: Rectangle shader is not loaded!"
				" Make sure a valid scene is active!");
			return;
		}

		VertexData v1 = VertexData(glm::vec3(point1.x, point1.y, 0), fillColor);
		VertexData v2 = VertexData(glm::vec3(point2.x, point1.y, 0), fillColor);
		VertexData v3 = VertexData(glm::vec3(point2.x, point2.y, 0), fillColor);
		VertexData v4 = VertexData(glm::vec3(point1.x, point2.y, 0), fillColor);

		// Fill
		if (fillColor.w != 0.f)
			DrawQuad(v1, v2, v3, v4, data->currentScene->rectangleShader.get());
		else
			LOG_CORE_TRACE("Rectangle fillColor alpha is 0: Skipping fill");

		// Outline
		if (outlineColor.w != 0.f) {
			DrawLine({ point1.x, point1.y }, { point2.x, point1.y }, outlineThickness, outlineColor, falloff);
			DrawLine({ point2.x, point1.y }, { point2.x, point2.y }, outlineThickness, outlineColor, falloff);
			DrawLine({ point2.x, point2.y }, { point1.x, point2.y }, outlineThickness, outlineColor, falloff);
			DrawLine({ point1.x, point2.y }, { point1.x, point1.y }, outlineThickness, outlineColor, falloff);
		}
		else {
			LOG_CORE_TRACE("Rectangle outlineColor alpha is 0: Skipping outline");
		}
	}






	// Primitive drawing routines

	void Renderer2D::DrawBackground(const glm::vec4& color) {
		CHECK_INIT();
		al_clear_to_color(ConvertAllegroColor(color));
	}

	void Renderer2D::DrawPrimitiveLine(const glm::vec2& p1, const glm::vec2& p2, float thickness, const glm::vec4& color) {
		CHECK_INIT();
		al_draw_line(p1.x, p1.y, p2.x, p2.y, ConvertAllegroColor(color), thickness);
	}

}
