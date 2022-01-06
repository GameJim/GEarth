/*!
*@File    Object.h
*@Brief   一般作为数据
*@Date    2021/12/22
*/
#pragma once
#include "Common/byte.h"
#include "Common/ApiDescription.h"
#include "Common/Subject.h"
#include <assert.h>
#include <memory>
class CORE_PUBLIC CObject 
{
public:
	virtual ~CObject() = default;
	/*!
	/* @Brief:     序列化
	/* @Date:      2021/12/21
	/* @Parameter: CByte* data
	/* @Return     void
	*/
    virtual void Serialize(CByte& data) {};

	/*!
	/* @Brief:     反序列化
	/* @Date:      2021/12/22
	/* @Parameter: CByte & data
	/* @Return     void
	*/
    virtual void Deserialize(CByte& data) {};

	/*!
	/* @Brief:     获取字节大小
	/* @Date:      2021/12/22
	/* @Return     size_t
	*/
    virtual size_t GetSize() { return 0; };
protected:
	CObject() = default;
};



