#ifndef FS_NO_COMPONENTS
#ifndef FS_COMPONENT
#define FS_COMPONENT

#include <type_traits>

namespace Firesteel {
	class Entity;
	class Component {
	public:
		Component(Entity* tEntity)
			: mEntity(tEntity) {}
		virtual void onStart() {}
		virtual void onUpdate() {}
		virtual void onDraw() {}
		virtual void onRemove() {}

		bool isInitialized() const { return mInitialized; }
	protected:
		Entity* mEntity;
		bool mInitialized=false;
	};
}

#endif // !FS_COMPONENT
#endif // !FS_NO_COMPONENTS