/*!
*@File    fill_symbol.h
*@Brief   文件描述:下次一定
*@Date    2021/12/22
*/
#pragma once
#include "Common/register.h"
#include "SymbolSystem/Symbol.h"
#include <vector>

class CFillSymbol : public Symbol
{
	REGISTER_CLASS(CFillSymbol);
public:
	CFillSymbol();
	virtual ~CFillSymbol() = default;
protected:

};




