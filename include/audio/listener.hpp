#ifndef FS_OAL_LISTENER
#define FS_OAL_LISTENER

#include "defenitions.hpp"

namespace FSOAL {

	enum DistanceModel {
		DM_NONE = AL_NONE,

		DM_INVERSE_DISTANCE = AL_INVERSE_DISTANCE,
		DM_INVERSE_DISTANCE_CLAMP = AL_INVERSE_DISTANCE_CLAMPED,

		DM_LINEAR = AL_LINEAR_DISTANCE,
		DM_LINEAR_CLAMP = AL_LINEAR_DISTANCE_CLAMPED,

		DM_EXPONENTIAL = AL_EXPONENT_DISTANCE,
		DM_EXPONENTIAL_CLAMP = AL_EXPONENT_DISTANCE_CLAMPED
	};

	struct Listener {
	public:
		static void setPosition(glm::vec3 tPos) {
			if(!globalInitState) return;
			alListener3f(AL_POSITION, tPos.x, tPos.y, tPos.z);
		}
		static void setRotation(glm::vec3 tForward, glm::vec3 tUp) {
			if(!globalInitState) return;
			float orientation[6]{
				tForward.x, tForward.y, tForward.z,
				tUp.x, tUp.y, tUp.z
			};

			alListenerfv(AL_ORIENTATION, orientation);
		}
		static void setDistanceModel(DistanceModel tMode) {
			if(!globalInitState) return;
			alDistanceModel(tMode);
		}
	};

}

#endif // !FS_OAL_LISTENER
