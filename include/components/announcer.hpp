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
			LOG_INFO("fs.announcer: "+value);
		}

#ifndef FS_NO_JSON
		virtual nlohmann::json serialize() const override {
			return {
				{"type", getName()},
				{"value", value}
			};
		}
		static std::shared_ptr<Announcer> deserialize(Entity* tEntity, const nlohmann::json& tData) {
			std::string val="Hello there!";
			if(tData.contains("value")) val=tData["value"];
			return std::make_shared<Announcer>(tEntity, val);
		}
#endif // !FS_NO_JSON

		std::string value;

		virtual const char* getName() const override { return "fs.announcer"; }
	};
}

#endif // !FS_COMP_ANNOUNCER