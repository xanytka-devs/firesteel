#ifndef FS_BUILTIN_TYPES
#define FS_BUILTIN_TYPES

#include <firesteel/type.hpp>
#include <glm/glm.hpp>

namespace Firesteel {
	struct BoolTypeHandler : public ITypeHandler {
		std::string toString(const void* tVal) const override {
			return *static_cast<const bool*>(tVal)?"true":"false";
		}
		void fromString(void* tVal, const std::string& tStr) const override {
			*static_cast<bool*>(tVal)=(tStr=="true");
		}
#ifndef FS_NO_JSON
		nlohmann::json toJson(void* tVal) const override {
			return *static_cast<const bool*>(tVal);
		}
		void fromJson(void* tVal, const nlohmann::json& tJson) const override {
			*static_cast<bool*>(tVal)=tJson.get<bool>();
		}
#endif // !FS_NO_JSON
	};
	struct IntTypeHandler : public ITypeHandler {
		std::string toString(const void* tVal) const override {
			return std::to_string(*static_cast<const int*>(tVal));
		}
		void fromString(void* tVal, const std::string& tStr) const override {
			*static_cast<int*>(tVal)=std::stoi(tStr);
		}
#ifndef FS_NO_JSON
		nlohmann::json toJson(void* tVal) const override {
			return *static_cast<const int*>(tVal);
		}
		void fromJson(void* tVal, const nlohmann::json& tJson) const override {
			*static_cast<int*>(tVal)=tJson.get<int>();
		}
#endif // !FS_NO_JSON
	};
	struct UIntTypeHandler : public ITypeHandler {
		std::string toString(const void* tVal) const override {
			return std::to_string(*static_cast<const unsigned int*>(tVal));
		}
		void fromString(void* tVal, const std::string& tStr) const override {
			*static_cast<unsigned int*>(tVal)=std::stoi(tStr);
		}
#ifndef FS_NO_JSON
		nlohmann::json toJson(void* tVal) const override {
			return *static_cast<const unsigned int*>(tVal);
		}
		void fromJson(void* tVal, const nlohmann::json& tJson) const override {
			*static_cast<unsigned int*>(tVal)=tJson.get<int>();
		}
#endif // !FS_NO_JSON
	};
	struct FloatTypeHandler : public ITypeHandler {
		std::string toString(const void* tVal) const override {
			return std::to_string(*static_cast<const float*>(tVal));
		}
		void fromString(void* tVal, const std::string& tStr) const override {
			*static_cast<float*>(tVal)=std::stof(tStr);
		}
#ifndef FS_NO_JSON
		nlohmann::json toJson(void* tVal) const override {
			return *static_cast<const float*>(tVal);
		}
		void fromJson(void* tVal, const nlohmann::json& tJson) const override {
			*static_cast<float*>(tVal)=tJson.get<float>();
		}
#endif // !FS_NO_JSON
	};
	struct DoubleTypeHandler : public ITypeHandler {
		std::string toString(const void* tVal) const override {
			return std::to_string(*static_cast<const double*>(tVal));
		}
		void fromString(void* tVal, const std::string& tStr) const override {
			*static_cast<double*>(tVal)=std::stof(tStr);
		}
#ifndef FS_NO_JSON
		nlohmann::json toJson(void* tVal) const override {
			return *static_cast<const double*>(tVal);
		}
		void fromJson(void* tVal, const nlohmann::json& tJson) const override {
			*static_cast<double*>(tVal)=tJson.get<double>();
		}
#endif // !FS_NO_JSON
	};
	struct StringTypeHandler : public ITypeHandler {
		std::string toString(const void* tVal) const override {
			return *static_cast<const std::string*>(tVal);
		}
		void fromString(void* tVal, const std::string& tStr) const override {
			*static_cast<std::string*>(tVal)=tStr;
		}
#ifndef FS_NO_JSON
		nlohmann::json toJson(void* tVal) const override {
			return *static_cast<const std::string*>(tVal);
		}
		void fromJson(void* tVal, const nlohmann::json& tJson) const override {
			*static_cast<std::string*>(tVal)=tJson.get<std::string>();
		}
#endif // !FS_NO_JSON
	};
	struct Vec2TypeHandler : public ITypeHandler {
		std::string toString(const void* tVal) const override {
			auto v=*static_cast<const glm::vec2*>(tVal);
			return std::to_string(v.x)+","+std::to_string(v.y);
		}
		void fromString(void* tVal, const std::string& tStr) const override {
			std::vector<float> vals;
			std::stringstream ss(tStr);
			std::string token;
			while (std::getline(ss, token, ',')) {
				try { vals.push_back(std::stof(token)); }
				catch(...) { vals.push_back(0); }
			}
			if(vals.size()<2) vals.resize(2,0);
			*static_cast<glm::vec2*>(tVal)=glm::vec2(vals[0], vals[1]);
		}
#ifndef FS_NO_JSON
		nlohmann::json toJson(void* tVal) const override {
			auto v=*static_cast<const glm::vec2*>(tVal);
			return nlohmann::json::array({v.x,v.y});
		}
		void fromJson(void* tVal, const nlohmann::json& tJson) const override {
			if(!tJson.is_array() || tJson.size() < 2) {
				*static_cast<glm::vec2*>(tVal)=glm::vec2(0);
				return;
			}
			*static_cast<glm::vec2*>(tVal)=glm::vec2(tJson[0],tJson[1]);
		}
#endif // !FS_NO_JSON
	};
	struct Vec3TypeHandler : public ITypeHandler {
		std::string toString(const void* tVal) const override {
			auto v=*static_cast<const glm::vec3*>(tVal);
			return std::to_string(v.x)+","+std::to_string(v.y)+","+std::to_string(v.z);
		}
		void fromString(void* tVal, const std::string& tStr) const override {
			std::vector<float> vals;
			std::stringstream ss(tStr);
			std::string token;
			while (std::getline(ss, token, ',')) {
				try { vals.push_back(std::stof(token)); }
				catch(...) { vals.push_back(0); }
			}
			if(vals.size()<3) vals.resize(3,0);
			*static_cast<glm::vec3*>(tVal)=glm::vec3(vals[0], vals[1], vals[2]);
		}
#ifndef FS_NO_JSON
		nlohmann::json toJson(void* tVal) const override {
			auto v=*static_cast<const glm::vec3*>(tVal);
			return nlohmann::json::array({v.x,v.y,v.z});
		}
		void fromJson(void* tVal, const nlohmann::json& tJson) const override {
			if(!tJson.is_array() || tJson.size() < 3) {
				*static_cast<glm::vec3*>(tVal)=glm::vec3(0);
				return;
			}
			*static_cast<glm::vec3*>(tVal)=glm::vec3(tJson[0],tJson[1],tJson[2]);
		}
#endif // !FS_NO_JSON
	};
	struct Vec4TypeHandler : public ITypeHandler {
		std::string toString(const void* tVal) const override {
			auto v=*static_cast<const glm::vec4*>(tVal);
			return std::to_string(v.x)+","+std::to_string(v.y)+","+std::to_string(v.z)+","+std::to_string(v.w);
		}
		void fromString(void* tVal, const std::string& tStr) const override {
			std::vector<float> vals;
			std::stringstream ss(tStr);
			std::string token;
			while (std::getline(ss, token, ',')) {
				try { vals.push_back(std::stof(token)); }
				catch(...) { vals.push_back(0); }
			}
			if(vals.size()<4) vals.resize(4,0);
			*static_cast<glm::vec4*>(tVal)=glm::vec4(vals[0], vals[1], vals[2], vals[3]);
		}
#ifndef FS_NO_JSON
		nlohmann::json toJson(void* tVal) const override {
			auto v=*static_cast<const glm::vec4*>(tVal);
			return nlohmann::json::array({v.x,v.y,v.z,v.w});
		}
		void fromJson(void* tVal, const nlohmann::json& tJson) const override {
			if(!tJson.is_array() || tJson.size() < 4) {
				*static_cast<glm::vec4*>(tVal)=glm::vec4(0);
				return;
			}
			*static_cast<glm::vec4*>(tVal)=glm::vec4(tJson[0],tJson[1],tJson[2],tJson[3]);
		}
#endif // !FS_NO_JSON
	};
}

#endif // !FS_BUILTIN_TYPES