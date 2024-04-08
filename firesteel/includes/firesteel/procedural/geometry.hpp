#ifndef PROCEDURAL_GEOMETRY_H
#define PROCEDURAL_GEOMETRY_H

#include "firesteel/rendering/mesh.hpp"

namespace firesteel {

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
            return firesteel::Mesh(firesteel::Vertex::generate_list(vertices, 6), indicies);
        }

		static Mesh Cube() {
            //Shortcuts.
            float x = 0.5f;
            float y = 0.5f;
            float z = 0.5f;
            //Main body.
            float vertices[] = {
                //   POSITIONS            NORMAL              UVs
                -x, -y, -z,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                 x, -y, -z,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
                 x,  y, -z,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                 x,  y, -z,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                -x,  y, -z,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
                -x, -y, -z,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

                -x, -y,  z,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                 x, -y,  z,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
                 x,  y,  z,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                 x,  y,  z,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                -x,  y,  z,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
                -x, -y,  z,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

                -x,  y,  z, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                -x,  y, -z, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                -x, -y, -z, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -x, -y, -z, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -x, -y,  z, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                -x,  y,  z, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

                 x,  y,  z,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                 x,  y, -z,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                 x, -y, -z,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                 x, -y, -z,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                 x, -y,  z,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                 x,  y,  z,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

                -x, -y, -z,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
                 x, -y, -z,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
                 x, -y,  z,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                 x, -y,  z,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                -x, -y,  z,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
                -x, -y, -z,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

                -x,  y, -z,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 x,  y, -z,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
                 x,  y,  z,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                 x,  y,  z,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                -x,  y,  z,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
                -x,  y, -z,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
            };
            //Indicies.
            std::vector<unsigned int> indicies(36);
            for (unsigned int i = 0; i < 36; i++)
                indicies[i] = i;
            return firesteel::Mesh(firesteel::Vertex::generate_list(vertices, 36), indicies);
		}
	};

}

#endif // PROCEDURAL_GEOMETRY_H