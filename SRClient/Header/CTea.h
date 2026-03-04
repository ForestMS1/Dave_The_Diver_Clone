#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CTea : public CGameObject
{
private:
	explicit CTea();
	explicit CTea(const CGameObject& rhs);
	virtual ~CTea();
	void	Update_ImGui() override;


public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pWasabiTextureCom;
	Engine::CTransform* m_pTransformCom;
public:
	static CTea* Create();

private:
	_float			m_fFrame;
	virtual void Free();
	void	Key_Input();
	void	Reset_Value();

private:
	bool	frameMove;
	bool	wasabiCreated;
	float	m_fScale;
	float   resetTime = 0.f;
	float   soundPlayedTime = 0.f;
	bool	finished = false;
	bool	soundPlayed = false;
public:
	float   gauge = -4.43f;
	float   potGaugeX = 0.81f;
	float   potGaugeY = 6.33f;
	float   angle = 0.f;
	float   potAngle = 0.f;
	CGameObject* customer;
	
	
};

