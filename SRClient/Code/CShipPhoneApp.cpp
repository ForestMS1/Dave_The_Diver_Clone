#include "pch.h"
#include "CShipPhoneApp.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CManagement.h"
#include "CHelper.h"
#include "CDInputMgr.h"
#include "CShipPhone.h"

CShipPhoneApp::CShipPhoneApp(_uint iAppIdx, _float fPosX, _float fPosY)
    : CGameObject()
    , m_iAppIdx(iAppIdx)
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_bSelect(false)
{
}

CShipPhoneApp::~CShipPhoneApp()
{
}


HRESULT		CShipPhoneApp::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };

    _vec3 vScale = { 0.3f , 0.3f, 0.3f };

    _vec3 vPos = { 0.0f, 0.0f, 0.0f };
    m_pTransformCom->Set_Scale(&vScale);
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    _vec3 vOri = { 0.f, 0.f, 0.f };
    m_pOBB = COBB::Create(&vPos, &vExtents, &vOri, L"OBB_PhoneApp", this);

    return S_OK;
}

_int		CShipPhoneApp::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    if (m_bSelect)
    {
        _vec3 vScale = { 0.35f , 0.48125f, 0.35f };
        m_pTransformCom->Set_Scale(&vScale);
    }
    else
    {
        _vec3 vScale = { 0.3f , 0.4125f, 0.3f };
        m_pTransformCom->Set_Scale(&vScale);
    }

    if (CGameObject* pParent = Get_Parent())
    {
        if (auto* pTransform = pParent->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform"))
        {
            //_matrix parentWorld = *pTransform->Get_World();
            //parentWorld._11 = 1.f;
            //parentWorld._22 = 1.f;
            //parentWorld._33 = 1.f;
            //_matrix myWorld = *m_pTransformCom->Get_World();
            //m_matNewWorld = myWorld * parentWorld;

            _matrix parentWorld = *pTransform->Get_World();

            // 2. 부모 행렬에서 스케일 제거 (각 축을 정규화하여 회전만 남김)
            _vec3 vRight = { parentWorld._11, parentWorld._12, parentWorld._13 };
            _vec3 vUp = { parentWorld._21, parentWorld._22, parentWorld._23 };
            _vec3 vLook = { parentWorld._31, parentWorld._32, parentWorld._33 };

            D3DXVec3Normalize(&vRight, &vRight);
            D3DXVec3Normalize(&vUp, &vUp);
            D3DXVec3Normalize(&vLook, &vLook);

            parentWorld._11 = vRight.x; parentWorld._12 = vRight.y; parentWorld._13 = vRight.z;
            parentWorld._21 = vUp.x;    parentWorld._22 = vUp.y;    parentWorld._23 = vUp.z;
            parentWorld._31 = vLook.x;  parentWorld._32 = vLook.y;  parentWorld._33 = vLook.z;

            // 3. 자식의 '상대적' 변환 행렬 (부모 기준 위치/회전/크기)
            _matrix myLocal = *m_pTransformCom->Get_World(); 
            
            // 4. 최종 결합
            m_matNewWorld = myLocal * parentWorld;
        }
    }

    // 충돌체 그룹에 넣어줘야한다.
    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Phone", m_pOBB);
    m_pOBB->Transform(&m_matNewWorld);

    return iExit;
}

void		CShipPhoneApp::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    // 레이와 충돌 체크
    _vec3 vRayPos, vRayDir;
    CHelper::GetMousePointRay(&vRayPos, &vRayDir);
    float fDist{};
    if (m_pOBB->Intersect(&vRayPos, &vRayDir, fDist))
    {
        // Some Logic
        if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
        {
            // 만약 충돌한다면 태그비교후 보이드포인터 들고와서 캐스팅한다음 조작한다.
            if (m_pOBB->Get_Tag() == L"OBB_PhoneApp")
            {
                if (reinterpret_cast<CShipPhoneApp*>(m_pOBB->Get_VoidPtr())->Get_Select())
                {
                    if (auto pPhone = dynamic_cast<CShipPhone*>(m_pParentGameObject))
                    {
                        pPhone->Focus_App(L"1");
                    }
                }
                else
                {
                    for (auto pChild : *m_pParentGameObject->Get_Children())
                    {
                        if (auto pApp = dynamic_cast<CShipPhoneApp*>(pChild))
                        {
                            pApp->Set_Select(false);
                        }
                    }

                    reinterpret_cast<CShipPhoneApp*>(m_pOBB->Get_VoidPtr())->Set_Select(true);
                }
            }
        }
    }
}

void		CShipPhoneApp::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    // 2. 투명도 설정 (0~255) - 예: 128은 약 50% 투명도
    float alpha = 0.5f;
    DWORD alphaValue = (DWORD)(alpha * 255);
    pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(alphaValue, 255, 255, 255));

    // 3. 텍스쳐의 RGB는 그대로 쓰고, 알파는 TFACTOR에서 가져오도록 설정
    //pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    //pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

    //pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    //pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

    pGraphicDev->SetTransform(D3DTS_WORLD, &m_matNewWorld);


    wstring sTexName;

    if (m_bSelect)
    {
        sTexName = L"Tex_PhoneApp";
    }
    else
    {
        sTexName = L"Tex_PhoneAppAlpha";
    }

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(sTexName))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(m_iAppIdx)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }


    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT			CShipPhoneApp::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}
CShipPhoneApp* CShipPhoneApp::Create(_uint iAppIdx, _float fPosX, _float fPosY)
{
    CShipPhoneApp* pShipPhoneApp = new CShipPhoneApp{ iAppIdx, fPosX, fPosY };

    if (FAILED(pShipPhoneApp->Ready_GameObject()))
    {
        Safe_Release(pShipPhoneApp);
        MSG_BOX("pShipPhoneApp Create Failed");
        return nullptr;
    }

    return pShipPhoneApp;
}

void CShipPhoneApp::Free()
{
    CGameObject::Free();
    Safe_Release(m_pOBB);
}
