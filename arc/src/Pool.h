#pragma once


#include <cstdint>
#include <vector>
#include <queue>
#include <stdexcept>
#include <algorithm>

namespace arc
{
    class IPoolable
    {
    public:
        virtual void free() = 0;
    };

    template <typename T>
    class Pool
    {
    public:
        Pool(uint32_t initialCapacity = 16*16, uint32_t max = 1000) : max(max)
        {
            //_freeObjects.resize(initialCapacity); // todo: std::queue can't be resized :(

            for (int i = 0; i < initialCapacity; ++i) {
                _freeObjects.push(new T());
            }
        }

        ~Pool()
        {
            while (_freeObjects.size() != 0)
            {
                delete _freeObjects.front();
                _freeObjects.pop();
            }
        }

        virtual T* obtain()
        {
            if(_freeObjects.empty())
                return newObject();

            auto* ret = _freeObjects.front();
            _freeObjects.pop();
            return ret;
        }

        void free(T* object)
        {
            if(object == nullptr) throw std::invalid_argument("can't free null object"); // todo: figure out exceptions

            if (_freeObjects.size() < max) {
                _freeObjects.push(object);
                peak = std::max(peak, _freeObjects.size);
            }
            reset(object);
        }

        void freeAll(std::vector<T*>& objects)
        {
            for (int i = 0; i < objects.size(); i++) {
                auto& object = objects[i];
                if (object == nullptr) continue;
                if (_freeObjects.size() < max)
                    _freeObjects.push(object);
                reset(object);
            }
            peak = std::max(peak, (uint32_t) _freeObjects.size());
        }

        void clear()
        {
            while (_freeObjects.size() != 0)
            {
                delete _freeObjects.front();
                _freeObjects.pop();
            }
        }

    protected:
        virtual T* newObject() = 0;

        void reset(T* object)
        {
            //if (auto* poolable = dynamic_cast<IPoolable*>(object)) {
            //    poolable->free();
            //}
        }

        uint32_t max{};
        uint32_t peak{};
    private:
        std::queue<T*> _freeObjects{};
    };

    template <typename T>
    class FlushablePool : public Pool<T>
    {
        // really needed ? hmm
    };
}