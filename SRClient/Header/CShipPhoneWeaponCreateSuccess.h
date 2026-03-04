#pragma once
#include "CGameObject.h"
class CShipPhoneWeaponCreateSuccess : public CGameObject
{
private:
	explicit CShipPhoneWeaponCreateSuccess();
	virtual ~CShipPhoneWeaponCreateSuccess();

public:
	void Set_AssetName(std::wstring_view sv) { m_sAssetName = sv; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	HRESULT Ready_AfterCreate();


private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	std::wstring m_sAssetName;

public:
	static CShipPhoneWeaponCreateSuccess* Create();

private:
	virtual void Free();
};

