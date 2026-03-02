#pragma once
#include "CGameObject.h"
#include "CAABB.h"

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
	enum State { WALK, MENU, WAIT, EAT, HAPPY, ANGER,LEAVE, STATE_END };
	enum Facing { LEFT, RIGHT, FACE_END };
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
	void						Find_Chair();
	void						Empty_Chair();
	CAABB* Get_AABB()		 { return m_pAABB; }
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
	wstring			sushiHanded;
	bool			gotSushi;
	CGameObject* MenuBubble;
	CGameObject* TeaBubble;
	bool			OrderedTea;
	bool			gotTea;

private:
	State			curState;
	_float			m_fFrame;
	Facing			curDir;
	Facing			prevDir;
	_vec3			targetPos;
	bool			ChairFound;
	bool			Sitted;
	bool			ChoosingMenu;
	bool			Reacting;
	bool			Eating;
	float			deltaTime;
	float			teaTime;
	float			teaWatingTime;
	float			teaChoosingTime;
	float			ExitTime;
	float			ReactionTime;
	float			EatingTime;
	float			EmotionTime;
	float			GoldTime;
	CAABB*			m_pAABB;
	_vec3 screen;
	int random1;
	int random2;
	virtual void Free();
	bool			choosingTea = false;
	bool			waitingMenu = false;;

};

