#include "CJohn2AttackReady.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn2.h"
#include "CSoundMgr.h"
CJohn2AttackReady::CJohn2AttackReady(CJohn2* pOwner)
	: CBaseState<CJohn2>(pOwner)
{
}

CJohn2AttackReady::~CJohn2AttackReady()
{
}

void CJohn2AttackReady::Enter()
{
	m_pOwner->Init_Frame();

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2GunSkill01_Aim")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_JW2_GunSkill01_Aim", CSoundMgr::BOSS_SFX, 1.f);
}

void CJohn2AttackReady::Input(const _float& fTimeDelta)
{
}

_int CJohn2AttackReady::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 5.f, 8, false);

	m_pOwner->Update_ToTargetDir();

	_vec3 vRotDir;
	if (m_pOwner->Get_ToTargetDir().x > 0.f)
		vRotDir = { 0.f, 0.f, 0.f };
	else
		vRotDir = { 0.f,-180.f, 0.f };
	m_pOwner->Set_RotateDir(&vRotDir);

	m_fReadyTime += fTimeDelta;

	if (m_pOwner->Get_Frame() == 7)
		m_pOwner->Set_State(JOHN2STATE::SHOT);
	return 0;
}

void CJohn2AttackReady::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohn2AttackReady::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"John2GunSkill01_Aim"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohn2AttackReady::Exit()
{
	Clear();
}

void CJohn2AttackReady::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2GunSkill01_Aim")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);


	m_fReadyTime = 0.f;
}

CJohn2AttackReady* CJohn2AttackReady::Create(CJohn2* pOwner)
{
	CJohn2AttackReady* pState = new CJohn2AttackReady(pOwner);

	return pState;
}

void CJohn2AttackReady::Free()
{
}
