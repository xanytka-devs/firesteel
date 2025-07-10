#ifndef FS_TRANSFORM
#define FS_TRANSFORM
#include <glm/vec3.hpp>

struct Transform {
    glm::vec3 position=glm::vec3(0);
    glm::vec3 rotation=glm::vec3(0);
    glm::vec3 size=glm::vec3(1);
    Transform(const glm::vec3& tPos=glm::vec3(0), const glm::vec3& tRot=glm::vec3(0), const glm::vec3& tSize=glm::vec3(1)) {
        position=tPos;
        rotation=tRot;
        size=tSize;
    }
};

#endif // !FS_TRANSFORM
