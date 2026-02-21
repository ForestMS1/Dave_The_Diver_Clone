#pragma once
#include "CGameObject.h"
class CShipPhoneIDiverUpgrade : public CGameObject
{
private:
	explicit CShipPhoneIDiverUpgrade(float fPosX, float fPosY);
	virtual ~CShipPhoneIDiverUpgrade();

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

	bool m_bSuccessOpen;
public:
	static CShipPhoneIDiverUpgrade* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

