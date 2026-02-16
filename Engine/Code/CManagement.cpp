#include "CManagement.h"
#include "CHelper.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement() : m_pScene(nullptr)
{
}

CManagement::~CManagement()
{
    Free();
}

CComponent* CManagement::Get_FirstObjectComponent(COMPONENTID eID, std::wstring_view svLayerTag, std::wstring_view svObjTag, std::wstring_view svComponentTag)
{
     if (nullptr == m_pScene)
         return nullptr;

     CLayer* pLayer = m_pScene->Get_Layer(svLayerTag);
     if (nullptr == pLayer)
         return nullptr;

     CGameObject* pGameObject = pLayer->Get_GameObjectFirst(svObjTag);
     if (nullptr == pGameObject)
         return nullptr;

     return pGameObject->Get_Component(eID, svComponentTag);
}

HRESULT CManagement::Set_Scene(CScene* pScene)
{
    if (nullptr == pScene)
        return  E_FAIL;

    // 해당씬이 지워지기 전에 호출한다.
    auto tmp = pScene->Before_SceneChange();

    Safe_Release(m_pScene);

    m_pScene = pScene;

    // 해당씬이 장착되고 자서 호출한다.
    m_pScene->After_SceneChange(tmp);

    return S_OK;
}

_int CManagement::Update_Scene(const _float& fTimeDelta)
{
    if (nullptr == m_pScene)
        return -1;

    return m_pScene->Update_Scene(fTimeDelta);
}

void CManagement::LateUpdate_Scene(const _float& fTimeDelta)
{
    m_pScene->LateUpdate_Scene(fTimeDelta);
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRenderer::GetInstance()->Render_GameObject(pGraphicDev);

    // debug 용
    m_pScene->Render_Scene();
}

void CManagement::Free()
{
    Safe_Release(m_pScene);
}

void CManagement::Update_ImGui()
{
    ImGui::Begin("MANAGEMENT");

    if (ImGui::TreeNode("RAW"))
    {
        for (auto& pLayerPair : *m_pScene->Get_Layer())
        {
            string s = CHelper::WStringToString(pLayerPair.first);
            if (ImGui::TreeNode(s.c_str()))
            {
                for (auto& pGamePair : *pLayerPair.second->Get_GameObjects())
                {
                    string s = "[" + ::to_string(pGamePair.second.size()) + "]#" + CHelper::WStringToString(pGamePair.first);
                    if (ImGui::TreeNode(s.c_str()))
                    {
                        for (auto& pGameObj : pGamePair.second)
                        {
                            pGameObj->Update_ImGui();
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("TREE"))
    {
        vector<CGameObject*> pRootObjectList;
        unordered_map< CGameObject*, std::string> mapDebug;

        // 루프1. 루트 오브젝트를 추린다.
        for (auto& pLayer : *m_pScene->Get_Layer())
        {
            for (auto& pObjMap : *(pLayer.second->Get_GameObjects()))
            {
                int i = 0;
                for (auto& pObj : pObjMap.second)
                {
                    std::string s = CHelper::WStringToString(pObjMap.first) + "_" + std::to_string(i);
                    mapDebug.insert({ pObj, s });
                    if (!pObj->Get_Parent())
                    {
                        pRootObjectList.push_back(pObj);
                    }
                    ++i;
                }
            }
        }

        std::function<void(CGameObject*)> RecursiveImguiUpdate = [&](CGameObject* pGObj)
        {
            if (ImGui::TreeNode((void*)pGObj, mapDebug[pGObj].c_str()))
            {
                pGObj->Update_ImGui();

                if (!pGObj->Get_Children()->empty())
                {
                    if (ImGui::TreeNode("Childs"))
                    {
                        for (auto& pChild : *pGObj->Get_Children())
                        {
                            RecursiveImguiUpdate(pChild);
                        }
                        ImGui::TreePop();
                    }
                }

                ImGui::TreePop();
            }
        };


        for (auto iter = pRootObjectList.begin(); iter != pRootObjectList.end(); ++iter)
        {
            RecursiveImguiUpdate(*iter);
        }

        ImGui::TreePop();
    }


    ImGui::End();
}