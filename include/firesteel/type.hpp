#ifndef FS_TYPES
#define FS_TYPES

#ifndef FS_NO_JSON
#include <firesteel/utils/json.hpp>
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
		virtual void fromJson(void* tVal, const nlohmann::json& tJson) const=0;
#endif // !FS_NO_JSON
	};

	class TypeRegistry {
	public:
		static TypeRegistry& sInstance() {
			static TypeRegistry inst;
			return inst;
		}
		template<typename T>
		void registerType(std::unique_ptr<ITypeHandler> tHandler) {
			mHandlers[typeid(T)]=std::move(tHandler);
		}
		ITypeHandler* get(const std::type_index& tInfo) const {
			auto it=mHandlers.find(tInfo);
			return (it!=mHandlers.end())?it->second.get():nullptr;
		}
		template<typename T>
		ITypeHandler* get() const {
			auto it=mHandlers.find(typeid(T));
			return (it!=mHandlers.end())?it->second.get():nullptr;
		}
	private:
		std::unordered_map<std::type_index, std::unique_ptr<ITypeHandler>> mHandlers;
	};
}

#endif // !FS_TYPES