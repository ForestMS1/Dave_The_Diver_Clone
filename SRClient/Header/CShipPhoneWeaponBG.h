#pragma once
#include "CGameObject.h"
#include "CAABB.h"
#include "CShipPhoneWeaponImg.h"
class CShipPhoneWeaponBG : public CGameObject
{
private:
	explicit CShipPhoneWeaponBG();
	virtual ~CShipPhoneWeaponBG();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();


private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	//_uint m_iSelectIdx;

	CShipPhoneWeaponImg* m_pEdge;

public:
	static CShipPhoneWeaponBG* Create();

	static _uint iCurrentIdx;

private:
	virtual void Free();
};

