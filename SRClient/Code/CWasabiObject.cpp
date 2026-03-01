#include "pch.h"
#include "CWasabiObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CWasabi.h"
#include "CColliderMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CWasabiObject::CWasabiObject()
    : CGameObject()
{

    created = false;
    percent = 100.f;
}

CWasabiObject::CWasabiObject(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CWasabiObject::~CWasabiObject()
{
}
void CWasabiObject::Update_ImGui()
{
    CGameObject::Update_ImGui();
    ImGui::DragFloat("tempY", &gauge, 0.01f);
}
HRESULT CWasabiObject::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    _vec3 vExtents = { 1.0f, 1.0f, 3.01f };
    _vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Wasabi", this);


    return S_OK;
}

_int CWasabiObject::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Wasabi", m_pAABB);

    if (created) {
        CGameObject* wasabi = CWasabi::Create();
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Wasabi", wasabi);
        created = false;
    }
    _matrix vPos = *m_pTransformCom->Get_World();
    //vPos.m[3][2] = 0;
    //vPos.m[3][0] -= 0.3f;
    m_pAABB->Transform(&vPos);
    percent = (gauge + 0.9f) * 100 / 0.93f;
    return iExit;
}

void CWasabiObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);
}

void CWasabiObject::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();


    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_WasabiGaugeBar"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    D3DXMATRIX matTmp = *m_pTransformCom->Get_World();
    matTmp = *m_pTransformCom->Get_World();
    matTmp.m[3][1] += 1.5f;
    matTmp.m[0][0] = 0.1f;
    matTmp.m[3][0] -= 0.2f;


    //matTmp.m[1][1] = 0.1f;
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
    m_pBufferCom->Render_Buffer();

    pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    pGraphicDev->SetRenderState(D3DRS_STENCILREF, 0x1);
    pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1); // 알파가 1 이상인 것만 통과
    pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    // 2. 색상과 깊이 기록은 끔 (틀만 잡기 위함)
    pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_WasabiGaugeStancil"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
    m_pBufferCom->Render_Buffer();

    pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);

    // 2. 스텐실 판정: 기록된 '1' 영역에만 그리기
    pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Orange"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    _matrix newMat = *m_pTransformCom->Get_World();
    newMat.m[3][1] = gauge;

    pGraphicDev->SetTransform(D3DTS_WORLD, &newMat);
    m_pBufferCom->Render_Buffer();


    pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_WasabiIcon"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    matTmp = *m_pTransformCom->Get_World();
    matTmp.m[3][1] += 2.f;
    matTmp.m[3][0] -= 0.2f;
    matTmp.m[0][0] = 0.15f;
    matTmp.m[1][1] = 0.15f;

    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
    m_pBufferCom->Render_Buffer();

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CWasabiObject::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_WasabiObjectTex", L"Com_Texture", &m_pTextureCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
    m_pTransformCom->m_vScale = { 0.9f, 0.62f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 6.9f, -1.2f, -2.8f };

    return S_OK;
}




CWasabiObject* CWasabiObject::Create()
{
    CWasabiObject* pBackGround = new CWasabiObject;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("wasabi object Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CWasabiObject::Free()
{
    Safe_Release(m_pAABB);

    CGameObject::Free();
}
