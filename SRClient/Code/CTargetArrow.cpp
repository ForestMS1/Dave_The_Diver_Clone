#include "CTargetArrow.h"
#include "CManagement.h"
#include "CGraphicDev.h"
#include "CDiveDave.h"
#include "CHelper.h"
#include "CAttackReadyArmTex.h"
CTargetArrow::CTargetArrow()
{
}

CTargetArrow::CTargetArrow(const CTargetArrow& rhs)
    : CGameObject(rhs)
{
}

CTargetArrow::~CTargetArrow()
{
}

HRESULT CTargetArrow::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 0.1f, 0.1f, 1.f };
    m_pTransformCom->Multiply_Scale(&vScale);

    _float fWidth = 26;
    _float fHeight = 18;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    m_pTransformCom->Multiply_Scale(&vScale);

    return S_OK;
}

_int CTargetArrow::Update_GameObject(const _float& fTimeDelta)
{
    ImGui::Begin("Arrow");
    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    ImGui::SliderFloat3("Transform", vPos, 0.f, 0.f);
    ImGui::End();
    if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
        return 0;

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    Set_ParentTransform();
    Rotate_ToMouse();
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    return iExit;
}

void CTargetArrow::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
        return;

    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTargetArrow::Render_GameObject()
{
    if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
        return;

    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTargetArrow::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CTargetArrowTex, ID_STATIC>(L"Proto_TargetArrowBuffer", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_TargetArrowGunTexture", L"Com_Texture", &m_pTextureCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

void CTargetArrow::Set_ParentTransform()
{
    _vec3 vParentPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);
    //m_vOffset = { 0.5f, 0.3f, 0.f };
    m_vOffset.y *= m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->m_vScale.y;
    vParentPos += m_vOffset;
    m_pTransformCom->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);
}

void CTargetArrow::Rotate_ToMouse()
{
    _vec3 vMousePos, vPlayerPos;

    CHelper::GetMousePointInWorld(&vMousePos);
    m_pTransformCom->Get_Info(INFO_POS, &vPlayerPos);

    _vec3 vDir = vMousePos - vPlayerPos;

    float fLengthSq = vDir.x * vDir.x + vDir.y * vDir.y;

    // 너무 가까우면 회전 유지
    if (fLengthSq < 0.001f)
        return;

    float fDegree = D3DXToDegree(atan2f(vDir.y, vDir.x));

    if (vDir.x < 0.f)
    {
        m_vOffset = { -0.35f, 0.3f, 0.f };
        m_pTransformCom->m_vAngle.x = 180.f;
    }
    else
    {
        m_vOffset = { 0.35f, 0.3f, 0.f };
        m_pTransformCom->m_vAngle.x = 0.f;
    }

    m_pTransformCom->m_vAngle.z = fDegree;
}

CTargetArrow* CTargetArrow::Create()
{
    CTargetArrow* pInstance = new CTargetArrow;
    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("TargetArrow create Failed");
        return nullptr;
    }
    return pInstance;
}

void CTargetArrow::Free()
{
    CGameObject::Free();
}