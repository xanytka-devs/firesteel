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
    static glm::vec3 decomposeQuaternion(const glm::quat& tQuat) {
        return glm::degrees(glm::eulerAngles(tQuat));
    }
    static Transform decomposeMatrix(const glm::mat4 tMatrix) {
        Transform rslt{};
        //Get position.
        rslt.position=glm::vec3(tMatrix[3]);
        //Get scale.
        glm::vec3 scl0(tMatrix[0].x,tMatrix[1].x,tMatrix[2].x);
        glm::vec3 scl1(tMatrix[0].y,tMatrix[1].y,tMatrix[2].y);
        glm::vec3 scl2(tMatrix[0].z,tMatrix[1].z,tMatrix[2].z);
        rslt.size=glm::vec3(glm::length(scl0),glm::length(scl1),glm::length(scl2));
        //Get rotation.
        if(rslt.size.x!=0) scl0/=rslt.size.x;
        if(rslt.size.y!=0) scl0/=rslt.size.y;
        if(rslt.size.z!=0) scl0/=rslt.size.z;
        rslt.rotation=decomposeQuaternion(glm::quat_cast(glm::mat3(scl0,scl1,scl2)));
        return rslt;
    }
    void fromQuaternion(const glm::quat& tQuat) {
        rotation=decomposeQuaternion(tQuat);
    }
    void fromMatrix(const glm::mat4 tMat) {
        Transform rslt=decomposeMatrix(tMat);
        position=rslt.position;
        rotation=rslt.rotation;
        size=rslt.size;
    }
    void fromMatrix(const std::vector<float>& tMat) {
        fromMatrix(glm::mat4(
            tMat[0],tMat[1],tMat[2],tMat[3],
            tMat[4],tMat[5],tMat[6],tMat[7],
            tMat[8],tMat[9],tMat[10],tMat[11],
            tMat[12],tMat[13],tMat[14],tMat[15]
        ));
    }
    void fromMatrix(const std::vector<double>& tMat) {
        fromMatrix(glm::mat4(
            tMat[0],tMat[1],tMat[2],tMat[3],
            tMat[4],tMat[5],tMat[6],tMat[7],
            tMat[8],tMat[9],tMat[10],tMat[11],
            tMat[12],tMat[13],tMat[14],tMat[15]
        ));
    }
};

#endif // !FS_TRANSFORM_H
