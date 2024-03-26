#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

#include "xengine/component.hpp"
#include "xengine/rendering/shader.hpp"
#include "xengine/rendering/transform.hpp"

namespace XEngine {
	enum class CollisionType {
		CT_NONE = 0x00,
		CT_AABB = 0x01,
		CT_SPHERE = 0x02,
	};

	class Collider {
	public:
		//Basic constructor.
		Collider(CollisionType t_type) : type(t_type),
			bb_max(glm::vec3(0)), bb_min(glm::vec3(0)), sp_center(glm::vec3(0)), sp_radius(0) { }
		//AABB.
		Collider(glm::vec3 t_min, glm::vec3 t_max) : type(CollisionType::CT_AABB),
			bb_min(t_min), bb_max(t_max), sp_center(glm::vec3(0)), sp_radius(0) { }
		//Sphere.
		Collider(glm::vec3 t_center, float t_radius) : type(CollisionType::CT_SPHERE),
			sp_center(t_center), sp_radius(t_radius), bb_max(glm::vec3(0)), bb_min(glm::vec3(0)) { }

		CollisionType type;
		//AABB.
		glm::vec3 bb_min;
		glm::vec3 bb_max;
		//Sphere.
		glm::vec3 sp_center;
		float sp_radius;

		/// <summary>
		/// Gets collider's center.
		/// </summary>
		/// <returns>Collider's center.</returns>
		glm::vec3 calc_center() const {
			return (type == CollisionType::CT_AABB) ? (bb_min + bb_max) / 2.0f : sp_center;
		}

		/// <summary>
		/// Gets collider's dimensions.
		/// </summary>
		/// <returns>Collider's dimensions.</returns>
		glm::vec3 calc_dimensions() const {
			return (type == CollisionType::CT_AABB) ? bb_max - bb_min : glm::vec3(2.0f * sp_radius);
		}

		/// <summary>
		/// Checks if collider fully contains point.
		/// </summary>
		/// <param name="t_point">Position of the point.</param>
		/// <returns>True if collider fully contains point.</returns>
		bool contains(glm::vec3 t_point) const {
			//AABB - Point larger than max and point smaller than min.
			if (type == CollisionType::CT_AABB)
				return	(t_point.x >= bb_min.x) && (t_point.x <= bb_max.x) &&
				(t_point.y >= bb_min.y) && (t_point.y <= bb_max.y) &&
				(t_point.z >= bb_min.z) && (t_point.z <= bb_max.z);
			//SPHERE - Distance is less than radius.
			else if (type == CollisionType::CT_SPHERE) {
				float dist_sqrd = 0.0f;
				for (int i = 0; i < 3; i++) {
					dist_sqrd += (sp_center[i] - t_point[i]) * (sp_center[i] - t_point[i]);
				}
				return dist_sqrd <= (sp_radius * sp_radius);
			}
		}

		/// <summary>
		/// Checks if collider fully contains region.
		/// </summary>
		/// <param name="t_collider">Collision object.</param>
		/// <returns>True if collider fully contains region.</returns>
		bool contains(Collider t_collider) {
			//AABB - Check if max and min points are contained.
			if (type == CollisionType::CT_AABB)
				return contains(t_collider.bb_min) && contains(t_collider.bb_max);
			//SPHERE (both) - Combination of distance from centers and t_collider's radius is less than radius.
			else if (type == CollisionType::CT_SPHERE && t_collider.type == CollisionType::CT_SPHERE)
				return glm::length(sp_center - t_collider.sp_center) + t_collider.sp_radius < sp_radius;
			//AABB & SPHERE
			else {
				//Center is outside the box.
				if (!contains(t_collider.sp_center)) return false;
				//Otherwise center is inside the box.
				for (int i = 0; i < 3; i++) {
					if (abs(bb_max[i] - t_collider.sp_center[i]) < t_collider.sp_radius ||
						abs(t_collider.sp_center[i] - bb_min[i]) < t_collider.sp_radius)
						return false;
				}
			}
			return true;
		}

