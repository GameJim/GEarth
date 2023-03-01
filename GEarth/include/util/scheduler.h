/*!
*@File    scheduler.h
*@Brief   文件描述:下次一定
*@Date    2023/2/10
*/
#pragma once

#include <functional>
#include <memory>
#include <util/util_export.h>
namespace util {

    class Mailbox;

    class UTIL_API Scheduler : public std::enable_shared_from_this<Scheduler> {
    public:
        virtual ~Scheduler() = default;

        virtual void schedule(std::function<void()>) = 0;
        // Makes a weak pointer to this Scheduler.
        virtual std::weak_ptr<Scheduler> makeWeakPtr();

        std::function<void()> bindOnce(std::function<void()>);

        template <typename TaskFn, typename ReplyFn>
        void scheduleAndReplyValue(const TaskFn& task, const ReplyFn& reply) {
            assert(GetCurrent());
            scheduleAndReplyValue(task, reply, GetCurrent()->makeWeakPtr());
        }

        static Scheduler* GetCurrent();
        static void SetCurrent(Scheduler*);

    protected:
        template <typename TaskFn, typename ReplyFn>
        void scheduleAndReplyValue(const TaskFn& task,
            const ReplyFn& reply,
            std::weak_ptr<Scheduler> replyScheduler) {
            auto scheduled = [replyScheduler = std::move(replyScheduler), task, reply]{
                auto lock = replyScheduler.lock();
            if (!replyScheduler) return;
            auto scheduledReply = [reply, result = task()]{ reply(result); };
            replyScheduler->schedule(std::move(scheduledReply));
            };

            schedule(std::move(scheduled));
        }
    };
} // namespace mbgl



