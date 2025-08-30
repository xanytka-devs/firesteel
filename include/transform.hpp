#ifndef FS_TRANSFORM_H
#define FS_TRANSFORM_H
#include <glm/gtc/quaternion.hpp>

struct Transform {
    glm::vec3 position=glm::vec3(0);
    glm::vec3 rotation=glm::vec3(0);
    glm::vec3 size=glm::vec3(1);
    Transform(const glm::vec3& tPos=glm::vec3(0), const glm::vec3& tRot=glm::vec3(0), const glm::vec3& tSize=glm::vec3(1)) {
        position=tPos;
        rotation=tRot;
        size=tSize;
    }
    glm::vec3 eulerAngles() const {
        return glm::radians(rotation);
    }
    glm::quat quaternion() const {
        return glm::quat(eulerAngles());
    }
    glm::mat4 getMatrix() const {
        glm::mat4 result=glm::mat4(1);
        result=glm::translate(result,position);
        result*=glm::mat4_cast(quaternion());
        result=glm::scale(result,size);
        return result;
    }
};

#endif // !FS_TRANSFORM_H
