#ifndef COMPONENT_H
#define COMPONENT_H

#include "xengine/rendering/transform.hpp"

namespace XEngine {
	class Component {
	public:
		Component(Transform t_transform) : m_transform(&t_transform) {}

		void initialize() { }
		void update() { }
		void on_destroy() { }

		Transform get_transform() { return *m_transform; }
	private:
		unsigned int m_id = 0;
	protected:
		Transform* m_transform;
	};
}

#endif // COMPONENT_H