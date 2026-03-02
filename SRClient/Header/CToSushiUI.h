#pragma once
#include "CGameObject.h"
class CToSushiUI : public CGameObject
{
private:
	explicit CToSushiUI(float fPosX, float fPosY);
	virtual ~CToSushiUI();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	HRESULT Ready_AfterCreate();

public:
	void Update_ImGui() override;

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	float m_fDbgX;
	float m_fDbgY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	bool m_bOpen;
	tweeny::tween<float> m_tweenOpen;
	bool m_bOpenTween;


	// Money
// LV
	// CNT
	// Title
	// RANK



public:
	static CToSushiUI* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

