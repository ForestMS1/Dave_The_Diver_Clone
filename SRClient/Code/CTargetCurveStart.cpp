#include "CTargetCurveStart.h"
#include "CManagement.h"
#include "CGraphicDev.h"
#include "CDiveDave.h"
#include "CHelper.h"
CTargetCurveStart::CTargetCurveStart()
{
}

CTargetCurveStart::CTargetCurveStart(const CTargetCurveStart& rhs)
    : CGameObject(rhs)
{
}

CTargetCurveStart::~CTargetCurveStart()
{
}

HRESULT CTargetCurveStart::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 0.4f, 0.4f, 1.f };
    m_pTransformCom->Multiply_Scale(&vScale);

    _float fWidth = 23.f;
    _float fHeight = 128.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    m_pTransformCom->Multiply_Scale(&vScale);

    return S_OK;
}

_int CTargetCurveStart::Update_GameObject(const _float& fTimeDelta)
{
    if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DIVEDAVESTATE::ATTACK)
        return 0;

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    Set_ParentTransform();
    Mouse_Check();
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    return iExit;
}

void CTargetCurveStart::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DIVEDAVESTATE::ATTACK)
        return;

    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
}

void CTargetCurveStart::Render_GameObject()
{
    if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DIVEDAVESTATE::ATTACK)
        return;

    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


HRESULT CTargetCurveStart::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_TargetCurveStartTexture", L"Com_Texture", &m_pTextureCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

void CTargetCurveStart::Set_ParentTransform()
{
    _vec3 vParentPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);
    m_vOffset.y *= m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->m_vScale.y;
    vParentPos += m_vOffset;
    m_pTransformCom->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);
}

void CTargetCurveStart::Mouse_Check()
{
    if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_AttackSubState() == ATTACKSUBSTATE::ATTACK_FIGHT)
        return;

    _vec3 vMousePos, vPlayerPos;
    CHelper::GetMousePointInWorld(&vMousePos);
    static_cast<CDiveDave*>(m_pParentGameObject)->Get_Pos(&vPlayerPos);

    if (vMousePos.x <= vPlayerPos.x)
    {
        _vec3 vRotateDir = { 0.f, -180.f, 0.f };
        m_vOffset = { -1.f, 0.2f, 0.f };
        m_pTransformCom->Set_Rotation(&vRotateDir);
    }
    else
    {
        _vec3 vRotateDir = { 0.f, 0.f, 0.f };
        m_vOffset = { 1.f, 0.2f, 0.f };
        m_pTransformCom->Set_Rotation(&vRotateDir);
    }
}


CTargetCurveStart* CTargetCurveStart::Create()
{
    CTargetCurveStart* pInstance = new CTargetCurveStart;
    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("TargetCurveStart create Failed");
        return nullptr;
    }
    return pInstance;
}

void CTargetCurveStart::Free()
{
    CGameObject::Free();
}
