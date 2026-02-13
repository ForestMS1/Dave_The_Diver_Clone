#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CShipDiverBox : public CGameObject
{
private:
	explicit CShipDiverBox();
	explicit CShipDiverBox(const CShipDiverBox& rhs);
	virtual ~CShipDiverBox();

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
	static CShipDiverBox* Create();

private:
	virtual void Free();
};

