#pragma once
#include "CGameObject.h"
class CLogoTitle : public CGameObject
{
private:
	explicit CLogoTitle(float fPosX, float fPosY);
	virtual ~CLogoTitle();

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

public:
	static CLogoTitle* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

