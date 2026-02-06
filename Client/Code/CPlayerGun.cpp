#include "pch.h"
#include "CPlayerGun.h"
#include "CManagement.h"
#include "CDSPlayer.h"
CPlayerGun::CPlayerGun()
	: m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CPlayerGun::CPlayerGun(const CPlayerGun& rhs)
{
}

CPlayerGun::~CPlayerGun()
{
}

HRESULT CPlayerGun::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	Set_Parent(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"Player"));

	return S_OK;
}

_int CPlayerGun::Update_GameObject(const _float& fTimeDelta)
{
	dynamic_cast<CDSPlayer*>(m_pParentGameObject)->Get_State();

	return _int();
}

void CPlayerGun::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CPlayerGun::Render_GameObject()
{
}

HRESULT	CPlayerGun::Add_Component()
{

	return S_OK;
}

void CPlayerGun::Free()
{
	CGameObject::Free();
}
