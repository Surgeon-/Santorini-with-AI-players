#pragma once

#pragma once

#include <mutex>
#include <condition_variable>

namespace etf_santorini_bj150087d {

    namespace util {

        class Semaphore {

        public:

            Semaphore(unsigned count = 0) {
                m_count = count;
            }

            void signal() {
                std::unique_lock<decltype(m_mutex)> lock(m_mutex);
                m_count += 1;
                m_condition.notify_one();
            }

            void wait() {
                std::unique_lock<decltype(m_mutex)> lock(m_mutex);
                while (!m_count)
                    m_condition.wait(lock);
                m_count -= 1;
            }

            bool try_wait() {
                std::unique_lock<decltype(m_mutex)> lock(m_mutex);
                if (m_count) {
                    m_count -= 1;
                    return true;
                }
                return false;
            }

            unsigned val() const {
                return m_count;
            }

        private:

            std::mutex m_mutex;
            std::condition_variable m_condition;
            unsigned m_count;

        };

    }

}