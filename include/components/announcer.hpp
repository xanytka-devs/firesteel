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

#ifndef FS_NO_JSON
		virtual nlohmann::json serialize() const override {
			return {
				{"type", "Announcer"},
				{"value", value}
			};
		}
		static std::shared_ptr<Announcer> deserialize(Entity* tEntity, const nlohmann::json& tData) {
			return std::make_shared<Announcer>(tEntity, tData["value"].get<std::string>());
		}
#endif // !FS_NO_JSON

		std::string value;
	};
}

#endif // !FS_COMP_ANNOUNCER