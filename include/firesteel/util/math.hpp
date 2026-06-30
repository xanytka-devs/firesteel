#ifndef FS_UTIL_MATH_H
#define FS_UTIL_MATH_H

#include <firesteel/common.hpp>

namespace Math {
    //Bound given `tV` between `tMin` and `tMax`.
    int clamp(int tV, int tMin, int tMax) {
        if (tV < tMin) return tMin;
        if (tV > tMax) return tMax;
        return tV;
    }
    //Bound given `tV` between `tMin` and `tMax`.
    float clamp(float tV, float tMin, float tMax) {
        if(tV<tMin) return tMin;
        if(tV>tMax) return tMax;
        return tV;
    }
    //Bound given `tV` between `tMin` and `tMax`.
    double clamp(double tV, double tMin, double tMax) {
        if (tV < tMin) return tMin;
        if (tV > tMax) return tMax;
        return tV;
    }
    //Bound given `tV` between `tMin` and `tMax`.
    glm::vec2 clamp(glm::vec2 tV, glm::vec2 tMin, glm::vec2 tMax) {
        glm::vec2 t = tV;
        if (t.x > tMax.x) t.x = tMax.x;
        if (t.y > tMax.y) t.y = tMax.y;

        if (t.x < tMin.x) t.x = tMin.x;
        if (t.y < tMin.y) t.y = tMin.y;
        return t;
    }
    //Bound given `tV` between `tMin` and `tMax`.
    glm::vec3 clamp(glm::vec3 tV, glm::vec3 tMin, glm::vec3 tMax) {
        glm::vec3 t = tV;
        if (t.x > tMax.x) t.x = tMax.x;
        if (t.y > tMax.y) t.y = tMax.y;
        if (t.z > tMax.z) t.z = tMax.z;

        if (t.x < tMin.x) t.x = tMin.x;
        if (t.y < tMin.y) t.y = tMin.y;
        if (t.z < tMin.z) t.z = tMin.z;
        return t;
    }
    //Bound given `tV` between `tMin` and `tMax`.
    glm::vec4 clamp(glm::vec4 tV, glm::vec4 tMin, glm::vec4 tMax) {
        glm::vec4 t = tV;
        if (t.x > tMax.x) t.x = tMax.x;
        if (t.y > tMax.y) t.y = tMax.y;
        if (t.z > tMax.z) t.z = tMax.z;
        if (t.w > tMax.z) t.w = tMax.w;

        if (t.x < tMin.x) t.x = tMin.x;
        if (t.y < tMin.y) t.y = tMin.y;
        if (t.z < tMin.z) t.z = tMin.z;
        if (t.w < tMin.w) t.w = tMin.w;
        return t;
    }

    // Lineary interpolate between `tA` and `tB` by fraction of `tF`.
    float lerp(const float& tA, const float& tB, const float& tF) {
        return tA * (1.f - tF) + (tB * tF);
    }
    // Lineary interpolate between `tA` and `tB` by fraction of `tF`.
    glm::vec2 lerp(const glm::vec2& tA, const glm::vec2& tB, const float& tF) {
        return glm::vec2(lerp(tA.x, tB.x, tF),lerp(tA.y, tB.y, tF));
    }
    // Lineary interpolate between `tA` and `tB` by fraction of `tF`.
    glm::vec3 lerp(const glm::vec3& tA, const glm::vec3& tB, const float& tF) {
        return glm::vec3(lerp(tA.x, tB.x, tF),lerp(tA.y, tB.y, tF),lerp(tA.z, tB.z, tF));
    }
    // Lineary interpolate between `tA` and `tB` by fraction of `tF`.
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