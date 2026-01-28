#ifndef FS_NO_COMPONENTS
#ifndef FS_COMPONENT
#define FS_COMPONENT
#include <firesteel/builtin_types.hpp>
#include <firesteel/property.hpp>

namespace Firesteel {
	class Entity;
	class Component {
	public:
		Component(std::shared_ptr<Entity> tEntity)
			: mEntity(tEntity) {
			registerProperties();
		}
		template<typename T>
		static std::shared_ptr<Component> create(std::shared_ptr<Entity> tEntity, const nlohmann::json& tData) {
            ASSERT((std::is_base_of<Component,T>::value), "Given component must be derived from base Component type");
			auto comp=std::make_shared<T>(tEntity);
			comp->registerProperties();
			if(!tData.empty()) comp->deserialize(tData);
			return comp;
		}
		virtual void onStart() {}
		virtual void onUpdate() {}
		virtual void onDraw() {}
		virtual void onRemove() {}
#ifndef FS_NO_JSON
		virtual nlohmann::json serialize() const {
			nlohmann::json j;
			j["type"]=name();
			if(!mProperties.empty()) {
				nlohmann::json data=nlohmann::json::object();
				for(const auto& prop:mProperties) data[prop.name()]=prop.serialize();
				j["data"]=data;
			}
			return j;
		}
		virtual void deserialize(const nlohmann::json& tData) {
			if(!tData.contains("data")) return;
			const auto& data=tData["data"];
			for(auto& p:mProperties) if(data.contains(p.name())) p.deserialize(data[p.name()]);
		}
#endif // !FS_NO_JSON

		void registerProperties() { properties(); }
		bool isInitialized() const { return mInitialized; }
		virtual const char* name() const { return "fs.generic"; }
	protected:
		std::shared_ptr<Entity> mEntity;
		bool mInitialized=false;

		virtual void properties() {}
		template<typename T>
		void addProperty(const char* tName, T* tVal) {
			auto* handler=TypeRegistry::sInstance()->get<T>();
			if(handler) mProperties.emplace_back(tName,tVal,handler);
			else LOGF_ERRR("No Type Handler for property \"%s\"",tName);
		}
		std::vector<Property> mProperties;
	};
#ifndef FS_NO_JSON
	using ComponentFactory=std::function<std::shared_ptr<Component>(std::shared_ptr<Entity>, const nlohmann::json&)>;
	template<typename T>
	ComponentFactory DefaultComponentFactory=ComponentFactory([](std::shared_ptr<Entity> e, const nlohmann::json& j) {return Component::create<T>(e,j);});
	class ComponentRegistry {
	public:
		static ComponentRegistry* sInstance() {
			static ComponentRegistry instance;
			return &instance;
		}

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
#endif // !FS_NO_JSON
}

#endif // !FS_COMPONENT
#endif // !FS_NO_COMPONENTS