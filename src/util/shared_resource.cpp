#include "shared_resource.h"

std::mutex &SharedResource::get_mutex() {
    return m_mutex;
}

std::condition_variable &SharedResource::get_condition_var() {
    return m_condition_var;
}
