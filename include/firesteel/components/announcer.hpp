#ifndef FS_COMP_ANNOUNCER
#define FS_COMP_ANNOUNCER

#include <firesteel/utils/log.hpp>
#include <firesteel/component.hpp>

namespace Firesteel {
	class Announcer : public Component {
	public:
		Announcer(Entity* tEntity, const std::string& tVal="Hello World!")
			: value(tVal), Component(tEntity) { }
		void onUpdate() override {
			LOG_INFO("fs.announcer: "+value);
		}

		void properties() override {
			PROPERTY(value);
		}
		const char* name() const override { return "fs.announcer"; }

		std::string value;
	};
}

#endif // !FS_COMP_ANNOUNCER