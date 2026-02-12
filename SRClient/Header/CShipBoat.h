#pragma once
#include "CGameObject.h"
#include "CAABB.h"

class CShipBoat : public CGameObject
{
private:
	explicit CShipBoat();
	explicit CShipBoat(const CShipBoat& rhs);
	virtual ~CShipBoat();

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
	CAABB* m_pAABB;

public:
	static CShipBoat* Create();

private:
	virtual void Free();
};

