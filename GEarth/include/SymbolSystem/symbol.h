#pragma once
#include <string>
#include "Common/register.h"
#include "common/Object.h"

/*!
*@class   Symbol
*@Brief   ���������࣬����
*@Date    2021/12/21
*/
class CORE_PUBLIC SymbolProperty
{
public:
	virtual ~SymbolProperty() = default;
	virtual void Serialize(CByte& byte) = 0;
	virtual void Deserialize(CByte& byte) = 0;
	virtual size_t GetSize() = 0;
protected:
	SymbolProperty() = default;        
};


/*!
*@class   Symbol
*@Brief   ���ŵĻ���
*@Date    2021/12/21
*/
class CORE_PUBLIC Symbol : public CObject
{
public:
	virtual ~Symbol();
	enum EnSymbolType : uint8_t
	{
		SYMBOL_TYPE_MARKER = 0,     //�����
		SYMBOL_TYPE_LINE = 1,       //�߷���
		SYMBOL_TYPE_FILL = 2,       //������ 
		SYMBOL_TYPE_ANIMATION = 3,  //�������ţ���̬���ţ�
		SYMBOL_TYPE_CUSTOM          //�Զ������
	};
	
	/*!
	/* @Brief:     ��������
	/* @Date:      2021/12/21
	/* @Return     Symbol::EnSymbolType
	*/
	EnSymbolType SymbolType() const;

	
	/*!
	/* @Brief:     ��ӷ�������
	/* @Date:      2021/12/21
	/* @Parameter: SymbolProperty * pProperty
	/* @Return     void
	*/
	void SetProperty(SymbolProperty* pProperty);


	SymbolProperty* Property() const;
	/*!
	/* @Brief:     ���л�
	/* @Date:      2021/12/21
	/* @Parameter: CByte* data
	/* @Return     void
	*/
	virtual void Serialize(CByte& data);    //���л�

	/*!
	/* @Brief:     �����л�
	/* @Date:      2021/12/21
	/* @Parameter: CByte * data
	/* @Return     void
	*/
	virtual void Deserialize(CByte& data);  //�����л�

	/*!
	/* @Brief:     ��ȡ��С
	/* @Date:      2021/12/21
	/* @Return     size_t
	*/
	virtual size_t GetSize();                       // �ļ������ֽ���
protected:
	Symbol(const EnSymbolType& symbolType);
	EnSymbolType m_eSymbolType;                   //��������
	SymbolProperty* m_pProperty;
};

