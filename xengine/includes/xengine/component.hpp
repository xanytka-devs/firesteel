#ifndef COMPONENT_H
#define COMPONENT_H

#include <unordered_map>
#include <functional>
#include <memory>
#include <typeinfo>
#include <typeindex>

namespace XEngine {
	class Transform;
    class Component;

    class ComponentRegistry {
    public:
        using ComponentCreator = std::function<std::shared_ptr<Component>(const Component&)>;

        static ComponentRegistry& instance() {
            static ComponentRegistry instance;
            return instance;
        }

        template <typename T>
        void push() {
            creators_[typeid(T)] = [=](const Component& original) -> std::shared_ptr<Component> {
                return std::make_shared<T>(dynamic_cast<const T&>(original));
            };
        }

        std::shared_ptr<Component> component_by_type(const Component& original);

    private:
        std::unordered_map<std::type_index, ComponentCreator> creators_;
    };

	class Component {
	public:
		Component();
		Component(Transform& t_transform);

		virtual void initialize() { }
		virtual void update() { }
		virtual void on_destroy() { m_transform = nullptr; }

		Transform get_transform() const;
		void set_transform(Transform* t_value);

        virtual std::shared_ptr<Component> clone() const {
            return ComponentRegistry::instance().component_by_type(*this);
        }
	protected:
		Transform* m_transform;
	};
}

#endif // COMPONENT_H