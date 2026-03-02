#pragma once
#include "CGameObject.h"
#include "CAABB.h"
namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CSushiDave : public CGameObject
{
private:
	explicit CSushiDave();
	explicit CSushiDave(const CGameObject& rhs);
	virtual ~CSushiDave();
	void	Update_ImGui() override;


public:
	enum State { IDLE,WALK, RUN,TIRED, SUSHI_IDLE, SUSHI_WALK, SUSHI_RUN, SUSHI_TIRED, STATE_END };
	enum Facing { LEFT, RIGHT, FACE_END };
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pIdleTextureCom;
	Engine::CTexture* m_pWalkTextureCom;
	Engine::CTexture* m_pRunTextureCom;
	Engine::CTexture* m_pTiredTextureCom;
	Engine::CTexture* m_pSushiIdleTextureCom;
	Engine::CTexture* m_pSushiWalkTextureCom;
	Engine::CTexture* m_pSushiRunTextureCom;
	Engine::CTexture* m_pSushiTiredTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	void Key_Input(const _float& fTimeDelta);
	static CSushiDave* Create();

private:
	State			curState;
	_float			m_fFrame;
	bool			holdingSushi;
	Facing			curDir;
	Facing			prevDir;
	CAABB*			m_pAABB;
	wstring			m_sSushiName;
	wstring			m_sTexName;
	float			m_fGauge= -0.49;
	float			tiredTime;
public:
	bool			makingWasabi = false;

};

