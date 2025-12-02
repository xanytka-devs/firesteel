#ifndef FS_NO_COMPONENTS
#ifndef FS_COMPONENT
#define FS_COMPONENT

#include <type_traits>
#ifndef FS_NO_JSON
#include "utils/json.hpp"
#endif // !FS_NO_JSON

namespace Firesteel {
	class Entity;
	class Component {
	public:
		Component(Entity* tEntity)
			: mEntity(tEntity) {}
		virtual void onStart()=0;
		virtual void onUpdate()=0;
		virtual void onDraw()=0;
		virtual void onRemove()=0;
#ifndef FS_NO_JSON
		virtual nlohmann::json serialize() const=0;
#endif // !FS_NO_JSON

		bool isInitialized() const { return mInitialized; }
	protected:
		Entity* mEntity;
		bool mInitialized=false;
	};
}

#endif // !FS_COMPONENT
#endif // !FS_NO_COMPONENTS