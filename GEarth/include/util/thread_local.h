﻿/*!
*@File    thread_local.h
*@Brief   文件描述:下次一定
*@Date    2023/2/10
*/
#pragma once

#include <type_traits>

namespace util {
    namespace impl {

        class ThreadLocalBase {
        protected:
            ThreadLocalBase();
            ~ThreadLocalBase();

            void* get();
            void set(void*);

        private:
            std::aligned_storage_t<sizeof(void*), alignof(void*)> storage;
        };

    } // namespace impl

    template <class T>
    class ThreadLocal : public impl::ThreadLocalBase {
    public:
        ThreadLocal() = default;

        ThreadLocal(T* val) {
            set(val);
        }

        T* get() {
            return reinterpret_cast<T*>(impl::ThreadLocalBase::get());
        }

        void set(T* ptr) {
            impl::ThreadLocalBase::set(ptr);
        }
    };

} 


