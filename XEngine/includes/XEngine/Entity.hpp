#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

#include "Components/Component.hpp"

namespace XEngine {

	class Entity {
	public:
		//Entity();
		//~Entity();

		std::vector<std::unique_ptr<Component>> components;

		//Transform* transform;
		void setActive(bool status) { enabled = status; }
		void update() {
			if(!enabled) return;
			for(auto& c : components) c->update();
		}
		void destroy() {
			for (auto& c : components) c->onDestroy();
			enabled = false;
		}

		bool isEnabled() const { return enabled; }
		template <typename T> bool hasComponent() const {
			return componentBitSet[getComponentID<T>]
		}
		template <typename T, typename... TArgs>
		T& addComponent(TArgs&&... mArgs) {
			T* c(new T(std::forward<TArgs>(mArgs)...));
			c->entity = this;
			std::unique_ptr<Component> uPtr{ c };
			components.emplace_back(std::move(uPtr));

			componentArray[getComponentID<T>()] = c;
			componentArray[getComponentID<T>()] = true;

			c->initialized();
			return *c;
		}
		template<typename T> T& getComponent() const {
			auto ptr(componentArray[getComponentID<T>()]);
			return *static_cast<T*>(ptr);
		}
	private:
		unsigned int curId = 0;
		bool enabled = true;
		size_t componentsAmount = 0;
		ComponentArray componentArray;
		ComponentBitSet componentBitSet;
	};

	class EntityManager {
	public:
		void update() {
			for (auto& e : entities) e->update();
		}
		void refresh() {
			entities.erase(std::remove_if(std::begin(entities), std::end(entities),
				[](const std::unique_ptr<Entity>& mEntity) {
					return !mEntity->isEnabled();
				}), std::end(entities));
		}
		Entity& instantiate() {
			Entity* e = new Entity();
			std::unique_ptr<Entity> ePtr{ e };
			entities.emplace_back(std::move(ePtr));
			return *e;
		}
	private:
		std::vector<std::unique_ptr<Entity>> entities;
	};


}