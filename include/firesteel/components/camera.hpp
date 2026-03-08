#ifndef FS_COMP_CAMERA_H
#define FS_COMP_CAMERA_H
#include <firesteel/common.hpp>
#include <firesteel/component.hpp>
#include <firesteel/entity.hpp>
#include <firesteel/rendering/camera.hpp>

namespace Firesteel {
	class CameraComponent : public Component {
    public:
        CameraComponent(std::shared_ptr<Entity> tEntity,
            const float& tNearPlane=0.1f, const float& tFarPlane=100.f,
            const float& tFov=45.0f, const float& tAspect=0.5f, const bool& tIsPerspective=true)
            : Component(tEntity) {
                camera=Camera{tEntity->transform.position,tEntity->transform.rotation};
                camera.nearPlane=tNearPlane;
                camera.farPlane=tFarPlane;
                camera.fov=tFov;
                camera.aspect=tAspect;
                camera.isPerspective=tIsPerspective;
                camera.update();
        }
        void properties() override {
            PROP("nearPlane",camera.nearPlane);
            PROP("farPlane",camera.farPlane);
            PROP("fov",camera.fov);
            PROP("aspect",camera.aspect);
            PROP("isPerspective",camera.isPerspective);
        }

        Camera camera;
    };
}
#endif // !FS_COMP_CAMERA_H