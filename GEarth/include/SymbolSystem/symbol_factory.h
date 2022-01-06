/*!
*@File    symbol_factory.h
*@Brief   �ļ�����:�´�һ��
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


