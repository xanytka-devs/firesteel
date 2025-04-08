#ifndef FSE_UNDO_BUFFER_H
#define FSE_UNDO_BUFFER_H

#include "../../engine/include/common.hpp"
#include "../../engine/include/entity.hpp"

using namespace Firesteel;

class UndoableAction {
public:
    virtual ~UndoableAction() = default;
    virtual void undo() = 0;
    virtual void redo() = 0;
    bool mUndone = false;
};

class PositionChangeAction : public UndoableAction {
public:
    PositionChangeAction(Entity& entity, const glm::vec3& newPosition)
        : entity(entity), oldPosition(entity.transform.Position), newPosition(newPosition) {}

    void undo() override {
        entity.transform.Position = oldPosition;
    }

    void redo() override {
        entity.transform.Position = newPosition;
    }

private:
    Entity& entity;
    glm::vec3 oldPosition;
    glm::vec3 newPosition;
};

class RotationChangeAction : public UndoableAction {
public:
    RotationChangeAction(Entity& entity, const glm::vec3& newRotation)
        : entity(entity), oldRotation(entity.transform.Rotation), newRotation(newRotation) {}

    void undo() override {
        entity.transform.Rotation = oldRotation;
    }

    void redo() override {
        entity.transform.Rotation = newRotation;
    }

private:
    Entity& entity;
    glm::vec3 oldRotation;
    glm::vec3 newRotation;
};

class SizeChangeAction : public UndoableAction {
public:
    SizeChangeAction(Entity& entity, const glm::vec3& newSize)
        : entity(entity), oldSize(entity.transform.Size), newSize(newSize) {}

    void undo() override {
        entity.transform.Size = oldSize;
    }

    void redo() override {
        entity.transform.Size = newSize;
    }

private:
    Entity& entity;
    glm::vec3 oldSize;
    glm::vec3 newSize;
};

class ActionRegistry {
public:
    void pushBack(std::shared_ptr<UndoableAction> action) {
        undoStack.push_back(action);
        redoStack.clear();
    }

    void undoLast() {
        if (!undoStack.empty()) {
            auto& action = undoStack.back();
            action->undo();
            action->mUndone = true;
            undoStack.pop_back();
            redoStack.push_back(action);
        }
    }

    void redoLast() {
        if (!redoStack.empty()) {
            auto& action = redoStack.back();
            action->redo();
            action->mUndone = false;
            redoStack.pop_back();
            undoStack.push_back(action);
        }
    }

private:
    std::vector<std::shared_ptr<UndoableAction>> undoStack;
    std::vector<std::shared_ptr<UndoableAction>> redoStack;
};

#endif // !FSE_UNDO_BUFFER_H