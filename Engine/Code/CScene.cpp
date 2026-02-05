#include "CScene.h"
#include <CHelper.h>

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CLayer* CScene::Get_Layer(std::wstring_view svLayerTag)
{
    auto    iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_FinderSV(svLayerTag));

    if (iter == m_mapLayer.end())
        return nullptr;

    return iter->second;
}

HRESULT CScene::Ready_Scene()
{
    return S_OK;
}

_int CScene::Update_Scene(const _float& fTimeDelta)
{
	vector<CGameObject*> pRootObjectList;

	// 루프1. 루트 오브젝트를 추린다.
	for (auto& pLayer : m_mapLayer)
	{
		for (auto& pObjMap : *(pLayer.second->Get_GameObjects()))
		{
			for (auto& pObj : pObjMap.second)
			{
				if (!pObj->Get_Parent())
				{
					pRootObjectList.push_back(pObj);
				}
			}
		}
	}
	

	// 루프2. 루트오브젝트를 시작으로, 큐를 사용하여 레벨순회를돈다.
	//			이때 죽을 애들은 SetDead로 마킹한다.
	for (auto iter = pRootObjectList.begin(); iter != pRootObjectList.end(); ++iter)
	{
		vector<CGameObject*> hier;

		CHelper::TreeLevelTraversal(*iter, [&](CGameObject* pObj) {
				hier.push_back(pObj);
			});

		for (auto& pObj : hier)
		{
			if (!pObj->Get_Dead())
			{
				int iRes = pObj->Update_GameObject(fTimeDelta);
				if (iRes & 0x80000000)
				{
					return iRes;
				}
				else if (iRes == OBJ_DEAD)
				{
					pObj->Set_Dead();
				}
			}
		}
	}

	// 루프3. 데드 마킹된 애들에 대해서 해제 처리를 진행한다.
	for (auto& pLayer : m_mapLayer)
	{
		for (auto& pObjMap : *(pLayer.second->Get_GameObjects()))
		{
			list<CGameObject*>* pObjList = &pObjMap.second;

			for (auto iter = pObjList->begin(); iter != pObjList->end(); )
			{
				if ((*iter)->Get_Dead())
				{
					Safe_Release(*iter);
					iter = pObjList->erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
	}
	

    return 0;
}

void CScene::LateUpdate_Scene(const _float& fTimeDelta)
{
	vector<CGameObject*> pRootObjectList;
    for (auto& pLayer : m_mapLayer)
    {
        for (auto& pObjMap : *(pLayer.second->Get_GameObjects()))
        {
            for (auto& pObj : pObjMap.second)
            {
                if (!pObj->Get_Parent())
                {
                    pRootObjectList.push_back(pObj);
                }
            }
        }
    }

    for (auto& pRootObj : pRootObjectList)
    {
		vector< CGameObject*> hier;

		CHelper::TreeLevelTraversal(pRootObj, [&](CGameObject* pObj) {
			hier.push_back(pObj);
			});


		for (auto& pObj: hier)
		{
			if (!pObj->Get_Dead())
			{
				pObj->LateUpdate_GameObject(fTimeDelta);
			}
		}
    }
}



void CScene::Free()
{
    for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
    m_mapLayer.clear();

    Safe_Release(m_pGraphicDev);
}
