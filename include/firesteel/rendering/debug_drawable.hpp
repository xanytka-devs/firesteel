#ifndef FS_DEBUG_DRAWABLE_H
#define FS_DEBUG_DRAWABLE_H

#include <firesteel/enviroment.hpp>
using namespace Firesteel;

class IDebugDrawable {
public:
	void debugDraw2D(glm::vec2 pos, glm::vec2 size, glm::vec4 color, bool dot) {
		if(dot) Enviroment::getRenderer()->drawPoint(glm::vec3(pos,0), color);
		else {
			Enviroment::getRenderer()->drawLine({pos.x-size.x, pos.y-size.y, 0}, {pos.x+size.x, pos.y-size.y, 0}, color);
			Enviroment::getRenderer()->drawLine({pos.x+size.x, pos.y-size.y, 0}, {pos.x+size.x, pos.y+size.y, 0}, color);
			Enviroment::getRenderer()->drawLine({pos.x+size.x, pos.y+size.y, 0}, {pos.x-size.x, pos.y+size.y, 0}, color);
			Enviroment::getRenderer()->drawLine({pos.x-size.x, pos.y+size.y, 0}, {pos.x-size.x, pos.y-size.y, 0}, color);
		}
	}
};

#endif // !FS_DEBUG_DRAWABLE_H