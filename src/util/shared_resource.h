#ifndef NETREPLAY_SHARED_RESOURCE_H
#define NETREPLAY_SHARED_RESOURCE_H

#include <mutex>
#include <condition_variable>

class SharedResource {
public:
    SharedResource() = default;

    std::mutex &get_mutex();

    std::condition_variable &get_condition_var();

private:
    std::mutex m_mutex;
    std::condition_variable m_condition_var;
};

#endif
