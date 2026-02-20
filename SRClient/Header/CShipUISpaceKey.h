#pragma once
#include "CGameObject.h"
class CShipUISpaceKey : public CGameObject
{
private:
	explicit CShipUISpaceKey(float fPosX, float fPosY);
	virtual ~CShipUISpaceKey();

public:
	Engine::CTransform* Get_Transform() const { return m_pTransformCom; }
	void AddRender(bool bAdd) { m_bAddRender = bAdd; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	bool m_bAddRender;

public:
	static CShipUISpaceKey* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

