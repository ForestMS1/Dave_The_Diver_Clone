#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CCustomer1 : public CGameObject
{
private:
	explicit CCustomer1();
	explicit CCustomer1(const CGameObject& rhs);
	virtual ~CCustomer1();

public:
	enum State { WALK, MENU, WAIT, EAT, HAPPY, ANGER, STATE_END };
	enum Facing { LEFT, RIGHT, FACE_END };
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
	void						Find_Chair();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pWalkTextureCom;
	Engine::CTexture* m_pMenuTextureCom;
	Engine::CTexture* m_pWaitTextureCom;
	Engine::CTexture* m_pEatTextureCom;
	Engine::CTexture* m_pHappyTextureCom;
	Engine::CTexture* m_pAngerTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CCustomer1* Create();

private:
	State			curState;
	_float			m_fFrame;
	Facing			curDir;
	Facing			prevDir;
	_vec3			targetPos;
	bool			Sitted;
	bool			ChoosingMenu;
	bool			Waiting;
	virtual void Free();

};

