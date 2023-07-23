#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

#include "XEngine/Entity.hpp"

namespace XEngine {

	class Component;
	class Entity;

	using ComponentID = size_t;

	inline ComponentID getComponentID() {
		static ComponentID lastID = 0;
		return lastID++;
	}

	template <typename T> inline ComponentID getComponentID() noexcept {
		static ComponentID typeID = getComponentID();
		return typeID;
	}

	constexpr size_t maxComponents = 32;
	using ComponentBitSet = std::bitset<maxComponents>();
	using ComponentArray = std::array<Component*, maxComponents>();

	class Component {
	public:
		~Component();

		Entity* entity;

		virtual void initialized() { }
		virtual void start() { }
		virtual void update() { }
		virtual void onDestroy() { }
	};

}