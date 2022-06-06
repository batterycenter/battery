
#include "Battery/Core/Log.h"
#include "Battery/Renderer/ShaderProgram.h"
#include "Battery/Utils/FileUtils.h"
/*
namespace Battery {

	ShaderProgram::ShaderProgram() {

	}

	ShaderProgram::ShaderProgram(ALLEGRO_DISPLAY* display, const std::string& vertexShader, const std::string& fragmentShader) {
		LOG_CORE_TRACE("Constructing Battery::ShaderProgram");
		Load(display, vertexShader, fragmentShader);
	}

	ShaderProgram::~ShaderProgram() {
		LOG_CORE_TRACE("Destructing Battery::ShaderProgram");

		if (loaded)
			al_destroy_shader(shader);
	}








	bool ShaderProgram::Load(ALLEGRO_DISPLAY* display, const std::string& vertexShader, const std::string& fragmentShader) {

		this->display = display;

		// Some checks in the beginning
		if (loaded) {
			LOG_CORE_ERROR("Can't load shader program: Is already loaded, unload first");
			return false;
		}

		if (!FileExists(vertexShader)) {
			LOG_CORE_ERROR("Can't find vertex shader file: " + vertexShader);
			return false;
		}

		if (!FileExists(fragmentShader)) {
			LOG_CORE_ERROR("Can't find fragment shader file: " + fragmentShader);
			return false;
		}

		// Check if the display flags are correct
		int flags = al_get_display_flags(display);
		if (!(flags & ALLEGRO_PROGRAMMABLE_PIPELINE))
			throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_PROGRAMMABLE_PIPELINE' was not set!");
		if (!(flags & ALLEGRO_OPENGL))
			throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_OPENGL' was not set!");


		if (!AllegroContext::GetInstance()->IsInitialized())
			throw Battery::Exception("Can't load Shader Program: The Allegro Context was not initialized yet!");

		// Now load everything
		shader = al_create_shader(ALLEGRO_SHADER_GLSL);

		if (!shader) {
			LOG_CORE_ERROR("Failed to create Allegro shader");
			ShowErrorMessageBox("Failed to create Allegro shader");
			return false;
		}

		if (!al_attach_shader_source_file(shader, ALLEGRO_VERTEX_SHADER, vertexShader.c_str())) {
			LOG_CORE_ERROR(std::string("Vertex shader failed: ") + al_get_shader_log(shader));
			ShowErrorMessageBox(std::string("Vertex shader failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		if (!al_attach_shader_source_file(shader, ALLEGRO_PIXEL_SHADER, fragmentShader.c_str())) {
			LOG_CORE_ERROR(std::string("Fragment shader failed: ") + al_get_shader_log(shader));
			ShowErrorMessageBox(std::string("Fragment shader failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		if (!al_build_shader(shader)) {
			LOG_CORE_ERROR(std::string("Shader linking failed: ") + al_get_shader_log(shader));
			ShowErrorMessageBox(std::string("Shader linking failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		loaded = true;
		return true;
	}

	bool ShaderProgram::LoadSource(ALLEGRO_DISPLAY* display, const std::string& vertexShader, const std::string& fragmentShader) {

		if (display == nullptr)
			throw Battery::Exception("The Allegro display pointer is NULL!");

		this->display = display;

		// Some checks in the beginning
		if (loaded) {
			LOG_CORE_ERROR("Can't load shader program: Is already loaded, unload first");
			return false;
		}

		// Check if the display flags are correct
		int flags = al_get_display_flags(display);
		if (!(flags & ALLEGRO_PROGRAMMABLE_PIPELINE))
			throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_PROGRAMMABLE_PIPELINE' was not set!");
		if (!(flags & ALLEGRO_OPENGL))
			throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_OPENGL' was not set!");


		if (!AllegroContext::GetInstance()->IsInitialized())
			throw Battery::Exception("Can't load Shader Program: The Allegro Context was not initialized yet!");

		// Now load everything
		shader = al_create_shader(ALLEGRO_SHADER_GLSL);

		if (!shader) {
			LOG_CORE_ERROR("Failed to create Allegro shader");
			ShowErrorMessageBox("Failed to create Allegro shader");
			return false;
		}

		if (!al_attach_shader_source(shader, ALLEGRO_VERTEX_SHADER, vertexShader.c_str())) {
			LOG_CORE_ERROR(std::string("Vertex shader failed: ") + al_get_shader_log(shader));
			ShowErrorMessageBox(std::string("Vertex shader failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		if (!al_attach_shader_source(shader, ALLEGRO_PIXEL_SHADER, fragmentShader.c_str())) {
			LOG_CORE_ERROR(std::string("Fragment shader failed: ") + al_get_shader_log(shader));
			ShowErrorMessageBox(std::string("Fragment shader failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		if (!al_build_shader(shader)) {
			LOG_CORE_ERROR(std::string("Shader linking failed: ") + al_get_shader_log(shader));
			ShowErrorMessageBox(std::string("Shader linking failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		loaded = true;
		return true;
	}

	void ShaderProgram::Unload() {

		if (loaded) {

			if (!AllegroContext::GetInstance()->IsInitialized())
				throw Battery::Exception("Can't unload Shader Program: The Engine is not initialized, .Unload() "
					"must be called in Shutdown(), as when the Application destructor is called, the engine is already down!");

			al_destroy_shader(shader);
			shader = nullptr;
			loaded = false;
		}
	}

	bool ShaderProgram::IsLoaded() {
		return loaded;
	}

	void ShaderProgram::Use() {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		al_use_shader(shader);
	}

	void ShaderProgram::Release() {
		al_use_shader(NULL);
	}







	// Uniforms

	bool ShaderProgram::SetUniformSampler(const char* name, ALLEGRO_BITMAP* texture, int ID) {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		if (!al_set_shader_sampler(name, texture, ID)) {
			LOG_CORE_WARN("WARNING: The Shader Sampler uniform '{}' was not set correctly or is not being used!", name);
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformMatrix(const char* name, glm::mat4 matrix) {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		ALLEGRO_TRANSFORM m;
		for (int x = 0; x < 4; x++)
			for (int y = 0; y < 4; y++)
				m.m[x][y] = matrix[x][y];

		if (!al_set_shader_matrix(name, &m)) {
			LOG_CORE_WARN("WARNING: The Shader Matrix uniform '{}' was not set correctly or is not being used!", name);
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformInt(const char* name, int n) {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		if (!al_set_shader_int(name, n)) {
			LOG_CORE_WARN("WARNING: The Shader Integer uniform '{}' was not set correctly or is not being used!", name);
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformInt(const char* name, glm::ivec2 n) {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		if (!al_set_shader_int_vector(name, 2, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Integer uniform '{}' was not set correctly or is not being used!", name);
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformInt(const char* name, glm::ivec3 n) {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		if (!al_set_shader_int_vector(name, 3, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Integer uniform '{}' was not set correctly or is not being used!", name);
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformInt(const char* name, glm::ivec4 n) {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		if (!al_set_shader_int_vector(name, 4, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Integer uniform '{}' was not set correctly or is not being used!", name);
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformFloat(const char* name, float n) {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		if (!al_set_shader_float(name, n)) {
			LOG_CORE_WARN("WARNING: The Shader Float uniform '{}' was not set correctly or is not being used!", name);
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformFloat(const char* name, glm::vec2 n) {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		if (!al_set_shader_float_vector(name, 2, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Float uniform '{}' was not set correctly or is not being used!", name);
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformFloat(const char* name, glm::vec3 n) {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		if (!al_set_shader_float_vector(name, 3, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Float uniform '{}' was not set correctly or is not being used!", name);
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformFloat(const char* name, glm::vec4 n) {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		if (!al_set_shader_float_vector(name, 4, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Float uniform '{}' was not set correctly or is not being used!", name);
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformBool(const char* name, bool n) {
		if (!loaded)
			throw Battery::Exception(std::string(__FUNCTION__) + ": ShaderProgram was not loaded!");

		if (!al_set_shader_bool(name, n)) {
			LOG_CORE_WARN("WARNING: The Shader Bool uniform '{}' was not set correctly or is not being used!", name);
			return false;
		}

		return true;
	}
}
*/