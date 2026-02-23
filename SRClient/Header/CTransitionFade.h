#pragma once
#include "CGameObject.h"
class CTransitionFade : public CGameObject
{
public:
	enum FADE_ID
	{
		// Black To Screen
		FADE_IN,
		// Screen To Black
		FADE_OUT,
		FADE_ID_END
	};
private:
	explicit CTransitionFade(float fPosX, float fPosY, FADE_ID eFade);
	virtual ~CTransitionFade();

public:
	void Set_OnEnd(function<void()> func) { m_funcOnEnd = func; }

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
	const FADE_ID m_eFade;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	tweeny::tween<float> m_tween;
	float m_fAlpha;
	function<void()> m_funcOnEnd;

public:
	static CTransitionFade* Create(float fPosX, float fPosY, FADE_ID eFade);

private:
	virtual void Free();
};

