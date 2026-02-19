#pragma once
#include "CGameObject.h"
class CShipPhoneIDiverBG : public CGameObject
{
private:
	explicit CShipPhoneIDiverBG();
	virtual ~CShipPhoneIDiverBG();

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

public:
	static CShipPhoneIDiverBG* Create();

private:
	virtual void Free();
};

