#pragma once
#include "CGameObject.h"
class CShipUIDiveBtn : public CGameObject
{
private:
	explicit CShipUIDiveBtn(float fPosX, float fPosY);
	virtual ~CShipUIDiveBtn();

public:
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

	int m_iFrame;
	float m_fAccFrameDelta;

public:
	static CShipUIDiveBtn* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

