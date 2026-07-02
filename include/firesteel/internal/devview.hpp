#ifndef FS_INTERNAL_DEVVIEW_H
#define FS_INTERNAL_DEVVIEW_H

namespace Firesteel {
	namespace DEVVIEW {
		inline bool sDrawDevView;
		void draw(const float tDeltaTime, const unsigned int tFPS);
	}
}

#endif // !FS_INTERNAL_DEVVIEW_H