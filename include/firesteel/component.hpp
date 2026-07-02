#ifndef FS_NO_COMPONENTS
#ifndef FS_COMPONENT
#define FS_COMPONENT
#include <firesteel/builtin_types.hpp>
#include <firesteel/property.hpp>

namespace Firesteel {
	class Entity;
	class Component : public std::enable_shared_from_this<Component> {
	public:
		Component(std::shared_ptr<Entity> tEntity);
		std::shared_ptr<Component> getShared() {return shared_from_this(); }
		template<typename T>
		static std::shared_ptr<Component> create(std::shared_ptr<Entity> tEntity, const nlohmann::json& tData);
		void start();
		void update();
		void draw();
		void remove();
#ifndef FS_NO_JSON
		virtual nlohmann::json serialize() const;
		virtual void deserialize(const nlohmann::json& tData);
#endif // !FS_NO_JSON

		void registerProperties();
		virtual const char* name() const { return "fs.generic"; }
		std::shared_ptr<Entity> entity() { return mEntity; }
		bool enabled=true;
	protected:
		std::shared_ptr<Entity> mEntity;

		virtual void onStart() {}
		virtual void onUpdate() {}
		virtual void onDraw() {}
		virtual void onRemove() {}
		virtual void properties() {}
		template<typename T>
		void addProperty(const char* tName, T* tVal);
		std::vector<Property> mProperties;
	};
#ifndef FS_NO_JSON
	using ComponentFactory=std::function<std::shared_ptr<Component>(std::shared_ptr<Entity>, const nlohmann::json&)>;
	template<typename T>
	ComponentFactory DefaultComponentFactory=ComponentFactory([](std::shared_ptr<Entity> e, const nlohmann::json& j) {return Component::create<T>(e,j);});
	class ComponentRegistry {
	public:
		static ComponentRegistry* sInstance();

		bool contains(const std::string& tName);
		void append(const std::string& tName, ComponentFactory tFactory);
		ComponentFactory get(const std::string& tName);
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