#pragma once

namespace Battery {
	
	/// 
    /// \brief Retrieve a configuration value from the global qlib.json file.
    /// \tparam datatype of the configuration value. Value is extracted from the JSON as this type
    /// \param[in] identifier The string identifier, joined by dots for nested JSON values
    /// \param[in] silent Set true to suppress error logs and only throw the exception silently
    /// \param[in] attempt Internal. Leave at the default
    /// \return The retrieved value as the template type T
    /// \throw std::runtime_error when any error occurrs and the value cannot be retrieved
    /// \ingroup qlib
    /// \see <tt>qlib::getJson<T, elements>(const char*)</tt>
    /// 
	extern const std::string BATTERY_SHADER_SOURCE_VERTEX_SIMPLE;

	extern const std::string BATTERY_SHADER_SOURCE_FRAGMENT_LINE;
	extern const std::string BATTERY_SHADER_SOURCE_FRAGMENT_CIRCLE;
	extern const std::string BATTERY_SHADER_SOURCE_FRAGMENT_ARC;

	extern const std::string BATTERY_SHADER_SOURCE_FRAGMENT_COLOR_GRADIENT;

}
