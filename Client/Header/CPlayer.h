#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}

class CPlayer : public CGameObject
{
private:
	explicit CPlayer();
	explicit CPlayer(const CGameObject& rhs);
	virtual ~CPlayer();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();
	void			Key_Input(const _float& fTimeDelta);
	void			Mouse_Move();
	void			Set_Cam();
	void			Set_OnTerrain();

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CTransform*		m_pTransformCom;
	Engine::CCalculator*	m_pCalculatorCom;

public:
	static CPlayer* Create();

private:
	virtual void Free();

};

