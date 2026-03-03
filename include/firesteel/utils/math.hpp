#ifndef FS_UTIL_MATH_H
#define FS_UTIL_MATH_H

#include <firesteel/common.hpp>

namespace Math {
    // Lineary interpolate between `tA` and `tB` by fraction of `tF`.
    float lerp(const float& tA, const float& tB, const float& tF) {
        return tA * (1.f - tF) + (tB * tF);
    }
    glm::vec2 lerp(const glm::vec2& tA, const glm::vec2& tB, const float& tF) {
        return glm::vec2(lerp(tA.x, tB.x, tF),lerp(tA.y, tB.y, tF));
    }
    glm::vec3 lerp(const glm::vec3& tA, const glm::vec3& tB, const float& tF) {
        return glm::vec3(lerp(tA.x, tB.x, tF),lerp(tA.y, tB.y, tF),lerp(tA.z, tB.z, tF));
    }
    glm::vec4 lerp(const glm::vec4& tA, const glm::vec4& tB, const float& tF) {
        return glm::vec4(lerp(tA.x, tB.x, tF),lerp(tA.y, tB.y, tF),lerp(tA.z, tB.z, tF),lerp(tA.w, tB.w, tF));
    }

    // Cast array to glm::mat4.
    glm::mat4 toMat4(const float** tF) {
        return glm::mat4(
            tF[0][0],tF[0][1],tF[0][2],tF[0][3],
            tF[1][0],tF[1][1],tF[1][2],tF[1][3],
            tF[2][0],tF[2][1],tF[2][2],tF[2][3],
            tF[3][0],tF[3][1],tF[3][2],tF[3][3]
        );
    }
    glm::mat4 toMat4(const double** tF) {
        return glm::mat4(
            CASTF(tF[0][0]),CASTF(tF[0][1]),CASTF(tF[0][2]),CASTF(tF[0][3]),
            CASTF(tF[1][0]),CASTF(tF[1][1]),CASTF(tF[1][2]),CASTF(tF[1][3]),
            CASTF(tF[2][0]),CASTF(tF[2][1]),CASTF(tF[2][2]),CASTF(tF[2][3]),
            CASTF(tF[3][0]),CASTF(tF[3][1]),CASTF(tF[3][2]),CASTF(tF[3][3])
        );
    }
    // Cast std::vector to glm::mat4.
    glm::mat4 toMat4(const std::vector<float>* tF) {
        if(tF->size()<17) return glm::mat4(1);
        glm::mat4 result{};
        for(int i=0;i<4;i++)
            result[i*4]=glm::vec4(tF->at(i),tF->at(i+1),tF->at(i+2),tF->at(i+3));
        return result;
    }
    glm::mat4 toMat4(const std::vector<double>* tF) {
        if(tF->size()<17) return glm::mat4(1);
        glm::mat4 result{};
        for(int i=0;i<4;i++)
            result[i*4]=glm::vec4(tF->at(i),tF->at(i+1),tF->at(i+2),tF->at(i+3));
        return result;
    }

    // Calculate position on theoretical circle from given radius and angle.
    glm::vec2 posOnCircle(const float& tRad, const float& tAngle) {
        return glm::vec2(tRad*cos(tAngle*(3.14f/180.f)),tRad*sin(tAngle*(3.14f/180.f)));
    }
    // Calculates position on theoretical sphere.
    // @param `tTheta` Angle in XY plane.
    // @param `tPhi` Angle from Z axis.
    glm::vec3 posOnSphere(const float& tRad, const float& tTheta, const float& tPhi) {
        float theta=glm::radians(tTheta);
        float phi=glm::radians(tPhi);
        return glm::vec3(
            tRad*sin(phi)*cos(theta),
            tRad*sin(phi)*sin(theta),
            tRad*cos(phi)
        );
    }
}

#endif // !FS_UTIL_MATH_H