#ifndef PROCEDURAL_GEOMETRY_H
#define PROCEDURAL_GEOMETRY_H

#include "xengine/rendering/mesh.hpp"

namespace XEngine {

	class Geometry {
	public:
        static Mesh Plane(glm::vec2 t_size = glm::vec3(1.f), glm::vec3 t_center = glm::vec3(0.f)) {
            //Shortcuts.
            float x = t_size.x;
            float z = t_size.y;
            float c_x = t_center.x;
            float c_z = t_center.y;
            //Main body.
            float vertices[] = {
                //       POSITIONS                         NORMAL              UVs
                (-1.f+c_x)*x, 0.f, (-1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
                ( 1.f+c_x)*x, 0.f, (-1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
                ( 1.f+c_x)*x, 0.f, ( 1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
                ( 1.f+c_x)*x, 0.f, ( 1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
                (-1.f+c_x)*x, 0.f, ( 1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
                (-1.f+c_x)*x, 0.f, (-1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f
            };
            //Indicies.
            std::vector<unsigned int> indicies(6);
            for (unsigned int i = 0; i < 6; i++)
                indicies[i] = i;
            return XEngine::Mesh(XEngine::Vertex::generate_list(vertices, 6), indicies);
        }

		static Mesh Cube(glm::vec3 t_size = glm::vec3(1.f), glm::vec3 t_center = glm::vec3(0.f)) {
            //Shortcuts.
            float x = t_size.x;
            float y = t_size.y;
            float z = t_size.z;
            float c_x = t_center.x;
            float c_y = t_center.y;
            float c_z = t_center.z;
            //Main body.
            float vertices[] = {
                //       POSITIONS                                  NORMAL              UVs
                (-1.f+c_x)*x, (-1.f+c_y)*y, (-1.f+c_z)*z,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
                ( 1.f+c_x)*x, (-1.f+c_y)*y, (-1.f+c_z)*z,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
                ( 1.f+c_x)*x, ( 1.f+c_y)*y, (-1.f+c_z)*z,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
                ( 1.f+c_x)*x, ( 1.f+c_y)*y, (-1.f+c_z)*z,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
                (-1.f+c_x)*x, ( 1.f+c_y)*y, (-1.f+c_z)*z,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
                (-1.f+c_x)*x, (-1.f+c_y)*y, (-1.f+c_z)*z,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

                (-1.f+c_x)*x, (-1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
                ( 1.f+c_x)*x, (-1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
                ( 1.f+c_x)*x, ( 1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
                ( 1.f+c_x)*x, ( 1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
                (-1.f+c_x)*x, ( 1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
                (-1.f+c_x)*x, (-1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,

                (-1.f+c_x)*x, ( 1.f+c_y)*y, ( 1.f+c_z)*z,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
                (-1.f+c_x)*x, ( 1.f+c_y)*y, (-1.f+c_z)*z,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
                (-1.f+c_x)*x, (-1.f+c_y)*y, (-1.f+c_z)*z,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
                (-1.f+c_x)*x, (-1.f+c_y)*y, (-1.f+c_z)*z,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
                (-1.f+c_x)*x, (-1.f+c_y)*y, ( 1.f+c_z)*z,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
                (-1.f+c_x)*x, ( 1.f+c_y)*y, ( 1.f+c_z)*z,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

                ( 1.f+c_x)*x, ( 1.f+c_y)*y, ( 1.f+c_z)*z,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
                ( 1.f+c_x)*x, ( 1.f+c_y)*y, (-1.f+c_z)*z,     1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
                ( 1.f+c_x)*x, (-1.f+c_y)*y, (-1.f+c_z)*z,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
                ( 1.f+c_x)*x, (-1.f+c_y)*y, (-1.f+c_z)*z,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
                ( 1.f+c_x)*x, (-1.f+c_y)*y, ( 1.f+c_z)*z,     1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
                ( 1.f+c_x)*x, ( 1.f+c_y)*y, ( 1.f+c_z)*z,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

                (-1.f+c_x)*x, (-1.f+c_y)*y, (-1.f+c_z)*z,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
                ( 1.f+c_x)*x, (-1.f+c_y)*y, (-1.f+c_z)*z,     0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
                ( 1.f+c_x)*x, (-1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
                ( 1.f+c_x)*x, (-1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
                (-1.f+c_x)*x, (-1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
                (-1.f+c_x)*x, (-1.f+c_y)*y, (-1.f+c_z)*z,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

                (-1.f+c_x)*x, ( 1.f+c_y)*y, (-1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
                ( 1.f+c_x)*x, ( 1.f+c_y)*y, (-1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
                ( 1.f+c_x)*x, ( 1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
                ( 1.f+c_x)*x, ( 1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
                (-1.f+c_x)*x, ( 1.f+c_y)*y, ( 1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
                (-1.f+c_x)*x, ( 1.f+c_y)*y, (-1.f+c_z)*z,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f
            };
            //Indicies.
            std::vector<unsigned int> indicies(36);
            for (unsigned int i = 0; i < 36; i++)
                indicies[i] = i;
            return XEngine::Mesh(XEngine::Vertex::generate_list(vertices, 36), indicies);
		}
	};

}

#endif // PROCEDURAL_GEOMETRY_H