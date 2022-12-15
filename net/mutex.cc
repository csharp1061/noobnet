#include "mutex.h"

namespace noobnet {

Semophore::Semophore(uint32_t count) {
    if (sem_init(&m_semophore, 0, count)) {
        throw std::logic_error("sem_init error");
    }
}

Semophore::~Semophore() {
    sem_destroy(&m_semophore);
}

void Semophore::wait() {
    if (sem_wait(&m_semophore)) {
        throw std::logic_error("sem_wait error");
    }
}

void Semophore::notify() {
    if (sem_post(&m_semophore)) {
        throw std::logic_error("sem_post error");
    }
}

} // noobnet