#include "Material.h"

arc::Material::Material(arc::Attribute* attribute) {
    set(attribute);
}

arc::Material* arc::Material::copy() {
    auto* ret = new Material;
    ret->id = id;
    for(auto& attr : attributes)
    {
        ret->set(attr->copy());
    }
    return ret;
}
