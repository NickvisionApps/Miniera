#ifndef BOUNDEDQUEUE_H
#define BOUNDEDQUEUE_H

#include <queue>

namespace Nickvision::Miniera::Shared::Models
{
    template <typename T>
    class BoundedQueue : public std::queue<T>
    {
    public:
        BoundedQueue(size_t maxSize)
            : m_maxSize{ maxSize }
        {

        }

        size_t max_size() const
        {
            return m_maxSize;
        }

        void push(const T& value)
        {
            if(this->size() == m_maxSize)
            {
                this->pop();
            }
            this->c.push_back(value);
        }

        void push(T&& value)
        {
            if(this->size() == m_maxSize)
            {
                this->pop();
            }
            this->c.push_back(std::move(value));
        }

        template<typename... Args>
        decltype(auto) emplace(Args&&... args)
        {
            if(this->size() == m_maxSize)
            {
                this->pop();
            }
            return this->c.emplace_back(std::forward<Args>(args)...);
        }

    private:
        size_t m_maxSize;
    };
}

#endif //BOUNDEDSTACK_H
