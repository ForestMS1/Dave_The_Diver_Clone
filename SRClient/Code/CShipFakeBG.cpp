#include "pch.h"
#include "CShipFakeBG.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CGameMemMgr.h"

CShipFakeBG::CShipFakeBG(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CShipFakeBG::~CShipFakeBG()
{
}


HRESULT		CShipFakeBG::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    wstring sShipAssetName;
    if (CGameMemMgr::GetInstance()->Get_ShipNight())
    {
        sShipAssetName = L"Tex_Ship_NightFakeBG";
    }
    else
    {
        sShipAssetName = L"Tex_Ship_FakeBG";
    }
    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(sShipAssetName))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 180.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 180.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    return S_OK;
}

_int		CShipFakeBG::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CShipFakeBG::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipFakeBG::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    wstring sShipAssetName;
    if (CGameMemMgr::GetInstance()->Get_ShipNight())
    {
        sShipAssetName = L"Tex_Ship_NightFakeBG";
    }
    else
    {
        sShipAssetName = L"Tex_Ship_FakeBG";
    }
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(sShipAssetName))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT			CShipFakeBG::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipFakeBG* CShipFakeBG::Create(float fPosX, float fPosY)
{
    CShipFakeBG* pShipFakeBG = new CShipFakeBG{ fPosX , fPosY };

    if (FAILED(pShipFakeBG->Ready_GameObject()))
    {
        Safe_Release(pShipFakeBG);
        MSG_BOX("pShipFakeBG Create Failed");
        return nullptr;
    }

    return pShipFakeBG;
}

void CShipFakeBG::Free()
{
    CGameObject::Free();
}