#pragma once
#include "CGameObject.h"

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

public:
	enum State { IDLE,WALK, RUN,TIRED, SUSHI_IDLE, SUSHI_WALK, SUSHI_RUN, SUSHI_TIRED, STATE_END };
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pIdleTextureCom;
	Engine::CTexture* m_pCookTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CSushiDave* Create();

private:
	State			curState;
	_float			m_fFrame;
	virtual void Free();

};

