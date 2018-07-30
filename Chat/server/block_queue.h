#pragma once
#include <vector>
#include <semaphore.h>
//实现一个阻塞队列

namespace server
{

//类模板
template<typename T>
class BlockQueue
{
public:
    BlockQueue(size_t s = 1024)
        :data(s)
        ,_head(0)
        ,_tail(0)
        ,_size(0)
    {
        sem_init(&sem_lock,0,1);
        sem_init(&sem_data,0,0);
        sem_init(&sem_blank,0,s);
    }

    void PushBack(const T& value)
    {
        sem_wait(&sem_blank);
        sem_wait(&sem_lock);
        data[_tail++] = value;
        if(_tail >= data.size())
        {
            _tail = 0;
        }
        ++_size;
        sem_post(&sem_lock);
        sem_post(&sem_data);
    }

    void PopFront(T* value)
    {
        sem_wait(&sem_data);
        sem_wait(&sem_lock);
        *value = data[_head++];
        if(_head >= data.size())
        {
            _head = 0;
        }
        --_size;
        sem_post(&sem_lock);
        sem_post(&sem_blank);
    }

    ~BlockQueue()
    {
        sem_destroy(&sem_data);
        sem_destroy(&sem_lock);
        sem_destroy(&sem_blank);
    }
        
private:
    std::vector<T> data;
    sem_t sem_data;     //描述元素
    sem_t sem_blank;    //描述空格
    sem_t sem_lock;      //可以不加互斥锁（现在只是单生产者，单消费者模型）
    size_t _head;
    size_t _tail;
    size_t _size;
};

}
