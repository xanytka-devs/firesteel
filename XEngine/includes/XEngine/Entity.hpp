#pragma once

#include <iostream>

#include "Components/Component.hpp"

namespace XEngine {

	class Entity {
	public:
		Entity();
		~Entity();

		//void AddComponent(const Component* component);

		std::unique_ptr<class Transform> transform;
	private:
		unsigned int curId = 0;
		size_t componentsAmount = 0;
	};

}