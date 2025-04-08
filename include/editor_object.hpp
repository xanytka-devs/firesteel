#include <../engine/include/entity.hpp>
#include "material.hpp"

struct EditorObject {
	std::string name;
	Entity entity;
	Material material;
	unsigned int materialID = 0;

	virtual void onDraw() {}
	virtual void onRemove() {}

	EditorObject() {
		name = "New Object";
		entity = Entity();
		material = Material{};
	}

	EditorObject(std::string tName, Entity tEntity, Material tMat) {
		name = tName;
		entity = tEntity;
		material = tMat;
	}

	EditorObject(std::string tName, Entity tEntity, Material tMat, unsigned int tMatID) {
		name = tName;
		entity = tEntity;
		material = tMat;
		materialID = tMatID;
	}

	void draw() {
		onDraw();
		entity.draw(&material.shader);
	}

	void draw(Material* tMat) {
		material = *tMat;
		onDraw();
		entity.draw(&tMat->shader);
	}

	void remove() {
		onRemove();
		entity.clearMeshes();
	}
};