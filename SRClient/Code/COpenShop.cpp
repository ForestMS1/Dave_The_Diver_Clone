#include "pch.h"
#include "COpenShop.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CSushi.h"
#include "CDInputMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CSoundMgr.h"
COpenShop::COpenShop()
    : CGameObject()
{
}

COpenShop::COpenShop(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

COpenShop::~COpenShop()
{
}

HRESULT COpenShop::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f,1.0f,-6.f };
    m_pTransformCom->m_vScale = { 4.2f,0.25f,1.f };

    return S_OK;
}

_int COpenShop::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender && !openPressed) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        Key_Input();
    }
    else if (m_bRender && openPressed) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        openTime += fTimeDelta;
        scale += fTimeDelta;
        if (openTime > 3.f) {
            m_bDead = true;
        }
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void COpenShop::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);
    }

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);
}

void COpenShop::Render_GameObject()
{
    if (m_bRender && !openPressed) {
        //LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
        //float alphaRatio = 0.7f;
        //DWORD tfactor = D3DCOLOR_ARGB(
        //    (BYTE)(alphaRatio * 255.f),
        //    255, 255, 255
        //);
        //pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        //pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        //pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        //pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tfactor);

        //pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        //m_pTextureCom->Set_Texture(0);
        //// COLOR = Texture * TFACTOR
        //pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        //pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        //pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

        //// ALPHA = TextureAlpha * TFACTORAlpha
        //pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        //pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        //pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
        //m_pBufferCom->Render_Buffer();

        //pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        //pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
        //pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        //pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

        m_pTextureCom->Set_Texture(0);

        m_pBufferCom->Render_Buffer();

        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


        

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    }
    else if (m_bRender && openPressed) {

        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_OpenImage"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        _matrix matTmp;
        matTmp = *m_pTransformCom->Get_World();
        //D3DXMatrixIdentity(&matTmp);
        matTmp.m[0][0] = scale * 0.1f;
        matTmp.m[1][1] = scale * 0.1f;
        matTmp.m[3][0] = 0;
        matTmp.m[3][1] = 0;
        matTmp.m[3][2] -= 0.01f;
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    }
   
}

HRESULT COpenShop::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_OpenShopTex", L"Com_Texture", &m_pTextureCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 17.0f, 8.f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 1.4f, 6.f };
    return S_OK;
}


COpenShop* COpenShop::Create()
{
    COpenShop* pBackGround = new COpenShop;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("OpenShop Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void COpenShop::Free()
{
    CGameObject::Free();
}

void COpenShop::Key_Input()
{
    if (CDInputMgr::GetInstance()->Key_Down(DIKEYBOARD_E))
    {
        CScene* scene = CManagement::GetInstance()->Get_Scene();
        CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_OpenShop", CSoundMgr::SFX, 1.0f);
    
        static_cast<CSushi*>(scene)->Set_Open(true);
        openPressed = true;
        //m_bDead = true;
    }
}
