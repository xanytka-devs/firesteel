#ifndef COMPONENT_H
#define COMPONENT_H

namespace XEngine {
	class Transform;

	class Component {
	public:
		Component();
		Component(Transform& t_transform);

		virtual void initialize() { }
		virtual void update() { }
		virtual void on_destroy() { m_transform = nullptr; }

		Transform get_transform() const;
		int get_id() const;

		static void reset_global_id() { last_id = 0; }
	protected:
		static int last_id;
		unsigned const int m_id;
		Transform* m_transform;
	};
}

#endif // COMPONENT_H