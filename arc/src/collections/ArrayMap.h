#pragma once

#include <vector>

namespace arc
{
    template<class K, class V>
    class ArrayMap
    {
    public:
        std::vector<K*> keys;
        std::vector<V*> values;
    };
}