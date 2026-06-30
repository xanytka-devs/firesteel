#ifndef FS_UTIL_GEOMETRY_H
#define FS_UTIL_GEOMETRY_H

#include <vector>
#include <firesteel/rendering/mesh.hpp>

namespace Firesteel {
	namespace Geometry {
		Mesh quad(const glm::vec2& tSize = { 1,1 }) {
			std::vector<Vertex> vertices;
			const static float pos[] = {
				0.0f, 1.0f,
				1.0f, 0.0f,
				0.0f, 0.0f,
				1.0f, 1.0f,
            };
			for (uint i = 0; i < 4; i++) {
				Vertex vert{};
				vert.position={ pos[i*2+0]*tSize.x, pos[i*2+1]*tSize.y, 0.f };
				vert.uv={ pos[i*2+0], pos[i*2+1] };
				vertices.push_back(vert);
			}
			return Mesh(vertices, { 0,1,2,1,3,2 }, nullptr);
		}
	}
}

#endif // !FS_UTIL_GEOMETRY_H