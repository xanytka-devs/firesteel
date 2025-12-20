#ifndef FS_DELTA_TIME
#define FS_DELTA_TIME

#include <firesteel/common.hpp>

namespace Firesteel {
    struct DeltaTime {
    public:
        static float get() {
            return sInstance()->mDeltaTime;
        }
        void set(const float& tDt) {
            mDeltaTime = tDt;
        }
        static uint fps() {
            return sInstance()->mFps;
        }
        void setFps(const uint& tFps) {
            mFps = tFps;
        }
        static DeltaTime* sInstance() {
            static DeltaTime instance;
            return &instance;
        }
    private:
        float mDeltaTime = 0.01f;
        uint mFps = 0;
    };
}

#endif // !FS_DELTA_TIME