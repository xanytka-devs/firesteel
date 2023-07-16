#include "XEngine/Entity.hpp"

namespace XEngine {

	Entity::Entity() {
		transform = std::make_unique<Transform>();
	}

	Entity::~Entity() {
		curId = 0;
	}

	/*void Entity::AddComponent(const Component* component) {
		
	}*/

}