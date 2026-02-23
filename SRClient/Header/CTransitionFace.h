#pragma once
#include "CGameObject.h"
class CTransitionFace : public CGameObject
{
private:
	explicit CTransitionFace(float fPosX, float fPosY);
	virtual ~CTransitionFace();

public:
	void Set_Delay(float fDelay) { m_fDelay = fDelay; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void Ready_AfterCreate();

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	_uint m_iIdx;

	tweeny::tween<float> m_tween;

	float m_fDelay;
	float m_fAccDelayTime;

public:
	static CTransitionFace* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

