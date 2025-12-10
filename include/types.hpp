#ifndef FS_TYPES
#define FS_TYPES

#ifndef FS_NO_JSON
#include "utils/json.hpp"
#endif // !FS_NO_JSON
#include <typeindex>
#include <type_traits>

namespace Firesteel {
	struct ITypeHandler {
		virtual ~ITypeHandler()=default;
		virtual std::string toString(const void* tVal) const=0;
		virtual void fromString(void* tVal, const std::string& tStr) const=0;
#ifndef FS_NO_JSON
		virtual nlohmann::json toJson(void* tVal) const=0;
		virtual void fromJson(void* tVal, const nlohmann::json& tStr) const=0;
#endif // !FS_NO_JSON
	};

	template<typename T>
	std::string serString(const T& tVal);
	template<typename T>
	T deserString(const std::string& tStr);
#ifndef FS_NO_JSON
	template<typename T>
	nlohmann::json serJson(const T& tVal);
	template<typename T>
	T deserJson(const nlohmann::json& tJson);
#endif // !FS_NO_JSON

	template<typename T>
	struct TypeHandler : public ITypeHandler {
	public:
		std::string toString(const void* tVal) const override {
			return serString(*static_cast<const T*>(tVal));
		}
		void fromString(void* tVal, const std::string& tStr) const override {
			*static_cast<T*>(tVal)=deserString<T>(tStr);
		}
#ifndef FS_NO_JSON
		nlohmann::json toJson(const void* tVal) const override {
			return serJson(*static_cast<const T*>(tVal));
		}
		void fromJson(void* tVal, const nlohmann::json& tJson) const override {
			*static_cast<T*>(tVal)=deserJson<T>(tJson);
		}
#endif // !FS_NO_JSON
	};
	class TypeRegistry {
	public:
		static TypeRegistry& sInstance() {
			static TypeRegistry inst;
			return inst;
		}
		template<typename T>
		void registerType(const std::string& tName) {
			mHandler[tName]=std::make_unique<TypeHandler<T>>();
			mTypeNames[typeid(T)]=tName;
		}
		const ITypeHandler* getHandler(const std::string& tName) const {
			auto it=mHandler.find(tName);
			return (it!=mHandler.end())?it->second.get():nullptr;
		}
		const ITypeHandler* getHandler(const std::type_info& tInfo) const {
			auto it=mTypeNames.find(tInfo);
			return (it!=mTypeNames.end())?getHandler(it->second):nullptr;
		}
		std::string getTypeName(const std::type_info& tInfo) const {
			auto it=mTypeNames.find(tInfo);
			return (it!=mTypeNames.end())?it->second:"unknown";
		}
	private:
		std::unordered_map<std::string, std::unique_ptr<ITypeHandler>> mHandler;
		std::unordered_map<std::type_index, std::string> mTypeNames;
	};
}

#endif // !FS_TYPES