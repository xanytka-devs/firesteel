#ifndef FS_CAMERA_H
#define FS_CAMERA_H
#include <glm/gtc/matrix_transform.hpp>

#include "common.hpp"
#include "transform.hpp"

namespace Firesteel {
    class Camera {
    public:
        // Rotation: X=Roll,Y=Yaw,Z=Pitch
        Transform transform;
        // Orientation.
        glm::vec3 forward;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        // Additional parameters.
        float nearPlane = 0.1f;
        float farPlane = 100.f;
        float fov = 45.0f;
        float aspect = 0.5f;
        bool isPerspective = true;

        // Constructor with vectors.
        Camera(glm::vec3 tPos = glm::vec3(0.f, 0.f, 0.f), glm::vec3 tRot = glm::vec3(0.f, 0.f, 0.f)) {
            transform.position = tPos;
            transform.rotation = tRot;
            worldUp = glm::vec3(0.f, 1.f, 0.f);
            forward = glm::vec3(0.0f, 0.0f, -1.0f);
            update();
        }

        // Returns the view matrix calculated using Euler Angles.
        glm::mat4 getView() const {
            return glm::lookAt(transform.position, transform.position + forward, up);
        }
        // Returns viewport matrix.
        glm::mat4 getProjection(float tClipSize = 1) const {
            if(isPerspective) return glm::perspective(glm::radians(fov), tClipSize * aspect, nearPlane, farPlane);
            else return glm::ortho(-tClipSize, tClipSize,
                -tClipSize/aspect, tClipSize/aspect, nearPlane, farPlane);
        }

        // Calculates the front vector from the Camera's (updated) Euler Angles.
        void update() {
            //Calculate Forward vector.
            glm::vec3 front{};
            float roll = glm::radians(transform.rotation.x);
            float pitch = glm::radians(transform.rotation.y);
            float yaw = glm::radians(transform.rotation.z);
            front.x = cos(yaw) * cos(pitch);
            front.y = sin(pitch);
            front.z = sin(yaw) * cos(pitch);
            forward = glm::normalize(front);
            //Re-calculate Right and Up vectors.
            right = glm::normalize(glm::cross(forward, worldUp));
            up = glm::mat3(glm::rotate(glm::mat4(1.0f), roll, forward))
                * glm::normalize(glm::cross(right, forward));
        }
    };
}

#endif // !FS_CAMERA_H