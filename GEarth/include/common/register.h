/*!
*@File    register.h
*@Brief   ע����
*@Date    2021/12/22
*/
#pragma once
#include "Common/ApiDescription.h"
#include <map>


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

//ע�������
#define REGISTER_CLASS(class_name)   \
	private:                         \
		static void* instance()      \
		{                            \
           return new class_name;    \
		}                            \
		static const Register _static_register; \
		std::string m_sClassName = #class_name


//ע�����Ա��������CPP�ļ��ж���
#define REGISTER_OBJECT(class_name) \
		const Register class_name::_static_register(#class_name, class_name::instance);







