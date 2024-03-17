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

        ~ComponentRegistry() {
            creators_.clear();
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
        /// <summary>
        /// Initializes component.
        /// </summary>
		Component();
        /// <summary>
        /// Initializes component.
        /// </summary>
        /// <param name="t_transform">Assigned transform.</param>
		Component(Transform& t_transform);

        virtual void initialize() { m_is_initialized = true; }
        bool is_initialized() const { return m_is_initialized; }
		virtual void update() { }
		virtual void on_destroy() { m_transform = nullptr; }

        /// <summary>
        /// Gets assigned transform.
        /// </summary>
        /// <returns>Assigned transform.</returns>
		Transform get_transform() const;
		void set_transform(Transform* t_value);

        virtual std::shared_ptr<Component> clone() const {
            return ComponentRegistry::instance().component_by_type(*this);
        }
	protected:
		Transform* m_transform;
        bool m_is_initialized = false;
	};
}

#endif // COMPONENT_H