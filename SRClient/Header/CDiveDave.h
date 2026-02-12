#pragma once
#include "CGameObject.h"

class CPlayerState;

enum class DiveState
{
	IDLE = 0,
	MOVE,
	ATTACK,
	DIE,
	DAVE_STATE_END
};


class CDiveDave : public CGameObject
{
private:
	explicit CDiveDave();
	explicit CDiveDave(const CDiveDave& rhs);
	virtual ~CDiveDave();


public:
	HRESULT		Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;
	void Render_GameObject() override;

	DiveState		Get_State() const { return m_eCurState; }
	void			Set_State(DiveState state);

private:
	HRESULT Ready_Component();
	HRESULT	Add_State();

private:
	void	Key_Input();
	void	Mouse_Input();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	CPlayerState* m_pState = nullptr;
	DiveState m_eCurState;
	unordered_map<DiveState, CPlayerState*> m_mapState;

public:
	static CDiveDave* Create();

private:
	virtual void Free() override;
};

