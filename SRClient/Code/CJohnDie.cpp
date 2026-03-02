#include "pch.h"
#include "CJohnDie.h"
#include "CJohn.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CDiveDaveCam.h"
#include "CCameraMgr.h"
#include "CManagement.h"
#include "CJohn2.h"
CJohnDie::CJohnDie(CJohn* pOwner)
	:CBaseState<CJohn>(pOwner)
{
}

CJohnDie::~CJohnDie()
{
}

void CJohnDie::Enter()
{
	m_pOwner->Init_Frame();
	//D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnSurprised")->at(0))->Get_ImgInfo();
	//imgInfo.Width;
	m_wsTexName = L"JohnSurprised";

	_float fWidth = 46;
	_float fHeight = 60;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	CDiveDaveCam* pCam = static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_CurCamera());
	m_pOwner->Get_Pos(&m_vAt);
	pCam->Set_Target(&m_vAt);
}

void CJohnDie::Input(const _float& fTimeDelta)
{

}

_int CJohnDie::Update_State(const _float& fTimeDelta)
{
	if (m_wsTexName == L"Tex_JohnMoveSide")
	{
		m_pOwner->AddFrame(fTimeDelta, 10.f, 8, true);
		_vec3 vDir = { 1.f, 0.f, 0.f };
		m_pOwner->CJohn::Move(&vDir, fTimeDelta);
		_float fSpeed = m_pOwner->Get_Speed();
		m_fAccMoveDist += fTimeDelta * fSpeed;
	}
	else
		m_pOwner->AddFrame(fTimeDelta, 3.f, 8, false);


	if (m_pOwner->Get_Frame() == 7)
	{
		if (m_bChangeMotion == false)
		{
			m_pOwner->Init_Frame();
			m_bChangeMotion = true;
			m_wsTexName = L"Tex_JohnMoveSide";
			_float fWidth = 46;
			_float fHeight = 60;
			_float fAspect = fWidth + fHeight;
			fAspect /= 2.f;
			_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
			m_pOwner->Multiply_Scale(&vScale);
			
			D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();

			fWidth = imgInfo.Width;
			fHeight = imgInfo.Height;
			fAspect = fWidth + fHeight;
			fAspect /= 2.f;

			vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
			m_pOwner->Multiply_Scale(&vScale);

			_vec3 vRot = { 0.f, 0.f, 0.f };
			m_pOwner->Set_RotateDir(&vRot);
			CDiveDaveCam* pCam = static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_CurCamera());
			CTransform* pDiveTransform = static_cast<CTransform*>
				(CManagement::GetInstance()->Get_FirstObjectComponent(ID_DYNAMIC, L"0_GameLogic_Layer", L"DiveDave", L"Com_Transform"));
			pCam->Set_Target(&pDiveTransform->m_vInfo[INFO_POS]);
		}

	}
	if (m_fAccMoveDist > 10.f)
	{
		Exit();
		m_pOwner->Set_Dead();
	}
	return 0;
}

void CJohnDie::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohnDie::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_wsTexName))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohnDie::Exit()
{
	_vec3 vCurPos;
	m_pOwner->GetTransformCom()->Get_Info(INFO_POS, &vCurPos);
	CJohn2* pJohn2 = CJohn2::Create(vCurPos.x, vCurPos.y, vCurPos.z);
	CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"John2", pJohn2);

	Clear();
}

void CJohnDie::Clear()
{


	m_pOwner->Hit_Free();
	m_pOwner->Reset_HitTime();
}


CJohnDie* CJohnDie::Create(CJohn* pOwner)
{
	CJohnDie* pState = new CJohnDie(pOwner);

	return pState;
}

void CJohnDie::Free()
{
}
