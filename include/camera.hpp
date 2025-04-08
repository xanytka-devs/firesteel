#ifndef FS_CAMERA_H
#define FS_CAMERA_H

#include "common.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Firesteel {
    class Camera {
    public:
        // Position.
        glm::vec3 pos;
        glm::vec3 Forward;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 worldUp;
        // Euler & planes.
        glm::vec3 rot; // X=Roll,Y=Yaw,Z=Pitch
        float nearPlane = 0.1f;
        float farPlane = 100.f;
        float fov = 45.0f;
        float aspect = 0.5f;
        bool isPerspective = true;

        /// Constructor with vectors.
        Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 t_rot = glm::vec3(0.f, 0.f, 0.f)) {
            pos = position;
            rot = t_rot;
            worldUp = glm::vec3(0.f, 1.f, 0.f);
            Forward = glm::vec3(0.0f, 0.0f, -1.0f);
            update();
        }

        /// Returns the view matrix calculated using Euler Angles.
        glm::mat4 getView() const {
            return glm::lookAt(pos, pos + Forward, Up);
        }
        /// Returns viewport matrix.
        glm::mat4 getProjection(float tClipSize = 1) const {
            if(isPerspective) return glm::perspective(glm::radians(fov), tClipSize * aspect, nearPlane, farPlane);
            else return glm::ortho(-tClipSize, tClipSize,
                -tClipSize/aspect, tClipSize/aspect, nearPlane, farPlane);
        }

        /// Calculates the front vector from the Camera's (updated) Euler Angles.
        void update() {
            // Calculate Forward vector.
            glm::vec3 front{};
            float roll = glm::radians(rot.x);
            float pitch = glm::radians(rot.y);
            float yaw = glm::radians(rot.z);
            front.x = cos(yaw) * cos(pitch);
            front.y = sin(pitch);
            front.z = sin(yaw) * cos(pitch);
            Forward = glm::normalize(front);
            // Re-calculate Right and Up vectors.
            Right = glm::normalize(glm::cross(Forward, worldUp));
            Up = glm::mat3(glm::rotate(glm::mat4(1.0f), roll, Forward))
                * glm::normalize(glm::cross(Right, Forward));
        }
    };
}

#endif // !FS_CAMERA_H