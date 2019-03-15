#pragma once

#include <vector>
#include <string>
#include "NodePart.h"
#include "../MeshPart.h"
#include "../../math/Vec3.h"
#include "../../math/Quat.h"
#include "../../math/Mat4.h"
#include "../Mesh.h"

namespace arc
{
    class NodePart;

    class Node
    {
    public:
        std::string id;
        bool inheritTransform = true;
        bool isAnimated = false;

        Vec3 translation = {0, 0, 0};
        Quat rotation = Quat::identity();
        Vec3 scale = {1, 1, 1};

        Mat4 localTransform = Mat4::identity();
        Mat4 globalTransform = Mat4::identity();

        std::vector<NodePart*> parts;

        Node* parent;
        std::vector<Node*> children;

        void calculateLocalTransform() {
            if (!isAnimated)
                localTransform = Mat4::identity().set(translation, rotation, scale);
        }

        void calculateWorldTransform() {
            if (inheritTransform && parent != nullptr)
                globalTransform = parent->globalTransform * localTransform;
            else
                globalTransform = localTransform;
        }

        void calculateTransforms(bool recursive) {
            calculateLocalTransform();
            calculateWorldTransform();
            if (recursive) {
                for (auto& i : children) {
                    i->calculateTransforms(true);
                }
            }
        }

        int indexOf(Node* child) {
            for (int i = 0; i < children.size(); ++i) {
                if (children[i] == child)
                    return i;
            }
            return -1;
        }

        bool removeChild(Node* child) {
            int index = indexOf(child);
            if (index == -1) return false;

            children.erase(children.begin() + index);

            child->parent = nullptr;

            return true;
        }

        void detach() {
            if (parent != nullptr) {
                parent->removeChild(this);
                parent = nullptr;
            }
        }

        void set(Node* other) {
            detach();
            id = other->id;
            isAnimated = other->isAnimated;
            inheritTransform = other->inheritTransform;
            translation = other->translation;
            rotation = other->rotation;
            scale = other->scale;
            localTransform = other->localTransform;
            globalTransform = other->globalTransform;

            parts.reserve(other->parts.size());
            for (int i = 0; i < other->parts.size(); ++i) {
                NodePart* nodePart = other->parts[i];
                NodePart* copy = nodePart->copy();
                parts[i] = copy;
            }

            children.reserve(other->children.size());
            for (int i = 0; i < other->children.size(); ++i) {
                Node* child = other->children[i];
                addChild(child->copy());
            }
        }

        int addChild(Node* child) {
            for (Node* p = this; p != nullptr; p = p->parent) {
                if (p == child) throw std::invalid_argument("Cannot add a parent as a child");
            }

            Node* p = child->parent;
            if (p != nullptr && !p->removeChild(child)) {
                if (p == child)
                    throw std::invalid_argument("Could not remove child from its current parent");
            }

            children.emplace_back(child);
            child->parent = this;
            return children.size();
        }

        Node* copy() {
            Node* node = new Node();
            node->set(this);
            return node;
        }
    };

}


