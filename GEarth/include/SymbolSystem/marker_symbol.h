/*!
*@File    marker_symbol.h
*@Brief   µã·ûºÅ
*@Date    2021/12/22
*/
#pragma once
#include "Common/register.h"
#include "SymbolSystem/Symbol.h"
#include "SymbolSystem/marker_layer.h"
#include <vector>

class CMarkerSymbol : public Symbol
{
	REGISTER_CLASS(CMarkerSymbol);
public:
	CMarkerSymbol();
	virtual ~CMarkerSymbol();

	void AddMarkerLayer(CMarkerLayer* pMarkerLayer);
	CMarkerLayer* GetMarkerLayer(const unsigned& index);
	void RemoveMarkerLayer(CMarkerLayer* pMarkerLayer);
	void RemoveMarkerLayer(const unsigned& index);
protected:
	std::vector<CMarkerLayer*>  m_pLayers;   //Í¼²ã
};


