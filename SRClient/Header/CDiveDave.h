#pragma once
#include "CFSM.h"
#include "CBaseState.h"
#include "CAABB.h"
#include "CDiveDaveIdle.h"
#include "CDiveDaveOpen.h"
#include "CDiveDavePickUp.h"
#include "CSubject.h"

enum class EQUIPPED
{
	//MELEE, // 근접무기 -> 기본 장착
	HARPOON = 0, // 작살
	GUN,	// 총
	EQUIPPED_END
};

class CDiveDave : public CSubject
{
	friend class CDiveDaveIdle;
	friend class CDiveDaveOpen;
	friend class CDiveDavePickUp;

private:
	explicit			CDiveDave();
	explicit			CDiveDave(const CDiveDave& rhs);
	virtual				~CDiveDave();


public:
	void				Start(); // 유니티 Start함수 처럼 써보기
	HRESULT				Ready_GameObject() override;
	_int				Update_GameObject(const _float& fTimeDelta) override;
	void				LateUpdate_GameObject(const _float& fTimeDelta) override;
	void				Render_GameObject() override;

public:
	DIVEDAVESTATE		Get_State()															{ return m_pFSM->Get_State(); }
	ATTACKSUBSTATE		Get_AttackSubState();
	void				Set_State(DIVEDAVESTATE state)										{ m_pFSM->Set_State(state); }

	EQUIPPED			Get_CurEquipped() const												{ return m_eCurEquipped; }
	void				Set_CurEquipeed(EQUIPPED equip)										{ m_eCurEquipped = equip; }


	CTexture*			Get_TextureCom()													{ return m_pTextureCom; }
	void				Set_TextureCom(wstring_view ComName)								{ m_pTextureCom = CGameObject::GetComponent<CTexture, ID_STATIC>(ComName); }

	void				Multiply_Scale(_vec3* vScale)										{ m_pTransformCom->Multiply_Scale(vScale); }
	void				Move(_vec3* vDir, const _float& fTimeDelta);
	void				Set_RotateDir(_vec3* vDir) 
	{
		m_pTransformCom->m_vAngle.x = vDir->x; 
		m_pTransformCom->m_vAngle.y = vDir->y;
		m_pTransformCom->m_vAngle.z = vDir->z;
	}

	void				Get_Pos(_vec3* vPos)												{ return m_pTransformCom->Get_Info(INFO_POS, vPos); }

	_float				Get_Frame()															{ return m_fFrame; };
	void				Init_Frame()														{ m_fFrame = 0.f; }
	void				AddFrame(const _float& fTimeDelta, const _float& fSpeed,_uint size);

	void				Set_FishCaught(_bool bFishCaught)									{ m_bFishCaught = bFishCaught; }
	_bool				Is_FishCaught()														{ return m_bFishCaught; }

	// 다른 상태 객체에서도 Notify할 수 있게
	void				State_Notify(Event& e)												{ CDiveDave::Notify(e); };
	// 어느쪽 보고있는지
	_bool				Is_Flip()															const { return m_bFlip; }
	void				Set_Flip(_bool isFlip)												{ m_bFlip = isFlip; }

	// With DiveItemBox
	void				Set_IsOnItemBox(_bool isOn)											{ m_bIsOnItemBox = isOn; }
	void				Set_CurOnItemBox(CGameObject* pItemBox)								{ m_pCurOnItemBox = pItemBox; }
	// With DiveItem
	void				Set_IsOnItem(_bool isOn)											{ m_bIsOnItem = isOn; }
	void				Set_CurOnItem(CGameObject* pItem)									{ m_pCurOnItem = pItem; }

	// 무기 등록
	void				Set_WeaponSlot(CGameObject* pWeapon, EQUIPPED equipped)
	{
		if (equipped == EQUIPPED::EQUIPPED_END)
			return;
		
		m_vecWeaponSlot[(_uint)equipped] = pWeapon;
		Event e;
		e.type = EVENTTYPE::GET_WEAPON;
		e.ItemTextureName = pWeapon->Get_TexName();
		e.value = (_uint)equipped + 1;
		CDiveDave::Notify(e);
	}

