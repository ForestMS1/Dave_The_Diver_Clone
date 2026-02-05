#include "CLayer.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

HRESULT CLayer::Add_GameObject(std::wstring_view svObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	auto	iter = find_if(m_mapGameObjects.begin(), m_mapGameObjects.end(),
		CTag_FinderSV(svObjTag));

	if (iter == m_mapGameObjects.end())
	{
		m_mapGameObjects.insert({ std::wstring(svObjTag), {pGameObject} });
	}
	else
	{
		iter->second.push_back(pGameObject);
	}

	return S_OK;
}

list<CGameObject*>* CLayer::Get_GameObjects(std::wstring_view svObjTag)
{
	auto	iter = find_if(m_mapGameObjects.begin(), m_mapGameObjects.end(),
		CTag_FinderSV(svObjTag));

	if (iter == m_mapGameObjects.end())
		return nullptr;

	return &(iter->second);
}

CGameObject* CLayer::Get_GameObjectFirst(std::wstring_view svObjTag)
{
	auto	iter = find_if(m_mapGameObjects.begin(), m_mapGameObjects.end(),
		CTag_FinderSV(svObjTag));

	if (iter == m_mapGameObjects.end())
		return nullptr;

	if (iter->second.empty())
		return nullptr;
	else
		return iter->second.front();
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}



CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		MSG_BOX("Layer Create Failed");
		Safe_Release(pLayer);
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	for (auto& pObjList : m_mapGameObjects)
	{
		for (auto& pObj : pObjList.second)
		{
			Safe_Release(pObj);
		}
	}
	m_mapGameObjects.clear();
}
