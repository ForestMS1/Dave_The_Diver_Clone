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
		//m_mapObject.insert({ std::wstring(svObjTag), pGameObject });
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
	
	return iter->second.front();
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iResult(0);

	for (auto& pObjList : m_mapGameObjects)
	{
		for (auto& pObj : pObjList.second)
		{
			iResult = pObj->Update_GameObject(fTimeDelta);

			if (iResult & 0x80000000)
				return iResult;
		}
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
	for (auto& pObjList : m_mapGameObjects)
	{
		for (auto& pObj : pObjList.second)
		{
			pObj->LateUpdate_GameObject(fTimeDelta);
		}
	}
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
			pObj->Release();
		}
	}
	m_mapGameObjects.clear();
}
