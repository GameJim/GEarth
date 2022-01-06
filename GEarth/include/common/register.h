/*!
*@File    register.h
*@Brief   注册类
*@Date    2021/12/22
*/
#pragma once
#include "common/apidescription.h"
#include <map>

namespace COMMON_NAMESPACE
{
    typedef void * (*fun_ptr) ();
    typedef std::map<const char *, fun_ptr> create_obj_map;

    class _Static {
    public:
        template<int N, class T>
        static T & _static() {
            //initial once
            static T _instance;
            return _instance;
        }
    };


    class CORE_PUBLIC ClassFactory {
    public:
        static void  * get_instance(const char *);
        static void register_class(const char *, fun_ptr);
    };

    class CORE_PUBLIC Register {
    public:
        Register(const char *, fun_ptr);
    };

    //注册类对象
#define REGISTER_CLASS(class_name)   \
	private:                         \
		static void* instance()      \
		{                            \
           return new class_name;    \
		}                            \
		static const Register _static_register; \
		std::string m_sClassName = #class_name


    //注册类成员，必须在CPP文件中定义
#define REGISTER_OBJECT(class_name) \
		const Register class_name::_static_register(#class_name, class_name::instance);
}









