#include "pch.h"
#include "CSmallMenu.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CAssetDefaultFont.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CSmallMenu::CSmallMenu()
    : CGameObject()
{
    sushiTex = L"";
    fishName = L"";
}

CSmallMenu::CSmallMenu(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CSmallMenu::~CSmallMenu()
{
}

HRESULT CSmallMenu::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    return S_OK;
}

_int CSmallMenu::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    _vec3 curPos;
    float xOffset, yOffset;
    m_pTransformCom->Get_Info(INFO_POS, &curPos);
    xOffset = 0.05f;
    yOffset = -0.02f;
    curPos.x -= xOffset;
    curPos.y -= yOffset;
    CHelper::GetScreenPointFromWorld(&screen, &curPos);
    yOffset = 0.4f;
    curPos.y -= yOffset;
    CHelper::GetScreenPointFromWorld(&screen1, &curPos);
 
    if (sushiTex == L"") {
        if (fishName == L"블루종") {
            sushiTex = L"Tex_Bluejong";
        }
        else if (fishName == L"노랑백") {
            sushiTex = L"Tex_YellowBack";
        }
        else if (fishName == L"코반아지") {
            sushiTex = L"Tex_Dart";
        }
        else if (fishName == L"흰동가리") {
            sushiTex = L"Tex_ClownFish";
        }
        else if (fishName == L"노랑탕") {
            sushiTex = L"Tex_YellowTang";
        }
        else if (fishName == L"???") {
            sushiTex = L"Tex_BanchoSushi";
        }
    }
  

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CSmallMenu::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);
}

void CSmallMenu::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


    CAssetDefaultFont* pNumberFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_SushiNumber");

    //가격
    _vec2 vPos1 = { screen.x, screen.y };
    _vec2 vPos2 = { screen1.x, screen1.y };
    if (stoi(CurQuantity) == 0) {
        pNumberFont->Render_Font(CurQuantity, &vPos1, D3DXCOLOR(255 / 255.f, 0 / 255.f, 0 / 255.f, 1.f));
   
        pNumberFont->Render_Font(totalQuantity, &vPos2, D3DXCOLOR(255 / 255.f, 0 / 255.f, 0 / 255.f, 1.f));
    }
    else {
        pNumberFont->Render_Font(CurQuantity, &vPos1, D3DXCOLOR(255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f));
 
        pNumberFont->Render_Font(totalQuantity, &vPos2, D3DXCOLOR(255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f));
        
    }

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(sushiTex))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    matTmp = *m_pTransformCom->Get_World();
    matTmp.m[0][0] = 0.2f;
    matTmp.m[1][1] = 0.2f;
    matTmp.m[3][1] = 0.25f;

    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
    m_pBufferCom->Render_Buffer();

}

HRESULT CSmallMenu::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_SmallMenu", L"Com_Texture", &m_pTextureCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 0.3f, 0.8f,1.f };
    return S_OK;
}


CSmallMenu* CSmallMenu::Create()
{
    CSmallMenu* pBackGround = new CSmallMenu;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("SmallMenu Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CSmallMenu::Free()
{
    CGameObject::Free();
}
