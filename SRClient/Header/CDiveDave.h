#pragma once
#include "CGameObject.h"
#include "CPlayerState.h"

enum class DiveState
{
	IDLE = 0,
	MOVE,
	ATTACK,			// 작살, 총 공격
	MELEEATTACK,	// 근접 공격
	TANNING,
	DIE,
	DAVE_STATE_END
};

enum class EQUIPPED
{
	MELEE, // 근접무기
	HARPOON, // 작살
	GUN,	// 총
	EQUIPPED_END
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
	ATTACKSUBSTATE	Get_AttackSubState();
	void			Set_State(DiveState state);

	EQUIPPED		Get_CurEquipped() const { return m_eCurEquipped; }
	void			Set_CurEquipeed(EQUIPPED equip) { m_eCurEquipped = equip; }


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

	void		Set_FishCaught(_bool bFishCaught) { m_bFishCaught = bFishCaught; }
	_bool		Is_FishCaught()					  { return m_bFishCaught; }

public:
	void Set_CanKeyInput(_bool canKey) { m_bCanKeyInput = canKey; }
	void Set_CanMouseInput(_bool canMouse) { m_bCanMouseInput = canMouse; }
	_bool Get_CanKeyInput() const { return m_bCanKeyInput; }
	_bool Get_CanMouseInput() const { return m_bCanMouseInput; }

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
	EQUIPPED m_eCurEquipped = EQUIPPED::MELEE;

private:
	_float m_fSpeed = 5.f;
	_float m_fFrame = 0.f;
	_bool  m_bFishCaught = false;

	_bool  m_bCanKeyInput = true;
	_bool  m_bCanMouseInput = true;

public:
	static CDiveDave* Create();

private:
	virtual void Free() override;
};

