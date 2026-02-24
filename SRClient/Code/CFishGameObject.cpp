#include "CFishGameObject.h"

#include "CAssetMgr.h"
#include "CAssetSpine.h"
#include "CFishStopState.h"
#include "CFishSwimState.h"
#include "CHelper.h"

CFishGameObject::CFishGameObject()
    : m_sFishName({})
    , m_fSpeed(0.f)
    , m_fsm(Fish::FS_STOP)
{

}

void CFishGameObject::Update_ImGui()
{
    CGameObject::Update_ImGui();
   
    ImGui::Text("CurrState: %s", CHelper::WStringToString(m_fsm.Get_CurrentStateStr()).c_str());

    if (ImGui::Button("SWIM"))
    {
        m_fsm.RequestChangeState(Fish::FS_SWIM);
    }
}

_int CFishGameObject::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = CGameObject::Update_GameObject(fTimeDelta);
    if (m_fsm.IsRequestedChange())
    {
        m_fsm.ChangeState();
    }
    m_fsm.Get_CurrentState()->Update(fTimeDelta);
    return iExit;
}

void CFishGameObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
    m_fsm.Get_CurrentState()->LateUpdate(fTimeDelta);
}

void CFishGameObject::Render(function<void()> beforeDrawLambda)
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (beforeDrawLambda)
    {
        beforeDrawLambda();
    }
    m_pSpineCom->Render(m_pDynamicBuffer);

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFishGameObject::Ready(std::wstring_view svSpineName)
{
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    // 버퍼
    if (FAILED((AddComponent<Engine::CDynamicBuffer, ID_STATIC>(L"Proto_DynamicBuffer_Spine", L"Com_DynamicBuffer", &m_pDynamicBuffer))))
        return E_FAIL;

    // 스파인
    if (FAILED((AddComponent<Engine::CSpine, ID_DYNAMIC>(L"Proto_Spine", L"Com_Spine", &m_pSpineCom))))
        return E_FAIL;

    if (auto pAssSpine = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetSpine>(svSpineName))
    {
        m_pDynamicBuffer->Set_VertexCnt(pAssSpine->Get_NumVertex());
        m_pDynamicBuffer->Set_TriCnt(pAssSpine->Get_NumTri());
        m_pDynamicBuffer->Ready_Buffer();

        m_pSpineCom->Ready_AfterCreate(pAssSpine->Get_SkeletonData(), pAssSpine->Get_AnimationStateData(), svSpineName);
        m_pSpineCom->Index_Buffer_Lock(m_pDynamicBuffer->Get_IndexBuffer());
    };


    m_fsm.AddState(Fish::FS_STOP, make_unique<Fish::CFishStopState>(this));
    m_fsm.AddState(Fish::FS_SWIM, make_unique<Fish::CFishSwimState>(this));


    m_fsm.Get_CurrentState()->Enter();
	return S_OK;
}

void CFishGameObject::Free()
{
    CGameObject::Free();
}
