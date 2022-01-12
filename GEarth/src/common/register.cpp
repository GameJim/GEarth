#include "common/register.h"
#include "common/log.h"
#include <iostream>

namespace COMMON_NAMESPACE
{
    void * ClassFactory::get_instance(const std::string& class_name) {
        if (_Static::_static<0, create_obj_map>().find(class_name) == data.end()) {
            LOG_INFO(("Failed to find class:") + std::string(class_name));
            return NULL;
        }
        //std::cout << "found\n";
        return _Static::_static<0, create_obj_map>()[class_name]();
    }

    void ClassFactory::register_class(const std::string& class_name, fun_ptr fp) {
        _Static::_static<0, create_obj_map>().insert(std::make_pair(class_name, fp));
        LOG_DEBUG(("register class:") + std::string(class_name));
    }

    //辅助注册类的实现
    Register::Register(const std::string& class_name, fun_ptr fp) {
        ClassFactory::register_class(class_name, fp);
    }
}





