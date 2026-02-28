#include "CJohn2Idle.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn2.h"
CJohn2Idle::CJohn2Idle(CJohn2* pOwner)
	: CBaseState<CJohn2>(pOwner)
{
}

CJohn2Idle::~CJohn2Idle()
{
}

void CJohn2Idle::Enter()
{
	m_pOwner->Init_Frame();

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2Idle")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohn2Idle::Input(const _float& fTimeDelta)
{
}

_int CJohn2Idle::Update_State(const _float& fTimeDelta)
{
	//m_pOwner->EncounterTarget();

	m_pOwner->AddFrame(fTimeDelta, 10.f, 8);

	m_pOwner->Update_ToTargetDir();

	_vec3 vRotDir;
	if (m_pOwner->Get_ToTargetDir().x > 0.f)
		vRotDir = { 0.f, 0.f, 0.f };
	else
		vRotDir = { 0.f,-180.f, 0.f };
	m_pOwner->Set_RotateDir(&vRotDir);

	m_fBreakTime += fTimeDelta;

	if (m_fBreakTime > 3.f)
	{
		if (!m_pOwner->Check_TargetInRange(8.f))
		{
			m_pOwner->Set_State(JOHN2STATE::CHASE);
		}
		else
		{
			//1페이즈
			if (m_pOwner->Get_Hp() >= 100.f)
				m_iRand = (m_iRand + 1) % 2;
			else // 2페이즈 (체력 50%이하)
				m_iRand = ((m_iRand + 1) % 3) + 1;

			switch (m_iRand)
			{
			case 0:
				// 러쉬 준비
				m_pOwner->Set_State(JOHN2STATE::MELEEATTACK_READY);
				break;
			case 1:
				// 총쏘기 준비
				m_pOwner->Set_State(JOHN2STATE::ATTACK_READY);
				break;
			case 2:
				m_pOwner->Set_State(JOHN2STATE::SPLASH_MINE);
				break;
			default:
				break;
			}
		}
	}
	return 0;
}

void CJohn2Idle::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohn2Idle::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"John2Idle"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohn2Idle::Exit()
{
	Clear();
}

void CJohn2Idle::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2Idle")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	m_fBreakTime = 0.f;
}

CJohn2Idle* CJohn2Idle::Create(CJohn2* pOwner)
{
	CJohn2Idle* pState = new CJohn2Idle(pOwner);

	return pState;
}

void CJohn2Idle::Free()
{
}
