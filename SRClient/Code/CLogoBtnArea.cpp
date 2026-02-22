#include "pch.h"
#include "CLogoBtnArea.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include "CTransition.h"

CLogoBtnArea::CLogoBtnArea(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CLogoBtnArea::~CLogoBtnArea()
{
}


HRESULT		CLogoBtnArea::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 2.f , 0.25f, 1.f };
    //if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Logo_BG"))
    //{
    //    if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
    //    {
    //        float fWidth = pTexture->Get_ImgInfo()->Width / 1920.f;
    //        float fHeight = pTexture->Get_ImgInfo()->Height / 1080.f;
    //        vScale = { fWidth, fHeight, 1.f };
    //    }
    //}

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    _vec3 vPos = { 0.f, -0.1f, 0.f };
    _vec3 vExtents = { 0.1f, 0.04f, 0.001f };
    for (int i = 0; i < 5; ++i)
    {
        vPos.y -= 0.1f;
        m_pAABB[i] = CAABB::Create(&vPos, &vExtents, L"AABB_Btn_"+to_wstring(i));
    }

    m_iSelected = 9999;
    return S_OK;
}

_int		CLogoBtnArea::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    for (int i = 0; i < 5; ++i)
    {
        CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_LogoBtnArea", m_pAABB[i]);
    }
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CLogoBtnArea::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
    {
        _vec3 vRayPos, vRayDir;
        CHelper::GetMousePointRay(&vRayPos, &vRayDir);
        for (int i = 0; i < 5; ++i)
        {
            float fDist{};
            if (m_pAABB[i]->Intersect(&vRayPos, &vRayDir, fDist))
            {
                _uint iBeforeIdx = m_iSelected;
                if (m_pAABB[i]->Get_Tag() == L"AABB_Btn_0")
                {
                    m_iSelected = 0;
                }
                else if (m_pAABB[i]->Get_Tag() == L"AABB_Btn_1")
                {
                    m_iSelected = 1;
                }
                else if (m_pAABB[i]->Get_Tag() == L"AABB_Btn_2")
                {
                    m_iSelected = 2;
                }
                else if (m_pAABB[i]->Get_Tag() == L"AABB_Btn_3")
                {
                    m_iSelected = 3;
                }
                else if (m_pAABB[i]->Get_Tag() == L"AABB_Btn_4")
                {
                    m_iSelected = 4;
                }

                if (iBeforeIdx == m_iSelected)
                {
                    CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_LOGO, CTransition::SCENE_SHIP));
                }
            }
        }


    }
    
}

void		CLogoBtnArea::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Logo_Black1pxAlpha"))
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


    //  0 새 게임
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.0f;
        float fOffsetY = 0.2f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            D3DXCOLOR col = D3DXCOLOR(1.f, 1.f, 1.f, 0.5f);
            if (m_iSelected == 0)
            {
                col = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
            }
            pDefFont->Render_Font(L"새 게임", &vPos, col, (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // 1 이어서 하기
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.0f;
        float fOffsetY = 0.1f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            D3DXCOLOR col = D3DXCOLOR(1.f, 1.f, 1.f, 0.5f);
            if (m_iSelected ==  1)
            {
                col = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
            }
            pDefFont->Render_Font(L"이어서 하기", &vPos, col, (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // 2 불러오기
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.0f;
        float fOffsetY = 0.0f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            D3DXCOLOR col = D3DXCOLOR(1.f, 1.f, 1.f, 0.5f);
            if (m_iSelected == 2)
            {
                col = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
            }
            pDefFont->Render_Font(L"불러오기", &vPos, col, (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // 3 설정
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.f;
        float fOffsetY = -0.1f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            D3DXCOLOR col = D3DXCOLOR(1.f, 1.f, 1.f, 0.5f);
            if (m_iSelected == 3)
            {
                col = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
            }
            pDefFont->Render_Font(L"설정", &vPos, col, (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // 4 나가기
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.0f;
        float fOffsetY = -0.2f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            D3DXCOLOR col = D3DXCOLOR(1.f, 1.f, 1.f, 0.5f);
            if (m_iSelected == 4)
            {
                col = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
            }
            pDefFont->Render_Font(L"나가기", &vPos, col, (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

}

HRESULT			CLogoBtnArea::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CLogoBtnArea* CLogoBtnArea::Create(float fPosX, float fPosY)
{
    CLogoBtnArea* pBtnArea = new CLogoBtnArea{ fPosX , fPosY };

    if (FAILED(pBtnArea->Ready_GameObject()))
    {
        Safe_Release(pBtnArea);
        MSG_BOX("pBtnArea Create Failed");
        return nullptr;
    }

    return pBtnArea;
}

void CLogoBtnArea::Free()
{
    CGameObject::Free();
    for (int i = 0; i < 5; ++i)
    {
        Safe_Release(m_pAABB[i]);
    }
}