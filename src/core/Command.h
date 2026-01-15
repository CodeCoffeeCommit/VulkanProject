#pragma once

#include "../world/World.h"
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <queue>
#include <mutex>

namespace libre {

    // ============================================================================
    // COMMAND BASE CLASS
    // ============================================================================

    class Command {
    public:
        virtual ~Command() = default;

        virtual void execute(World& world) = 0;
        virtual void undo(World& world) = 0;

        // Can this be merged with previous command?
        virtual bool canMergeWith(const Command& other) const { return false; }
        virtual void mergeWith(const Command& other) {}

        virtual std::string getName() const = 0;
        virtual std::string getDescription() const { return getName(); }
        virtual bool isUndoable() const { return true; }
    };

    // ============================================================================
    // CREATE ENTITY COMMAND
    // ============================================================================

    class CreateEntityCommand : public Command {
    public:
        CreateEntityCommand(const std::string& name, const std::string& type = "")
            : entityName_(name), entityType_(type) {
        }

        void execute(World& world) override {
            auto handle = world.createEntity(entityName_, entityType_);
            createdId_ = handle.getID();
        }

        void undo(World& world) override {
            world.destroyEntity(createdId_);
            createdId_ = INVALID_ENTITY;
        }

        std::string getName() const override { return "Create Entity"; }
        EntityID getCreatedEntity() const { return createdId_; }

    private:
        std::string entityName_;
        std::string entityType_;
        EntityID createdId_ = INVALID_ENTITY;
    };

    // ============================================================================
    // DELETE ENTITY COMMAND
    // ============================================================================

    class DeleteEntityCommand : public Command {
    public:
        DeleteEntityCommand(EntityID entity) : entityId_(entity) {}

        void execute(World& world) override {
            if (auto* meta = world.getMetadata(entityId_)) {
                savedName_ = meta->name;
                savedType_ = meta->type;
                savedFlags_ = meta->flags;
            }
            savedParent_ = world.getParent(entityId_);

            // Save transform
            if (auto* t = world.getComponent<TransformComponent>(entityId_)) {
                savedTransform_ = *t;
                hasTransform_ = true;
            }

            world.destroyEntity(entityId_);
        }

        void undo(World& world) override {
            auto handle = world.createEntity(savedName_, savedType_);
            entityId_ = handle.getID();

            if (auto* meta = world.getMetadata(entityId_)) {
                meta->flags = savedFlags_;
            }

            if (savedParent_ != INVALID_ENTITY) {
                world.setParent(entityId_, savedParent_);
            }

            if (hasTransform_) {
                world.addComponent<TransformComponent>(entityId_, savedTransform_);
            }
        }

        std::string getName() const override { return "Delete Entity"; }

    private:
        EntityID entityId_;
        std::string savedName_;
        std::string savedType_;
        EntityFlags savedFlags_ = EntityFlags::Default;
        EntityID savedParent_ = INVALID_ENTITY;
        TransformComponent savedTransform_;
        bool hasTransform_ = false;
    };

    // ============================================================================
    // TRANSFORM COMMAND
    // ============================================================================

    class TransformCommand : public Command {
    public:
        TransformCommand(EntityID entity,
            const glm::vec3& pos,
            const glm::quat& rot,
            const glm::vec