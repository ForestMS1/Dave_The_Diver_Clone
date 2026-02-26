#include "pch.h"
#include "CMenuBubble.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CGameMemMgr.h"
CMenuBubble::CMenuBubble()
    : CGameObject()
{
}

CMenuBubble::CMenuBubble(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CMenuBubble::~CMenuBubble()
{
}

void CMenuBubble::Update_ImGui()
{
    CGameObject::Update_ImGui();
    ImGui::DragFloat("tempY", &tempY, 0.01f);
}

HRESULT CMenuBubble::Ready_GameObject()
{
    if (FAILED(Ready_Component()))  
        return E_FAIL;

    m_pTransformCom->m_vScale = { 0.4f,0.4f,1.f };

    vector<CGameMemMgr::FISH*> menu = CGameMemMgr::GetInstance()->getMenu();

    int random = rand() % menu.size();
    CGameMemMgr::FISH* fish = menu[random];
    m_sFishName = fish->name;
    CGameMemMgr::GetInstance()->reduceMenu(fish->name);

    if (m_sFishName == L"블루종") {
        m_sTexName = L"Tex_Bluejong";
    }
    else if (m_sFishName == L"노랑탕") {
        m_sTexName = L"Tex_YellowTang";

    }
    else if (m_sFishName == L"코반아지") {
        m_sTexName = L"Tex_Dart";

    }
    else if (m_sFishName == L"노랑백") {
        m_sTexName = L"Tex_YellowBack";

    }
    else if (m_sFishName == L"흰동가리") {
        m_sTexName = L"Tex_ClownFish";
    }
    CGameMemMgr::GetInstance()->addCookingMenu(m_sFishName);

    return S_OK;
}

_int CMenuBubble::Update_GameObject(const _float& fTimeDelta)
{
 
 
    if (!m_bRender) {
        deltaTime += fTimeDelta;
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        if (deltaTime >= 2.f) {
            if (tempY <= 0) {
                tempY += fTimeDelta * 0.05f;
            }
        }
       
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void CMenuBubble::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}


void      CMenuBubble::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    m_pMenuBubbleTextureCom->Set_Texture(0);
    m_pBufferCom->Render_Buffer();
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


    pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    pGraphicDev->SetRenderState(D3DRS_STENCILREF, 0x1);
    pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 140); // 알파가 1 이상인 것만 통과
    pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    // 2. 색상과 깊이 기록은 끔 (틀만 잡기 위함)
    pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
   // pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    //if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_MenuBubble"))
    //{
    //    if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
    //    {
    //        pGraphicDev->SetTexture(0, pTexture->Get_Texture());
    //    }
    //}
    //_matrix mat;
    //D3DXMatrixIdentity(&mat);
    //pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
    m_pBufferCom->Render_Buffer();

    pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);

    // 2. 스텐실 판정: 기록된 '1' 영역에만 그리기
    pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

    //if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_MenuBubble"))
    //{
    //    if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
    //    {
    //        pGraphicDev->SetTexture(0, pTexture->Get_Texture());
    //    }
    //}



    //m_pBufferCom->Render_Buffer();

    //D3DXMATRIX matTmp;
    //D3DXMatrixIdentity(&matTmp);
    //pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Red"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    _matrix newMat = *m_pTransformCom->Get_World();
    newMat.m[3][1] += tempY;
    
    pGraphicDev->SetTransform(D3DTS_WORLD, &newMat);
    m_pBufferCom->Render_Buffer();


    pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sTexName))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    _matrix scaleMat = *m_pTransformCom->Get_World();
    scaleMat.m[0][0] = 0.2f;
    scaleMat.m[1][1] = 0.2f;
    scaleMat.m[2][2] = 1.f;

    pGraphicDev->SetTransform(D3DTS_WORLD, &scaleMat);
    m_pBufferCom->Render_Buffer();
}

HRESULT CMenuBubble::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_MenuBubbleTex", L"Com_Texture", &m_pMenuBubbleTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 6.f, 6.f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 0,0,0 };
    return S_OK;
}


CMenuBubble* CMenuBubble::Create()
{
    CMenuBubble* overlay = new CMenuBubble;

    if (FAILED(overlay->Ready_GameObject()))
    {
        Safe_Release(overlay);
        MSG_BOX("overlay Create Failed");
        return nullptr;
    }

    return overlay;
}

void CMenuBubble::Free()
{
    CGameObject::Free();
}
