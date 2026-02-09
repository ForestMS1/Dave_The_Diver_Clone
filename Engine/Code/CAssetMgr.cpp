#include "CAssetMgr.h"


IMPLEMENT_SINGLETON(CAssetMgr)

CAssetMgr::CAssetMgr()
{

}

CAssetMgr::~CAssetMgr()
{
	Free();
}

HRESULT CAssetMgr::AddAsset(std::wstring_view svLayerTag, CAsset* pAsset)
{
	if (nullptr == pAsset) return E_FAIL ;

	if (auto pAssetLayer = Find_AssetLayer(svLayerTag))
	{
		pAssetLayer->push_back(pAsset);
	}
	else
	{
		m_AssetMap.insert({ std::wstring(svLayerTag), {pAsset} });
	}

	return S_OK;
}

void CAssetMgr::LoadAsset()
{
	for (auto& pAssetMap : m_AssetMap)
	{
		LoadAsset(pAssetMap.first);
	}
}

void CAssetMgr::LoadAsset(std::wstring_view svLayerTag)
{
	if (auto pAssetLayer = Find_AssetLayer(svLayerTag))
	{
		for (auto& pAsset : *pAssetLayer)
		{
			if (pAsset->Get_State() == CAsset::UNLOAD)
			{
				pAsset->Load();
			}
		}
	}
}

void CAssetMgr::DelAsset(std::wstring_view svLayerTag)
{
	if (auto pAssetLayer = Find_AssetLayer(svLayerTag))
	{
		for (auto iter = pAssetLayer->begin(); iter != pAssetLayer->end();)
		{
			Safe_Release(*iter);
			iter = pAssetLayer->erase(iter);
		}
	}
}

vector<CAsset*>* CAssetMgr::Find_AssetLayer(std::wstring_view svLayerTag)
{
	auto	iter = find_if(m_AssetMap.begin(), m_AssetMap.end(),
		CTag_FinderSV(svLayerTag));

	if (iter == m_AssetMap.end())
		return nullptr;

	return &(*iter).second;
}

void CAssetMgr::Free()
{
	for (auto& pAssetMap : m_AssetMap)
	{
		for (auto& pAsset : pAssetMap.second)
		{
			Safe_Release(pAsset);
		}
	}
	m_AssetMap.clear();
}