#include "SymbolSystem/marker_symbol.h"
#include "Common/Log.h"

REGISTER_OBJECT(CMarkerSymbol)

CMarkerSymbol::CMarkerSymbol()
	:Symbol(Symbol::EnSymbolType::SYMBOL_TYPE_MARKER)
{

}


CMarkerSymbol::~CMarkerSymbol()
{

}

void CMarkerSymbol::AddMarkerLayer(CMarkerLayer* pMarkerLayer)
{
	if (!pMarkerLayer)
	{
		LOG_ERROR("CMarkerLayer is error");
	}
	else
	{
		m_pLayers.emplace_back(pMarkerLayer);
	}
}

CMarkerLayer* CMarkerSymbol::GetMarkerLayer(const unsigned& index)
{
	if (index >= m_pLayers.size())
	{
		LOG_ERROR("CMarkerLayer'index  is error");
		return nullptr;
	}
	else
	{
		return m_pLayers[index];
	}
}

void CMarkerSymbol::RemoveMarkerLayer(CMarkerLayer* pMarkerLayer)
{
	for (auto itr = m_pLayers.begin();itr!= m_pLayers.end();itr++)
	{
		if ((*itr) == pMarkerLayer)
		{
			m_pLayers.erase(itr);
			return;
		}
	}
}

void CMarkerSymbol::RemoveMarkerLayer(const unsigned& index)
{
	if (index >= m_pLayers.size())
	{
		LOG_ERROR("CMarkerLayer'index  is error");
	}
	else
	{
		m_pLayers.erase(m_pLayers.begin() + index);
	}
}
