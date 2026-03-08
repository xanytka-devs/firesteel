#ifndef FS_COMP_ANNOUNCER_H
#define FS_COMP_ANNOUNCER_H

#include <firesteel/utils/log.hpp>
#include <firesteel/component.hpp>

namespace Firesteel {
	class AnnouncerComponent : public Component {
	public:
		AnnouncerComponent(std::shared_ptr<Entity> tEntity, const std::string& tVal="Hello World!")
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

#endif // !FS_COMP_ANNOUNCER_H