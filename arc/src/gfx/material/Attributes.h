#pragma once


#include "Attribute.h"

namespace arc
{
    class Attributes
    {
    public:
        ~Attributes()
        {
            for(auto& attr : attributes)
                delete attr;
        }

        void set(Attribute* attribute);
        bool has(uint64_t type);
        int indexOf(uint64_t type);

        template <typename T>
        T* get(uint64_t type) {
            int index = indexOf(type);
            if (index == -1)
                return nullptr;

            return (T*) attributes[index];
        }

        void sort();
        void clear();
        uint32_t size();
        uint64_t getMask();
        std::vector<Attribute*> attributes;
    protected:
        uint64_t mask{};
        bool  sorted = true;

    private:
        void enable(uint64_t mask);
        void disable(uint64_t mask);
    };
}


