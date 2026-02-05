#pragma once
#include "CGameObject.h"
namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}

class CPlayerState;

enum class PlayerState 
{
	IDLE = 0,
	ATTACK,
	DIE,
};

class CDSPlayer : public CGameObject
{
private:
	explicit CDSPlayer();
	explicit CDSPlayer(const CGameObject& rhs);
	virtual ~CDSPlayer();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	PlayerState		Get_State() const { return m_eCurState; }

	void			Set_State(PlayerState state);
private:
	HRESULT			Add_Component();
	HRESULT			Add_State();
	void			Key_Input(const _float& fTimeDelta);
	void			Mouse_Move();
	void			Set_Cam();
	void			Set_OnTerrain();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCalculator* m_pCalculatorCom;

private:
	CPlayerState* m_pState = nullptr;
	PlayerState m_eCurState;
	unordered_map<PlayerState, CPlayerState*> m_mapState;

public:
	static CDSPlayer* Create();

private:
	virtual void Free();

};

