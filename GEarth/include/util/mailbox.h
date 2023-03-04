/*!
*@File    mailbox.h
*@Brief   文件描述:下次一定
*@Date    2023/2/10
*/
#pragma once
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <util/util_export.h>
#include <util/message.h>
namespace util {

    class Scheduler;
    class Message;

    class UTIL_API Mailbox : public std::enable_shared_from_this<Mailbox> {
    public:
        /*!
        /* @Brief:     信息邮箱
        /* @Date:      2023/2/10
        /* @Parameter: const bool & recursive_  
                       以下条件使用false，否则使用默认值
                       1、Scheduler为多线程 且 信息之间存在先后响应关系必须开false，否则会阻塞多线程
        /* @Return     
        */
        Mailbox();
        Mailbox(Scheduler&);

        void open(Scheduler& scheduler_);
        void close();

        bool isOpen() const;

        void push(std::unique_ptr<Message>);

        void receive();  //如果存在Scheduler则自动开始接收信息

        static void maybeReceive(const std::weak_ptr<Mailbox>&);
        static std::function<void()> makeClosure(std::weak_ptr<Mailbox>);

		bool hasMessage();
    private:
        std::weak_ptr<Scheduler> weakScheduler;

        std::recursive_mutex receivingMutex;
        std::mutex pushingMutex;

        bool closed{ false };
        //bool recursive{ true };

        std::mutex queueMutex;
        std::queue<std::unique_ptr<Message>> queue;
    };

} 
