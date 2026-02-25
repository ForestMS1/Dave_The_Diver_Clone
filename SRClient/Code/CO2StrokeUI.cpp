#include "CO2StrokeUI.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CO2StrokeUI::CO2StrokeUI()
{
}

CO2StrokeUI::CO2StrokeUI(const CO2StrokeUI& rhs)
    :CGameObject(rhs)
{
}

CO2StrokeUI::~CO2StrokeUI()
{
}

HRESULT CO2StrokeUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 59.f, 59.f, 5.f };

    m_pTransformCom->Set_Scale(&vScale);
    m_pTransformCom->Set_Pos(-536.f, -265.f, 5.f);

    return S_OK;
}

_int CO2StrokeUI::Update_GameObject(const _float& fTimeDelta)
{
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    CGameObject::Update_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
    return 0;
}

void CO2StrokeUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CO2StrokeUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_O2Stroke"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    m_pBufferCom->Render_Buffer();
    //--------------------------------------------------------------------------------------------------------
}


//void      CLogoBG::Render_GameObject()
//{
//    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
//
//    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//
//
//
//    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Logo_BG"))
//    {
//        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
//        {
//            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
//        }
//    }
//    m_pBufferCom->Render_Buffer();
//
//
//    pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
//    pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
//    pGraphicDev->SetRenderState(D3DRS_STENCILREF, 0x1);
//    pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
//
//    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//    pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x01); // 알파가 1 이상인 것만 통과
//    pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
//
//    // 2. 색상과 깊이 기록은 끔 (틀만 잡기 위함)
//    pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
//    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//
//    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Bubble"))
//    {
//        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
//        {
//            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
//        }
//    }
//    _matrix mat;
//    D3DXMatrixIdentity(&mat);
//    pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
//    m_pBufferCom->Render_Buffer();
//
//    pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
//
//    // 2. 스텐실 판정: 기록된 '1' 영역에만 그리기
//    pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
//    pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
//
//    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Logo_BG"))
//    {
//        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
//        {
//            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
//        }
//    }
//
//
//
//    m_pBufferCom->Render_Buffer();
//
//    D3DXMATRIX matTmp;
//    D3DXMatrixIdentity(&matTmp);
//    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
//
//    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//
//
//
//
//    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_test"))
//    {
//        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
//        {
//            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
//        }
//    }
//
//    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
//    m_pBufferCom->Render_Buffer();
//
//
//    pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
//    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//}

HRESULT CO2StrokeUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CO2StrokeUI* CO2StrokeUI::Create()
{
    CO2StrokeUI* pUI = new CO2StrokeUI;
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("DaggerBoxUI Create failed");
        return nullptr;
    }

    return pUI;
}

void CO2StrokeUI::Free()
{
    CGameObject::Free();
}