	void				On_Hit(const _float& fDamage) 
	{ 
		if (m_fIvncTime > 0.f)
			return;

		m_bIsHit = true;
		m_fHp -= fDamage;
		Event e;
		e.type = EVENTTYPE::CHANGE_HP;
		e.value = (_uint)m_fHp;
		e.fValue = m_fHp / m_fMaxHp;
		CDiveDave::Notify(e);

		if (m_fHp <= 0.f)
		{
			m_fHp = 0.f;
			On_Dead();
		}
	}
	// Die 상태로 전이하기위한 함수
	void				On_Dead() 															{ m_bIsDie = true; }

	// Hp회복
	void				Restore_Hp(const _float& restore) 
	{ 
		m_fHp += restore; 
		if (m_fHp >= m_fMaxHp)
			m_fHp = m_fMaxHp;

		Event e;
		e.type = EVENTTYPE::CHANGE_HP;
		e.value = (_uint)m_fHp;
		e.fValue = m_fHp / m_fMaxHp;
		CDiveDave::Notify(e);
	}
	// Hit 상태 탈출시 호출
	void				Hit_Free()															{ m_bIsHit = false; }
	_float				Get_HitTime()														{ return m_fIvncTime; }
	void				Acc_HitTime(const _float& fTimeDelta)								{ m_fIvncTime += fTimeDelta; }
	void				Reset_HitTime()														{ m_fIvncTime = 0.f; }
	// 전역 상태 바로 진입
	_bool				Check_GlobalState();

	_bool				Is_SubMarine() const { return m_bSubMarine; }
	void				Set_SubMarine(_bool isSubMarine) { m_bSubMarine = isSubMarine; }
	CAABB*				Get_AABB()						{ return m_pAABB; }

public:
	void				Set_CanKeyInput(_bool canKey)										{ m_bCanKeyInput = canKey; }
	void				Set_CanMouseInput(_bool canMouse)									{ m_bCanMouseInput = canMouse; }
	_bool				Get_CanKeyInput() const												{ return m_bCanKeyInput; }
	_bool				Get_CanMouseInput() const											{ return m_bCanMouseInput; }

private:
	HRESULT				Ready_Component();
	HRESULT				Add_State();

private:
	void				Key_Input();
	void				Mouse_Input();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB; // 아이템 상자랑 충돌 용
	CAABB* m_pAABBItem; // 아이템이랑 충돌 용

private:
	CFSM<CDiveDave, DIVEDAVESTATE>* m_pFSM = nullptr;
	//CBaseState* m_pState = nullptr;
	//DIVEDAVESTATE m_eCurState;
	//unordered_map<DIVEDAVESTATE, CBaseState*> m_mapState;

private:
	EQUIPPED m_eCurEquipped = EQUIPPED::HARPOON;

private:
	_float m_fSpeed = 10.f;
	_float m_fFrame = 0.f;
	_bool  m_bFishCaught = false;

	_bool  m_bCanKeyInput = true;
	_bool  m_bCanMouseInput = true;

	_bool m_bIsOnItemBox = false;
	_bool m_bIsOnItem = false;

	_float m_fMaxHp = 100.f;
	_float m_fHp = 50.f;
	_float m_fIvncTime = 0.f; // 피격 당한 후 시간
	_bool  m_bIsHit = false;
	_bool  m_bIsDie = false; // CGameObject의 m_bDead와 다른 용도!
	
	_bool	m_bFlip = false; // false면 오른쪽 보는거, true면 왼쪽 보는거

	_bool	m_bInitComplete = false; // 유니티 Start함수 처럼 써보기

	_bool  m_bSubMarine = true;

private:
	CGameObject* m_pCurOnItemBox = nullptr;
	CGameObject* m_pCurOnItem = nullptr;

	unordered_map<std::wstring_view, CGameObject*> m_mapCanUseItemSlot = { {L"ItemSlot1", nullptr}, {L"ItemSlot2", nullptr} };
	CGameObject* m_vecWeaponSlot[(_uint)EQUIPPED::EQUIPPED_END] = {nullptr, nullptr};

public:
	static CDiveDave* Create();

private:
	virtual void Free() override;
};

