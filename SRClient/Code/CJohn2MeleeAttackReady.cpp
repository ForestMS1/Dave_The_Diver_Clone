#include "CJohn2MeleeAttackReady.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn2.h"
CJohn2MeleeAttackReady::CJohn2MeleeAttackReady(CJohn2* pOwner)
	: CBaseState<CJohn2>(pOwner)
{
}

CJohn2MeleeAttackReady::~CJohn2MeleeAttackReady()
{
}

void CJohn2MeleeAttackReady::Enter()
{
	m_pOwner->Init_Frame();

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2MeleeSkill01_Ready")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohn2MeleeAttackReady::Input(const _float& fTimeDelta)
{
}

_int CJohn2MeleeAttackReady::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 10.f, 2);
	m_pOwner->Update_ToTargetDir();

	m_fReadyTime += fTimeDelta;

	if (m_fReadyTime > 2.f)
	{
		//m_pOwner->Shot_Bullet();
		m_pOwner->Set_State(JOHN2STATE::MELEEATTACK);
	}

	return 0;
}

void CJohn2MeleeAttackReady::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohn2MeleeAttackReady::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"John2MeleeSkill01_Ready"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohn2MeleeAttackReady::Exit()
{
	Clear();
}

void CJohn2MeleeAttackReady::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2MeleeSkill01_Ready")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);


	m_fReadyTime = 0.f;
}

CJohn2MeleeAttackReady* CJohn2MeleeAttackReady::Create(CJohn2* pOwner)
{
	CJohn2MeleeAttackReady* pState = new CJohn2MeleeAttackReady(pOwner);

	return pState;
}

void CJohn2MeleeAttackReady::Free()
{
}
