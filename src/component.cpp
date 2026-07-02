#ifndef FS_NO_COMPONENTS
#include <firesteel/component.hpp>
#include <firesteel/builtin_types.hpp>
#include <firesteel/property.hpp>

namespace Firesteel {
	Component::Component(std::shared_ptr<Entity> tEntity)
    	: mEntity(tEntity) {
    	registerProperties();
    }
    template<typename T>
    static std::shared_ptr<Component> Component::create(std::shared_ptr<Entity> tEntity, const nlohmann::json& tData) {
    	ASSERT((std::is_base_of<Component, T>::value), "Given component must be derived from base Component type");
    	auto comp = std::make_shared<T>(tEntity);
    	comp->registerProperties();
    	if(!tData.empty()) comp->deserialize(tData);
    	return comp;
    }
    void Component::start() {
    	if(enabled) onStart();
    }
    void Component::update() {
    	if(enabled) onUpdate();
    }
    void Component::draw() {
    	if(enabled) onDraw();
    }
    void Component::remove() {
    	if(enabled) onRemove();
    }
#ifndef FS_NO_JSON
    nlohmann::json Component::serialize() const {
    	nlohmann::json j;
    	j["type"] = name();
    	if(!mProperties.empty()) {
            nlohmann::json data = nlohmann::json::object();
            for(const auto& prop : mProperties) data[prop.name()] = prop.serialize();
            j["data"] = data;
    	}
    	return j;
    }
    void Component::deserialize(const nlohmann::json& tData) {
    	if(!tData.contains("data")) return;
    	const auto& data = tData["data"];
    	for(auto& p : mProperties) if(data.contains(p.name())) p.deserialize(data[p.name()]);
    }
#endif // !FS_NO_JSON

    void Component::registerProperties() { properties(); }
    template<typename T>
    void Component::addProperty(const char* tName, T* tVal) {
    	auto* handler=TypeRegistry::sInstance()->get<T>();
    	if(handler) mProperties.emplace_back(tName, tVal, handler);
    	else LOGF_ERRR("No Type Handler for property \"%s\"", tName);
    }
#ifndef FS_NO_JSON
    ComponentRegistry* ComponentRegistry::sInstance() {
    	static ComponentRegistry instance;
    	return &instance;
    }

    bool ComponentRegistry::contains(const std::string& tName) {
    	return mFactories.count(tName);
    }
    void ComponentRegistry::append(const std::string& tName, ComponentFactory tFactory) {
    	if (contains(tName)) {
            LOG_ERRR("Tried to overwrite component \"" + tName + "\". Duplicate entries aren't allowed");
            return;
    	}
    	mFactories[tName] = tFactory;
    }
    ComponentFactory ComponentRegistry::get(const std::string& tName) {
    	ASSERT(contains(tName), "Component registry doesn't contain " + tName);
    	return mFactories[tName];
    }
#endif // !FS_NO_JSON
}

#endif // !FS_NO_COMPONENTS