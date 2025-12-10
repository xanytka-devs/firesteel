#ifndef FS_BUILTIN_TYPES
#define FS_BUILTIN_TYPES

#include "types.hpp"

namespace Firesteel {
	/* BOOLEAN */
	template<typename T>
	std::string serString<bool>(const T& tVal) {return static_cast<const bool>(tVal)?"true":"false";}
	template<>
	inline bool deserString<bool>(const std::string& tStr) {return tStr=="true";}
#ifndef FS_NO_JSON
	template<typename T>
	inline nlohmann::json serJson<bool>(const T& tVal) {return static_cast<const bool>(tVal);}
	template<>
	inline bool deserJson<bool>(const nlohmann::json& tJson) {return tJson.get<bool>();}
#endif // !FS_NO_JSON

	/* INT */
	template<typename T>
	std::string serString<int>(const T& tVal) {return std::to_string(static_cast<const int>(tVal));}
	template<>
	inline int deserString<int>(const std::string& tStr) { return std::stoi(tStr); }
#ifndef FS_NO_JSON
	template<typename T>
	inline nlohmann::json serJson<int>(const T& tVal) { return static_cast<const int>(tVal); }
	template<>
	inline int deserJson<int>(const nlohmann::json& tJson) { return tJson.get<int>(); }
#endif // !FS_NO_JSON

	/* FLOAT */
	template<typename T>
	std::string serString<float>(const T& tVal) {return std::to_string(static_cast<const float>(tVal));}
	template<>
	inline float deserString<float>(const std::string& tStr) { return std::stoi(tStr); }
#ifndef FS_NO_JSON
	template<typename T>
	inline nlohmann::json serJson<float>(const T& tVal) { return static_cast<const float>(tVal); }
	template<>
	inline float deserJson<float>(const nlohmann::json& tJson) { return tJson.get<float>(); }
#endif // !FS_NO_JSON
}

#endif // !FS_BUILTIN_TYPES