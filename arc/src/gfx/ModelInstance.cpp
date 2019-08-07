#include <algorithm>
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

arc::Animation* arc::ModelInstance::getAnimation(const std::string& id){
    for(auto* anim : animations)
        if(anim->id == id) return anim;

    return nullptr;
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
   for(auto& modelAnim : animations)
   {
       auto* animation = new Animation;
       animation->id = modelAnim->id;
       animation->duration = modelAnim->duration;
       for(auto& nanim : modelAnim->nodeAnimations)
       {
           auto* node = Node::getNode(nodes, nanim->node->id);
           if(node == nullptr)
               continue;
           auto* nodeAnim = new NodeAnimation;
           nodeAnim->node = node;
           if(shareKeyframes)
           {
               // todo: fix sharing keyframes, reference should be copied correctly
               nodeAnim->translation = nanim->translation;
               nodeAnim->rotation = nanim->rotation;
               nodeAnim->scaling = nanim->scaling;
           } else
           {
               // todo: should figure out why i can't resize the vectors.. i should avoid emplace_back if i already know the final size
               if(!nanim->translation.empty())
               {
                   //nodeAnim->translation.resize(nanim->translation.size());
                   for (int i = 0; i < nanim->translation.size(); ++i) {
                       auto& kf = nanim->translation[i];
                       if(kf.keytime > animation->duration) animation->duration = kf.keytime;
                       auto kff = NodeKeyframe<Vec3>{kf.keytime, kf.value};
                       nodeAnim->translation.emplace_back(kff);
                   }
               }
               if(!nanim->rotation.empty())
               {
                   //nodeAnim->rotation.resize(nanim->rotation.size());
                   for (int i = 0; i < nanim->rotation.size(); ++i) {
                       auto& kf = nanim->rotation[i];
                       if(kf.keytime > animation->duration) animation->duration = kf.keytime;
                       auto kff = NodeKeyframe<Quat>{kf.keytime, kf.value};
                       nodeAnim->rotation.emplace_back(kff);
                   }
               }
               if(!nanim->scaling.empty())
               {
                   //nodeAnim->scaling.resize(nanim->scaling.size());
                   for (int i = 0; i < nanim->scaling.size(); ++i) {
                       auto& kf = nanim->scaling[i];
                       if(kf.keytime > animation->duration) animation->duration = kf.keytime;
                       auto kff = NodeKeyframe<Vec3>{kf.keytime, kf.value};
                       nodeAnim->scaling.emplace_back(kff);
                   }
               }



               if ((!nodeAnim->translation.empty())
                   || (!nodeAnim->rotation.empty())
                   || (!nodeAnim->scaling.empty()))
                   animation->nodeAnimations.emplace_back(nodeAnim);
           }
       }

       if(!animation->nodeAnimations.empty())
           this->animations.emplace_back(animation);
   }
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
        
        if ( std::find(materials.begin(), materials.end(), part->material) == materials.end() )
        {
            int midx = -1;
            for (int j = 0; j < materials.size(); ++j) {
                if(materials[j] == part->material)
                {
                    midx = j;
                    break;
                }
            }

            if(midx < 0)
                materials.emplace_back(part->material = part->material->copy());
            else
                part->material = materials[midx];
        }
    }

    for (int k = 0; k < node->children.size(); ++k) {
        invalidate(node->children[k]);
    }
}

void arc::ModelInstance::getRenderables(DynamicObjectPool<Renderable>& pool, std::vector<Renderable*>& renderables) {
    for(auto& node : nodes)
    {
        for(auto& part : node->parts)
        {
            if(part->enabled == false) continue;

            Renderable* renderable = pool.new_object();
            // clean it here, remove when pool is fixed
            renderable->environement = nullptr;
            renderable->material = nullptr;
            renderable->meshPart.set("", nullptr, 0, 0, 0);
            renderable->shader = nullptr;
            renderable->bones = nullptr;
            renderable->worldTransform = Mat4::identity();
            // --

            renderable->material = part->material;
            renderable->bones = &part->bones;
            renderable->meshPart.set(part->meshPart);

            if(part->bones.empty())
                renderable->worldTransform = transform * node->globalTransform;
            else
                renderable->worldTransform = transform;

            renderables.emplace_back(renderable);
        }
    }
}
