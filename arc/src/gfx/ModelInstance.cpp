#include "ModelInstance.h"


arc::ModelInstance::ModelInstance(Model& model) : model(model) {
    copyNodes(model.nodes);
    invalidate();
    copyAnimations(model.animations);
    calculateTransforms();
}

void arc::ModelInstance::calculateTransforms() {
    auto n = nodes.size();
    for (int i = 0; i < n; ++i) {
        nodes[i]->calculateTransforms(true);
    }

    for (int j = 0; j < n; ++j) {
        nodes[j]->calculateBoneTransforms(true);
    }
}

void arc::ModelInstance::copyNodes(std::vector<arc::Node*>& nodes) {
    this->nodes.resize(nodes.size());
    for (int i = 0; i < nodes.size(); ++i) {
        auto* node = nodes[i];
        auto* copy = node->copy();
        this->nodes[i] = copy;
    }
}

void arc::ModelInstance::copyAnimations(std::vector<arc::Animation*>& animations, bool shareKeyframes) {

}

void arc::ModelInstance::invalidate() {
    for (auto& node : nodes) {
        invalidate(node);
    }
}

void arc::ModelInstance::invalidate(arc::Node* node) {
    for (int i = 0; i < node->parts.size(); ++i) {
        auto& part = node->parts[i];
        auto& bindPose = part->invBoneTransforms;
        for (int j = 0; j < bindPose.size(); ++j) {
            bindPose[j].first = Node::getNode(nodes, bindPose[j].first->id);
        }
        
        // todo: invalidate materials
    }

    for (int k = 0; k < node->children.size(); ++k) {
        invalidate(node->children[k]);
    }
}
