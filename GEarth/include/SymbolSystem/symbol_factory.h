/*!
*@File    symbol_factory.h
*@Brief   文件描述:下次一定
*@Date    2021/12/22
*/
#pragma once

#include "Common/Factory.h"
#include "SymbolSystem/marker_symbol.h"


class CSymbolFactory : public CFactory<CSymbolFactory>
{
public:
	CMarkerSymbol* GetDefaultMarkerSymbol();
};