		/// <summary>
		/// Checks if collider partialy contains region.
		/// </summary>
		/// <param name="t_collider">Collision object.</param>
		/// <returns>True if collider partialy contains region.</returns>
		bool intersects(Collider t_collider) {
			//AABB & AABB - Overlap all axes.
			if(type == CollisionType::CT_AABB && t_collider.type == CollisionType::CT_AABB) {
				glm::vec3 rad = calc_dimensions() / 2.0f;
				glm::vec3 rad_col = t_collider.calc_dimensions() / 2.0f;

				glm::vec3 center = calc_center();
				glm::vec3 center_col = t_collider.calc_center();

				glm::vec3 dist = abs(center - center_col);
				for (int i = 0; i < 3; i++) {
					if (dist[i] > rad[i] + rad_col[i])
						return false;
				}
				return true;
			}
			//SPHERE & SPHERE - Distance between centers must be less than combined radius.
			else if(type == CollisionType::CT_SPHERE && t_collider.type == CollisionType::CT_SPHERE)
				return glm::length(sp_center - t_collider.sp_center) < (sp_radius + t_collider.sp_radius);
			// SPHERE & AABB - Find distance (squared) to the closest plane.
			else if(type == CollisionType::CT_SPHERE && t_collider.type == CollisionType::CT_AABB) {
				float dist_sqrd = 0.0f;
				for(int i = 0; i < 3; i++) {
					//Determine closest side.
					float closestPt = std::max(t_collider.bb_min[i],
						std::min(sp_center[i], t_collider.bb_max[i]));
					//Add distance.
					dist_sqrd += (closestPt - sp_center[i]) * (closestPt - sp_center[i]);
				}

				return dist_sqrd < (sp_radius * sp_radius);
			}
			// AABB & SPHERE - Call collider's algorithm.
			else if(type == CollisionType::CT_AABB && t_collider.type == CollisionType::CT_SPHERE)
				return t_collider.intersects(*this);
			// Other variants are related to CT_NONE,
			// so there is no point for collision checks.
			else return false;
		}
	};

	class ColliderBase;

	typedef void (*CollisionCallback)(ColliderBase t_other);

	class ColliderBase {
	public:
		ColliderBase() : m_collider(CollisionType::CT_NONE) {}
		void on_collision(ColliderBase t_other) const { m_callback(t_other); }
		Collider get_collider() const { return m_collider; }
		void set_callback(CollisionCallback t_callback) {
			m_callback = t_callback;
		}
	private:
		Collider m_collider;
		CollisionCallback m_callback;
	};

	class CollissionSystem {
	public:
		static CollissionSystem instance() {
			static CollissionSystem m_instance;
			return m_instance;
		}
		std::vector<ColliderBase*> m_stored;

		void update() {
			for(size_t i1 = 0; i1 < m_stored.size(); i1++) {
				for(size_t i2 = 0; i2 < m_stored.size(); i2++) {
					if(m_stored[i1]->get_collider().intersects(m_stored[i2]->get_collider()))
						printf("INTERSECTION\n");
				}
			}
		}

		void remove() {
			m_stored.clear();
		}
	};


	class BoxCollider : public Component, ColliderBase {
	public:
		BoxCollider() : Component(), m_collider(CollisionType::CT_AABB) { }
		BoxCollider(Transform& t_transform) : Component(t_transform),
			center(m_transform->position), rotation(m_transform->rotation), size(m_transform->size),
			m_collider(center - size, center + size) { }

		void initialize() {
			CollissionSystem::instance().m_stored.push_back(this);
			Component::initialize();
		}

		void update() {
			m_collider.bb_min = center - size;
			m_collider.bb_max = center + size;
		}

		glm::vec3 center{ 0.f };
		glm::vec3 rotation{ 0.f };
		glm::vec3 size{ 1.f };
	private:
		Collider m_collider;
	};

	class SphereCollider : public Component, ColliderBase {
	public:
		SphereCollider() : Component(), m_collider(CollisionType::CT_SPHERE) { }
		SphereCollider(Transform& t_transform) : Component(t_transform),
			center(m_transform->position), radius((m_transform->size.x + m_transform->size.y + m_transform->size.z) / 3),
			m_collider(center, radius) { }

		void initialize() {
			CollissionSystem::instance().m_stored.push_back(this);
			Component::initialize();
		}

		void update() {
			m_collider.sp_center = center;
			m_collider.sp_radius = radius;
		}

		glm::vec3 center{ 0.f };
		float radius{ 1.f };
	private:
		Collider m_collider;
	};
}

#endif // COLLIDER_H