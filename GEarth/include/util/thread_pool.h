/*!
*@File    thread_pool.h
*@Brief   文件描述:下次一定
*@Date    2023/2/10
*/
#pragma once


#include <util/mailbox.h>
#include <util/scheduler.h>

#include <array>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <assert.h>

#include <util/util_export.h>
namespace util {

    class UTIL_API ThreadedSchedulerBase : public Scheduler {
    public:
        void schedule(std::function<void()>) override;

    protected:
        ThreadedSchedulerBase() = default;
        ~ThreadedSchedulerBase() override;

        void terminate();
        std::thread makeSchedulerThread(size_t index);

        std::queue<std::function<void()>> queue;
        std::mutex mutex;
        std::condition_variable cv;
        bool terminated{ false };
    };

   
    template <std::size_t N>
    class ThreadedScheduler : public ThreadedSchedulerBase {
    public:
        ThreadedScheduler() {
            for (std::size_t i = 0u; i < N; ++i) {
                threads[i] = makeSchedulerThread(i);
                threads[i].detach();
            }
        }

        ~ThreadedScheduler() override {
            terminate();
            for (auto& thread : threads) {
                assert(std::this_thread::get_id() != thread.get_id());
                thread.join();
            }
        }

    private:
        std::array<std::thread, N> threads;
        static_assert(N > 0, "Thread count must be more than zero.");
    };

    class SequencedScheduler : public ThreadedScheduler<1> {};

    template <std::size_t extra>
    using ParallelScheduler = ThreadedScheduler<1 + extra>;

    class UTIL_API ThreadPool : public ParallelScheduler<3> {};

} 

