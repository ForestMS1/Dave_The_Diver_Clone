#include "CManagement.h"

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
