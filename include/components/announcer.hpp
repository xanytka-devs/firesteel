#ifndef FS_COMP_ANNOUNCER
#define FS_COMP_ANNOUNCER

#include "../utils/log.hpp"
#include "../component.hpp"

namespace Firesteel {
	class Announcer : public Component {
	public:
		Announcer(Entity* tEntity, const std::string& tVal)
			: value(tVal), Component(tEntity) { }
		virtual void onUpdate() override {
			LOG_INFO("Announcer: "+value);
		}

		std::string value;
	};
}

#endif // !FS_COMP_ANNOUNCER