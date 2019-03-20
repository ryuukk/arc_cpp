#pragma once


#include "Attribute.h"

namespace arc
{
    class Attributes
    {
    public:
        void set(Attribute* attribute);
        bool has(uint64_t type);
        int indexOf(uint64_t type);

        template <class T>
        T* get(uint64_t type);

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


