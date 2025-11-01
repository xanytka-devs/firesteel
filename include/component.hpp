#ifndef FS_COMPONENT
#define FS_COMPONENT

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