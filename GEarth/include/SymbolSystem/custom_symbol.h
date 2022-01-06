/*!
*@File    custom_symbol.h
*@Brief   文件描述:下次一定
*@Date    2021/12/22
*/
#pragma once
#include "Common/register.h"
#include "SymbolSystem/Symbol.h"
#include <vector>

class CCustomSymbol : public Symbol
{
	REGISTER_CLASS(CCustomSymbol);
public:
	CCustomSymbol();
	virtual ~CCustomSymbol() = default;
protected:

};




