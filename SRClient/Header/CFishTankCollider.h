#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CFishTankCollider : public CGameObject
{

private:
	explicit CFishTankCollider(float fPosX, float fPosY);
	virtual ~CFishTankCollider();

public:
	Engine::CTransform* Get_Transform() const { return m_pTransformCom; }
	CAABB* Get_AABB() const { return m_pAABB; }


public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

	float m_fPosX;
	float m_fPosY;


public:
	static CFishTankCollider* Create(float fPosX, float fPosY);

private:
	virtual void Free();

};

//local
//
//dev  -DB
//
//qa  -DB
//
//staging
//			-DB
//product


// versioning
// 0.1
// 0.2 
// 