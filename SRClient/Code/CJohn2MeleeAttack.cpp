#include "CJohn2MeleeAttack.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn2.h"
#include "CColliderMgr.h"
CJohn2MeleeAttack::CJohn2MeleeAttack(CJohn2* pOwner)
	: CBaseState<CJohn2>(pOwner)
{
}

CJohn2MeleeAttack::~CJohn2MeleeAttack()
{
}

void CJohn2MeleeAttack::Enter()
{
	m_pOwner->Init_Frame();
	m_wsTexName = L"John2MeleeSkill01_Attack";

	Set_Size();

	m_pOwner->Update_ToTargetDir();
}

void CJohn2MeleeAttack::Input(const _float& fTimeDelta)
{
}

_int CJohn2MeleeAttack::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 10.f, 2, true);
	m_bGoComplete = m_pOwner->Rush_ToTarget(fTimeDelta);

	if(m_bGoComplete)
		m_pOwner->Set_State(JOHN2STATE::MELEEATTACK_RETURN);
	return 0;
}

void CJohn2MeleeAttack::LateUpdate_State(const _float& fTimeDelta)
{
	m_pOwner->CollisionWithTarget();
}

void CJohn2MeleeAttack::Render_State()
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

void CJohn2MeleeAttack::Exit()
{
	Clear();
}

void CJohn2MeleeAttack::Clear()
{
	Reset_Size();

	m_bGoComplete = false;
}

CJohn2MeleeAttack* CJohn2MeleeAttack::Create(CJohn2* pOwner)
{
	CJohn2MeleeAttack* pState = new CJohn2MeleeAttack(pOwner);

	return pState;
}

void CJohn2MeleeAttack::Free()
{
}

void CJohn2MeleeAttack::Reset_Size()
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

void CJohn2MeleeAttack::Set_Size()
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