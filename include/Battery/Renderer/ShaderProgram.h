#pragma once

#include "Battery/pch.h"

#include "glm/glm.hpp"
#include "Battery/Core.h"
#include "Battery/Core/Exception.h"

struct ALLEGRO_DISPLAY;
struct ALLEGRO_BITMAP;
struct ALLEGRO_SHADER;

namespace Battery {

	class ShaderProgram {
	public:

		ShaderProgram();
		ShaderProgram(ALLEGRO_DISPLAY* display, const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();

		bool Load(ALLEGRO_DISPLAY* display, const std::string& vertexShader, const std::string& fragmentShader);
		bool LoadSource(ALLEGRO_DISPLAY* display, const std::string& vertexShader, const std::string& fragmentShader);

		void Unload();
		bool IsLoaded();

		void Use();
		void Release();

		bool SetUniformSampler(const char* name, ALLEGRO_BITMAP* texture, int ID);
		bool SetUniformMatrix(const char* name, glm::mat4 matrix);
		bool SetUniformInt(const char* name, int n);
		bool SetUniformInt(const char* name, glm::ivec2 n);
		bool SetUniformInt(const char* name, glm::ivec3 n);
		bool SetUniformInt(const char* name, glm::ivec4 n);
		bool SetUniformFloat(const char* name, float n);
		bool SetUniformFloat(const char* name, glm::vec2 n);
		bool SetUniformFloat(const char* name, glm::vec3 n);
		bool SetUniformFloat(const char* name, glm::vec4 n);
		bool SetUniformBool(const char* name, bool n);

	private:

		bool loaded = false;
		ALLEGRO_DISPLAY* display = nullptr;	// This is just a supplied reference pointer, do not delete!!!
		ALLEGRO_SHADER* shader = nullptr;	// This is an object pointer and must be destroyed!

	};

}
