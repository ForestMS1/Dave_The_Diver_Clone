#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CShipPhoneIcon : public CGameObject
{
private:
	explicit CShipPhoneIcon();
	explicit CShipPhoneIcon(const CShipPhoneIcon& rhs);
	virtual ~CShipPhoneIcon();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

public:
	HRESULT Open_Phone();
	HRESULT Close_Phone();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

	bool m_bPhoneOpen;
public:
	static CShipPhoneIcon* Create();

private:
	virtual void Free();
};

