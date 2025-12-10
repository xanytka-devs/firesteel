#ifndef FS_NO_COMPONENTS
#ifndef FS_COMPONENT
#define FS_COMPONENT

#include "builtin_types.hpp"

namespace Firesteel {
	class Entity;
	class Component {
	public:
		Component(Entity* tEntity)
			: mEntity(tEntity) {}
		virtual void onStart(){}
		virtual void onUpdate(){}
		virtual void onDraw(){}
		virtual void onRemove(){}
#ifndef FS_NO_JSON
		virtual nlohmann::json serialize() const {
			return {
				{"type", getName()}
			};
		}
#endif // !FS_NO_JSON

		bool isInitialized() const { return mInitialized; }
		virtual const char* getName() const { return "fs.generic"; }
	protected:
		Entity* mEntity;
		bool mInitialized=false;
	};
#ifndef FS_NO_JSON
	using ComponentFactory=std::function<std::shared_ptr<Component>(Entity*, const nlohmann::json&)>;
	class ComponentRegistry {
	public:
		static std::unique_ptr<ComponentRegistry> Instance;

		bool contains(const std::string& tName) {
			return mFactories.count(tName);
		}
		void append(const std::string& tName, ComponentFactory tFactory) {
			if(contains(tName)) {
				LOG_ERRR("Tried to overwrite component \""+tName+"\". Duplicate entries aren't allowed");
				return;
			}
			mFactories[tName]=tFactory;
		}
		ComponentFactory get(const std::string& tName) {
			ASSERT(contains(tName),"Component registry doesn't contain "+tName);
			return mFactories[tName];
		}
		const std::unordered_map<std::string, ComponentFactory> map() {
			return mFactories;
		}
	private:
		std::unordered_map<std::string,ComponentFactory> mFactories;
	};
	std::unique_ptr<ComponentRegistry> ComponentRegistry::Instance=nullptr;
#endif // !FS_NO_JSON
}

#endif // !FS_COMPONENT
#endif // !FS_NO_COMPONENTS