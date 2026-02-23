#pragma once
#include "CGameObject.h"
class CShipPhoneIDiverSelectEdge : public CGameObject
{
private:
	explicit CShipPhoneIDiverSelectEdge(float fPosX, float fPosY);
	virtual ~CShipPhoneIDiverSelectEdge();

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
	static CShipPhoneIDiverSelectEdge* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

