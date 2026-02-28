#include "CJohn2NoStart.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn2.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CManagement.h"
CJohn2NoStart::CJohn2NoStart(CJohn2* pOwner)
	: CBaseState<CJohn2>(pOwner)
{
}

CJohn2NoStart::~CJohn2NoStart()
{
}

void CJohn2NoStart::Enter()
{
	m_pOwner->Init_Frame();

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2Angry")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohn2NoStart::Input(const _float& fTimeDelta)
{
}

_int CJohn2NoStart::Update_State(const _float& fTimeDelta)
{
	m_pOwner->Update_ToTargetDir();

	if (m_pOwner->Check_TargetInRange(10.f))
	{
		_vec3 vRotDir;
		if (m_pOwner->Get_ToTargetDir().x > 0.f)
			vRotDir = { 0.f, 0.f, 0.f };
		else
			vRotDir = { 0.f,-180.f, 0.f };
		m_pOwner->Set_RotateDir(&vRotDir);


		m_pOwner->AddFrame(fTimeDelta, 5.f, 8, false);
		CDiveDaveCam* pCam = static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_CurCamera());
		_vec3 vCurPos;
		m_pOwner->Get_Pos(&vCurPos);
		pCam->Set_Target(&m_pOwner->GetTransformCom()->m_vInfo[INFO_POS]);
	}


	if (m_pOwner->Get_Frame() == 7)
	{
		m_pOwner->EncounterTarget();
		m_pOwner->Set_State(JOHN2STATE::IDLE);
	}

	return 0;
}

void CJohn2NoStart::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohn2NoStart::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"John2Angry"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohn2NoStart::Exit()
{
	CDiveDaveCam* pCam = static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_CurCamera());
	CTransform* pDiveTransform = static_cast<CTransform*>
		(CManagement::GetInstance()->Get_FirstObjectComponent(ID_DYNAMIC, L"0_GameLogic_Layer", L"DiveDave", L"Com_Transform"));
	pCam->Set_Target(&pDiveTransform->m_vInfo[INFO_POS]);

	Clear();
}

void CJohn2NoStart::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2Angry")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

CJohn2NoStart* CJohn2NoStart::Create(CJohn2* pOwner)
{
	CJohn2NoStart* pState = new CJohn2NoStart(pOwner);

	return pState;
}

void CJohn2NoStart::Free()
{
}
