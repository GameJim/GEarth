#pragma once
/*!
*@File    Byte.h
*@Brief   字节操作类,不支持多线程
*@Date    2021/12/22
*/
#include <memory>
#include <string>
#include "common/nonCopyable.h"
#include "common/type.h"

namespace COMMON_NAMESPACE
{
    class CORE_PUBLIC CByte : public CNonCopyable
    {
    public:
        CByte(unsigned char* src, unsigned nSize = 0);
        CByte(size_t nSize);
        CByte() = delete;
        virtual ~CByte();

        const unsigned char* Data() const;
        unsigned Size() const;

        //跳转到指定位置
        void SeekgPos(const unsigned& nsize = 0);
        //跳过多少
        void Seekg(const unsigned& nsize);
        //数据转换
        CByte& operator >> (uint8_t& ui8);
        CByte& operator >> (bool& b);
        CByte& operator >> (char& c);
        CByte& operator >> (short& s);
        CByte& operator >> (int& i);
        CByte& operator >> (unsigned& ui);
        //CByte& operator >> (unsigned int& ui);
        CByte& operator >> (float& i);
        CByte& operator >> (double& ui);
        CByte& operator >> (std::string& str);

       
       



        CByte& operator << (const uint8_t& ui8);
        CByte& operator << (const bool& b);
        CByte& operator << (const char& c);
        CByte& operator << (const short& s);
        CByte& operator << (const int& i);
        CByte& operator << (const unsigned& ui);
        //CByte& operator << (const unsigned int& ui);
        CByte& operator << (const float& f);
        CByte& operator << (const double& d);
        CByte& operator << (const std::string& str);

        //处理数组
        void ReadCharArry(unsigned char* data, unsigned nSize);
        void WriteCharArry(unsigned char* data, unsigned nSize);


        template<class T>
        void ReadArry(T*, const unsigned& nSize);
        template<class T>
        void WriteArry(T*, const unsigned& nSize);

        template<class T>
        void ReadObjectArry(T*, const unsigned& nSize);
        template<class T>
        void WriteObjectArry(T*, const unsigned& nSize);

        template<class T>
        CByte& operator >> (CVec2<T>& vec);
        template<class T>
        CByte& operator << (const CVec2<T>& vec);

        template<class T>
        CByte& operator >> (CVec3<T>& vec);
        template<class T>
        CByte& operator << (const CVec3<T>& vec);

        template<class T>
        CByte& operator >> (CVec4<T>& vec);
        template<class T>
        CByte& operator << (const CVec4<T>& vec);


        template<class T>
        CByte& operator >> (CMatrix<T>& vec);
        template<class T>
        CByte& operator << (const CMatrix<T>& vec);
        
    protected:
        unsigned char* itr;     //数据的浏览器

        unsigned char* m_pData;
        size_t m_nSize;   //字节大小，如果为0则不代表有意义
        bool m_IsSwap;  //判断数据大小端
    };

    template<class T>
    void CByte::ReadArry(T*, const unsigned& nSize)
    {
        for (int i = 0;i < nSize;++i)
        {
            this << T[i];
        }
    }

    template<class T>
    void CByte::WriteArry(T*, const unsigned& nSize)
    {
        for (int i = 0; i < nSize; ++i)
        {
            this >> T[i];
        }
    }

    template<class T>
    void CByte::ReadObjectArry(T*, const unsigned& nSize)
    {
        for (int i = 0; i < nSize; ++i)
        {
            T[i].Serialize(*this);
        }
    }

    template<class T>
    void CByte::WriteObjectArry(T*, const unsigned& nSize)
    {
        T[i].Deserialize(*this);
    }

    template<class T>
    CByte& CByte::operator >> (CVec2<T>& vec)
    {
        *this >> vec.x >> vec.y;

        return *this;
    }


    template<class T>
    CByte& CByte::operator << (const CVec2<T>& vec)
    {
        *this << vec.x << vec.y;

        return *this;
    }
  

    template<class T>
    CByte& CByte::operator >> (CVec3<T>& vec)
    {
        *this >> vec.x >> vec.y >> vec.z;

        return *this;
    }


    template<class T>
    CByte& CByte::operator << (const CVec3<T>& vec)
    {
        *this << vec.x << vec.y << vec.z;

        return *this;
    }

    template<class T>
    CByte& CByte::operator >> (CVec4<T>& vec)
    {
        *this >> vec.x >> vec.y >> vec.z >> vec.w;

        return *this;
    }


    template<class T>
    CByte& CByte::operator << (const CVec4<T>& vec)
    {
        *this << vec.x << vec.y << vec.z << vec.w;

        return *this;
    }

    template<class T>
    CByte& CByte::operator<<(const CMatrix<T>& matrix)
    {
        *this << matrix[0] << matrix[1] << matrix[2] << matrix[3];
        return *this;
    }


    template<class T>
    CByte& CByte::operator >> (CMatrix<T>& matrix)
    {
        *this >> matrix[0] >> matrix[1] >> matrix[2] >> matrix[3];
        return *this;
    }

}
