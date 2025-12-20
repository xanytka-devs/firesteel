#ifndef FS_NO_COMPONENTS
#ifndef FS_PROPERTY
#define FS_PROPERTY
#include <firesteel/type.hpp>

namespace Firesteel {
	class Property {
	public:
		template<typename T>
		Property(const char* tName, T* tValue, ITypeHandler* tHandler)
			: mName(tName), mValue(tValue), mHandler(tHandler) { }

		const char* name() const { return mName; }
		std::string asString() const { return mHandler->toString(mValue); }
		void setFromString(const std::string& tStr) { mHandler->fromString(mValue, tStr); }
#ifndef FS_NO_JSON
		nlohmann::json serialize() const {
			return mHandler->toJson(mValue);
		}
		void deserialize(const nlohmann::json& tJson) {
			mHandler->fromJson(mValue,tJson);
		}
#endif // !FS_NO_JSON
	private:
		const char* mName;
		void* mValue;
		ITypeHandler* mHandler;
	};

#define PROPERTY(name) \
	{\
		auto* handler=Firesteel::TypeRegistry::sInstance()->get<std::remove_reference_t<decltype(name)>>();\
		if(handler) mProperties.emplace_back(#name,&name,handler);\
		else {LOGF_ERRR("No Type Handler for property \"%s\"",#name);}\
	}
}
#endif // !FS_PROPERTY
#endif // !FS_NO_COMPONENTS