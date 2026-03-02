#include "CJohnMeleeAttack.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn.h"
#include "CColliderMgr.h"
CJohnMeleeAttack::CJohnMeleeAttack(CJohn* pOwner)
	: CBaseState<CJohn>(pOwner)
{
}

CJohnMeleeAttack::~CJohnMeleeAttack()
{
}

void CJohnMeleeAttack::Enter()
{
	m_pOwner->Init_Frame();
	m_wsTexName = L"MeleeSkill01_Attack_01";

	Set_Size();

	m_pOwner->Update_ToTargetDir();
}

void CJohnMeleeAttack::Input(const _float& fTimeDelta)
{
}

_int CJohnMeleeAttack::Update_State(const _float& fTimeDelta)
{
	if (m_bGoComplete == true)
	{
		m_pOwner->AddFrame(fTimeDelta, 3.f, 3, false);
		//씝 하드코딩
		if (!m_bCompleteFramSizeCalc && (_uint)m_pOwner->Get_Frame() == 1)
		{
			D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();
			imgInfo.Width;

			_float fWidth = imgInfo.Width;;
			_float fHeight = imgInfo.Height;
			_float fAspect = fWidth + fHeight;
			fAspect /= 2.f;

			_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
			m_pOwner->Multiply_Scale(&vScale);

			fWidth = 40.f;
			fHeight = 59.f;

			vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
			m_pOwner->Multiply_Scale(&vScale);

			m_bCompleteFramSizeCalc = true;
		}
		if(m_pOwner->Get_Frame() >= 2)
			m_pOwner->Set_State(JOHNSTATE::IDLE);
		return 0;
	}

	m_pOwner->AddFrame(fTimeDelta, 10.f, 5, false);
	m_bGoComplete = m_pOwner->Rush_ToTarget(fTimeDelta);

	if (m_bGoComplete == true)
	{
		Reset_Size();
		m_wsTexName = L"MeleeSkill01_Attack_02";
		Set_Size();
		m_pOwner->Init_Frame();
	}

	return 0;
}

void CJohnMeleeAttack::LateUpdate_State(const _float& fTimeDelta)
{
	m_pOwner->CollisionWithTarget();
}

void CJohnMeleeAttack::Render_State()
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

void CJohnMeleeAttack::Exit()
{
	Clear();
}

void CJohnMeleeAttack::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(1))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = 40.f;
	_float fHeight = 59.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	m_bGoComplete = false;
	m_bCompleteFramSizeCalc = false;
}

CJohnMeleeAttack* CJohnMeleeAttack::Create(CJohn* pOwner)
{
	CJohnMeleeAttack* pState = new CJohnMeleeAttack(pOwner);

	return pState;
}

void CJohnMeleeAttack::Free()
{
}

void CJohnMeleeAttack::Reset_Size()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohnMeleeAttack::Set_Size()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}