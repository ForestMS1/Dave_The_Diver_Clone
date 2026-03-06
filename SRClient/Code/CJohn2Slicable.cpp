#include "pch.h"
#include "CJohn2Slicable.h"
#include "CJohn2.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CColliderMgr.h"
#include "CDiveDave.h"
#include "CManagement.h"
#include "CSoundMgr.h"
CJohn2Slicable::CJohn2Slicable(CJohn2* pOwner)
	:CBaseState<CJohn2>(pOwner)
{
}

CJohn2Slicable::~CJohn2Slicable()
{
}

void CJohn2Slicable::Enter()
{
	m_pOwner->Init_Frame();
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_Slicable")->at(0))->Get_ImgInfo();

	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	vScale *= 0.4f;
	m_pOwner->Multiply_Scale(&vScale);

	m_vRotDir = { 0.f, 0.f, 0.f };

    m_tween = m_tween.from(1.f).to(0.7f).during(300).via(tweeny::easing::linear);

    CSoundMgr::GetInstance()->StopSound(CSoundMgr::BOSS_BGM);
    CSoundMgr::GetInstance()->PlaySoundLoop(L"Sound_DiveInGameBGM", CSoundMgr::CHANNELID::BGM, 1.f);
	CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Boss_Die", CSoundMgr::BOSS_SFX1, 1.f);
}

void CJohn2Slicable::Input(const _float& fTimeDelta)
{

}

_int CJohn2Slicable::Update_State(const _float& fTimeDelta)
{
	if (m_vRotDir.z < 90.f)
	{
		m_vRotDir.z += fTimeDelta * 30.f;
		m_pOwner->Set_RotateDir(&m_vRotDir);
	}
    m_fAlpha = m_tween.step(int(fTimeDelta * 1000.f));

	m_pOwner->Set_ViewZ(10.5f);

	return 0;
}

void CJohn2Slicable::LateUpdate_State(const _float& fTimeDelta)
{
    // 데이브가 물고기 디텍스박스 충돌 체크
    if (auto pDaveExistsColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_DiveDaveWithItemBox"))
    {
        for (auto pDaveCollider : *pDaveExistsColliders)
        {
            if (pDaveCollider->Get_Tag() == L"AABB_DiveDaveWithItemBox")
            {
                CDiveDave* pDave = reinterpret_cast<CDiveDave*>(pDaveCollider->Get_VoidPtr());
				if (pDave == nullptr)
				{
					return;
				}
                // 데이브와 물고기 디텍트 박스 충돌체크
                if (auto pFishDetectBoxColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_JohnWithGuided"))
                {
                    for (auto pFishDetectBoxCollider : *pFishDetectBoxColliders)
                    {
                        if (pFishDetectBoxCollider->Get_Tag() == L"AABB_John2WithGuided")
                        {
                            //CFishGameObject* pFish = reinterpret_cast<CFishGameObject*>(pFishDetectBoxCollider->Get_VoidPtr());
                            if (pDaveCollider->Intersect(pFishDetectBoxCollider))
                            {
                                pDave->Set_SlicableJohn(m_pOwner);
                            }
                            else
                            {
                                pDave->Set_SlicableJohn(nullptr);
                            }
                        }
                    }
                }  
            }
            break;
        }
    }

}

void CJohn2Slicable::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    float alphaRatio = m_fAlpha;
    DWORD tfactor = D3DCOLOR_ARGB(
        (BYTE)(alphaRatio * 255.f),
        255, 255, 255
    );
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tfactor);

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_Slicable"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    // COLOR = Texture * TFACTOR
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

    // ALPHA = TextureAlpha * TFACTORAlpha
    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    // 원복은 CJohn2에서
}

void CJohn2Slicable::Exit()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_Slicable")->at(0))->Get_ImgInfo();

	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;
	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	vScale /= 0.4f;
	m_pOwner->Multiply_Scale(&vScale);

	Clear();
}

void CJohn2Slicable::Clear()
{
    CDiveDave* pDave = dynamic_cast<CDiveDave*>
        (CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDave"));

    if (pDave != nullptr)
    {
        pDave->Set_SlicableJohn(nullptr);
    }
}


CJohn2Slicable* CJohn2Slicable::Create(CJohn2* pOwner)
{
	CJohn2Slicable* pState = new CJohn2Slicable(pOwner);

	return pState;
}

void CJohn2Slicable::Free()
{
}
