#pragma once

#include "XEngine/Entity.hpp"
#include "XEngine/Components/Transform.hpp"

namespace XEngine {

	class Component {
	public:
		Component();
		~Component();

		virtual void initialized() { }
		virtual void start() { }
		virtual void update() { }
		virtual void exit() { }
	};

}