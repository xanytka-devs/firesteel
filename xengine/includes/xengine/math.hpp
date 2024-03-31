//Includes GLM without need to add it to CMake project.
#include "../../external/glm/glm/glm.hpp"
#include "../../external/glm/glm/gtc/type_ptr.hpp"

#ifndef MATH_H
#define MATH_H

namespace XEngine {

#define iVec1 int
#define iVector1 iVec1
#define Vec1 float
#define Vector1 Vec1
#define dVec1 double
#define dVector1 dVec1

	/// <summary>
	/// Vector2 with specified type.
	/// </summary>
	/// <typeparam name="T">Storage type.</typeparam>
	template<typename T>
	struct vec2 {
		union { T x, r, s; };
		union { T y, g, t; };

		vec2(T t_v) {
			x = y = t_v;
		}
		vec2(T t_x, T t_y) {
			x = t_x;
			y = t_y;
		}

		vec2 operator+(vec2 t) {
			x += t.x;
			y += t.y;
		}
		vec2 operator-(vec2 t) {
			x -= t.x;
			y -= t.y;
		}
		vec2 operator*(vec2 t) {
			x *= t.x;
			y *= t.y;
		}
		vec2 operator/(vec2 t) {
			x /= t.x;
			y /= t.y;
		}

		vec2 operator+=(vec2 t) {
			x += t.x;
			y += t.y;
		}
		vec2 operator-=(vec2 t) {
			x -= t.x;
			y -= t.y;
		}
		vec2 operator*=(vec2 t) {
			x *= t.x;
			y *= t.y;
		}
		vec2 operator/=(vec2 t) {
			x /= t.x;
			y /= t.y;
		}

		bool operator==(vec2 t) {
			return (x == t.x) && (y == t.y);
		}
		bool operator!=(vec2 t) {
			return !operator==(t);
		}
	};

#define iVec2 vec2<int>
#define iVector2 iVec2
#define Vec2 vec2<float>
#define Vector2 Vec2
#define dVec2 vec2<double>
#define dVector2 dVec2

	/// <summary>
	/// Vector3 with specified type.
	/// </summary>
	/// <typeparam name="T">Storage type.</typeparam>
	template<typename T>
	struct vec3 {
		union { T x, r, s; };
		union { T y, g, t; };
		union { T z, b, p; };

		vec3(T t_v) {
			x = y = z = t_v;
		}
		vec3(T t_x, T t_y, T t_z) {
			x = t_x;
			y = t_y;
			z = t_z;
		}

		vec3 operator+(vec3 t) {
			x += t.x;
			y += t.y;
			z += t.z;
		}
		vec3 operator-(vec3 t) {
			x -= t.x;
			y -= t.y;
			z -= t.z;
		}
		vec3 operator*(vec3 t) {
			x *= t.x;
			y *= t.y;
			z *= t.z;
		}
		vec3 operator/(vec3 t) {
			x /= t.x;
			y /= t.y;
			z /= t.z;
		}

		vec3 operator+=(vec3 t) {
			x += t.x;
			y += t.y;
			z += t.z;
		}
		vec3 operator-=(vec3 t) {
			x -= t.x;
			y -= t.y;
			z -= t.z;
		}
		vec3 operator*=(vec3 t) {
			x *= t.x;
			y *= t.y;
			z *= t.z;
		}
		vec3 operator/=(vec3 t) {
			x /= t.x;
			y /= t.y;
			z /= t.z;
		}

		bool operator==(vec3 t) {
			return (x == t.x) && (y == t.y) && (z == t.z);
		}
		bool operator!=(vec3 t) {
			return !operator==(t);
		}
	};

#define iVec3 vec3<int>
#define iVector3 iVec3
#define Vec3 vec3<float>
#define Vector3 Vec3
#define dVec3 vec3<double>
#define dVector3 dVec3

	/// <summary>
	/// Vector4 with specified type.
	/// </summary>
	/// <typeparam name="T">Storage type.</typeparam>
	template<typename T>
	struct vec4 {
		union { T x, r, s; };
		union { T y, g, t; };
		union { T z, b, p; };
		union { T w, a, q; };

		vec4(T t_v) {
			x = y = z = w = t_v;
		}
		vec4(T t_x, T t_y, T t_z, T t_w) {
			x = t_x;
			y = t_y;
			z = t_z;
			w = t_w;
		}

		vec4 operator+(vec4 t) {
			x += t.x;
			y += t.y;
			z += t.z;
			w += t.w;
		}
		vec4 operator-(vec4 t) {
			x -= t.x;
			y -= t.y;
			z -= t.z;
			w -= t.w;
		}
		vec4 operator*(vec4 t) {
			x *= t.x;
			y *= t.y;
			z *= t.z;
			w *= t.w;
		}
		vec4 operator/(vec4 t) {
			x /= t.x;
			y /= t.y;
			z /= t.z;
			w /= t.w;
		}

		vec4 operator+=(vec4 t) {
			x += t.x;
			y += t.y;
			z += t.z;
			w += t.w;
		}
		vec4 operator-=(vec4 t) {
			x -= t.x;
			y -= t.y;
			z -= t.z;
			w -= t.w;
		}
		vec4 operator*=(vec4 t) {
			x *= t.x;
			y *= t.y;
			z *= t.z;
			w *= t.w;
		}
		vec4 operator/=(vec4 t) {
			x /= t.x;
			y /= t.y;
			z /= t.z;
			w /= t.w;
		}

		bool operator==(vec4 t) {
			return (x == t.x) && (y == t.y) && (z == t.z) && (w == t.w);
		}
		bool operator!=(vec4 t) {
			return !operator==(t);
		}
	};

#define iVec4 vec4<int>
#define iVector4 iVec4
#define Vec4 vec4<float>
#define Vector4 Vec4
#define dVec4 vec4<double>
#define dVector4 dVec4

	class Rand {
		/// <summary>
		/// Gets random number.
		/// </summary>
		/// <returns>Random integer.</returns>
		static int get_int() {
			return rand();
		}
		/// <summary>
		/// Gets random number.
		/// </summary>
		/// <param name="t_seed">Randomization base.</param>
		/// <returns>Random integer (depending on seed).</returns>
		static int get_int_s(int t_seed) {
			srand(t_seed);
			return rand();
		}
		/// <summary>
		/// Gets random number between two values.
		/// </summary>
		/// <param name="t_min">Minimum value.</param>
		/// <param name="t_max">Maximum value.</param>
		/// <returns>Random integer.</returns>
		static int get_int(int t_min, int t_max) {
			return rand() % (t_max - t_min + 1) + t_min;
		}
		/// <summary>
		/// Gets random number between two values.
		/// </summary>
		/// <param name="t_seed">Randomization base.</param>
		/// <param name="t_min">Minimum value.</param>
		/// <param name="t_max">Maximum value.</param>
		/// <returns>Random integer (depending on seed).</returns>
		static int get_int(int t_seed, int t_min, int t_max) {
			srand(t_seed);
			return rand() % (t_max - t_min + 1) + t_min;
		}
	};
}

#endif // MATH_H