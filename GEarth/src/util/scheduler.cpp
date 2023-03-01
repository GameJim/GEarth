#include "util/scheduler.h"
#include "util/thread_local.h"
#include <assert.h>

namespace util {


    std::weak_ptr<util::Scheduler> Scheduler::makeWeakPtr()
    {
        std::weak_ptr<util::Scheduler> pWeakPtr = shared_from_this();
        return pWeakPtr;
    }

    std::function<void()> Scheduler::bindOnce(std::function<void()> fn) {
        assert(fn);
        return[scheduler = makeWeakPtr(), scheduled = std::move(fn)]() mutable {
            if (!scheduled) return; // Repeated call.
            auto schedulerGuard = scheduler.lock();
            if (auto locked = scheduler.lock()) locked->schedule(std::move(scheduled));
        };
    }

    static auto& current() {
        static util::ThreadLocal<Scheduler> scheduler;
        return scheduler;
    };

    void Scheduler::SetCurrent(Scheduler* scheduler) {
        current().set(scheduler);
    }

    Scheduler* Scheduler::GetCurrent() {
        return current().get();
    }
} //namespace mbgl




