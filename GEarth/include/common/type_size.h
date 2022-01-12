/*!
*@File    type_size.h
*@Brief   类型大小
*@Date    2021/12/22
*/
#pragma once
#include "common/type.h"
#include <string>
const int UINT8_SIZE = 1;
const int BOOL_SIZE = 1;
const int CHAR_SIZE = 1;
const int SHORT_SIZE = 2;
const int INT_SIZE = 4;
const int UINT_SIZE = 4;
const int FLOAT_SIZE = 4;
const int INT64_SIZE = 8;
const int DOUBLE_SIZE = 8;

namespace COMMON_NAMESPACE
{
    class CORE_PUBLIC CTypeSize
    {
    public:
        template<class T>
        size_t GetSize(const T&) { return sizeof(T); };

        template<>
        size_t CTypeSize::GetSize(const std::string& s)
        {
            return 4 + s.size();
        };

        static CTypeSize& Instance()
        {
            static CTypeSize instance;
            return instance;
      
        };
    };
  
}





