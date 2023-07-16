#include "XEngine/Components/Component.hpp"
#include "XEngine/Scene.hpp"

namespace XEngine {

	Component::Component() {
		initialized();
		start();
	}
	Component::~Component() {
		exit();
	}

}