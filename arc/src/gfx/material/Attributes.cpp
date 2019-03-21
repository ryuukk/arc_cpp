#include <cstdint>
#include "Attributes.h"

void arc::Attributes::sort() {

}

void arc::Attributes::clear() {

}

uint32_t arc::Attributes::size() {
    return attributes.size();
}

uint64_t arc::Attributes::getMask() {
    return mask;
}

void arc::Attributes::set(arc::Attribute* attribute) {
    int idx = indexOf(attribute->type);
    if (idx < 0)
    {
        enable(attribute->type);
        attributes.emplace_back(attribute);
        sorted = false;
    }
    else
    {
        attributes[idx] = attribute;
    }
    sort(); //FIXME: See #4186
}

bool arc::Attributes::has(uint64_t type) {
    return type != 0 && (mask & type) == type;
}

int arc::Attributes::indexOf(uint64_t type) {
    if (has(type))
        for (int i = 0; i < attributes.size(); i++)
            if (attributes[i]->type == type)
                return i;
    return -1;
}

void arc::Attributes::enable(uint64_t mask) {

    this->mask |= mask;
}

void arc::Attributes::disable(uint64_t mask) {

    this->mask &= ~mask;
}
