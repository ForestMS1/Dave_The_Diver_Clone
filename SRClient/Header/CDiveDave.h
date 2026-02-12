#pragma once
#include "CGameObject.h"
#include "CDiveDaveIdleTex.h"

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

public:
	DiveState		Get_State() const { return m_eCurState; }
	void			Set_State(DiveState state);


	CTexture*	Get_TextureCom() { return m_pTextureCom; }
	void		Set_TextureCom(wstring_view ComName) { m_pTextureCom = CGameObject::GetComponent<CTexture, ID_STATIC>(ComName); }

	void		Multiply_Scale(_vec3* vScale) { m_pTransformCom->Multiply_Scale(vScale); }
	void		Move(_vec3* vDir, const _float& fTimeDelta);
	void		Set_RotateDir(_vec3* vDir) 
	{
		m_pTransformCom->m_vAngle.x = vDir->x; 
		m_pTransformCom->m_vAngle.y = vDir->y;
		m_pTransformCom->m_vAngle.z = vDir->z;
	}

	void		Get_Pos(_vec3* vPos) { return m_pTransformCom->Get_Info(INFO_POS, vPos); }

	_float		Get_Frame() { return m_fFrame; };
	void		Init_Frame() { m_fFrame = 0.f; }
	void		AddFrame(const _float& fTimeDelta, const _float& fSpeed,_uint size);

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

private:
	_float m_fSpeed = 5.f;
	_float m_fFrame = 0.f;

public:
	static CDiveDave* Create();

private:
	virtual void Free() override;
};

