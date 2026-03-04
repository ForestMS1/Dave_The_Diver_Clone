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
	DIVEDAVESTATE		Get_PrevState()														{ return m_pFSM->Get_PrevState(); }

	EQUIPPED			Get_CurEquipped() const												{ return m_eCurEquipped; }
	void				Set_CurEquipeed(EQUIPPED equip)										{ m_eCurEquipped = equip; }


	CTexture*			Get_TextureCom()													{ return m_pTextureCom; }
	void				Set_TextureCom(wstring_view ComName)								{ m_pTextureCom = CGameObject::GetComponent<CTexture, ID_STATIC>(ComName); }

	void				Multiply_Scale(_vec3* vScale)										{ m_pTransformCom->Multiply_Scale(vScale); }
	void				Move(_vec3* vDir, const _float& fTimeDelta, const _float& fSpeed);
	void				Set_RotateDir(_vec3* vDir) 
	{
		m_pTransformCom->m_vAngle.x = vDir->x; 
		m_pTransformCom->m_vAngle.y = vDir->y;
		m_pTransformCom->m_vAngle.z = vDir->z;
	}
	_vec3				Get_LastMoveDir() const												{ return m_vLastMoveDir; }
	_float				Get_LastMoveSpeed() const											{ return m_fLastMoveSpeed; }

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

	CGameObject*		Get_WeponSlot(EQUIPPED equipped)									{ return m_vecWeaponSlot[(_uint)equipped]; }
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

	_bool				Is_SubMarine() const												{ return m_bSubMarine; }
	void				Set_SubMarine(_bool isSubMarine)									{ m_bSubMarine = isSubMarine; }
	CAABB*				Get_AABB()															{ return m_pAABB; }


	// GameMemMgr에서 사용할 Setter
	void				Set_MaxHp(_float fMaxHp)											{ m_fMaxHp = fMaxHp; }
	void				Set_MaxStorageWeight(_float fMaxStorageWeight)						{ m_fMaxStorageWeight = fMaxStorageWeight; }
	void				Set_MaxDepth(_float fMaxDepth)										{ m_fMaxDepth = fMaxDepth; }

	// [LSY] DiveInfo 기록용 현재 뎁스 게터 생성
	_float				Get_BestDepth() const												{ return m_fBestDepth; }

	void				Change_Depth(_float fChangeDepth)											
	{ 
		m_fCurDepth += fChangeDepth;

		// [LSY] 최고 잠수 기록용 분기
		if (m_fCurDepth > m_fBestDepth)
		{
			m_fBestDepth = m_fCurDepth;
		}

		Event e;
		e.type = EVENTTYPE::CHANGE_DEPTH;
		e.fValue = m_fCurDepth;
		Notify(e);
	}

	void				Change_Weight(_float fChangeWeight)
	{
		m_fCurStorageWeight += fChangeWeight;

		Event e;
		e.type = EVENTTYPE::CHANGE_WEIGTH;
		e.fValue = m_fCurStorageWeight;
		e.fValue2 = m_fMaxStorageWeight;
		Notify(e);
	}

	_float				Get_Speed()const													{ return m_fSpeed; }
	void				Set_Speed(_float fSpeed)											{ m_fSpeed = fSpeed; }

	_bool				Is_Overloaded() const												{ return m_bOverloaded; }


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
	void				DoT(const _float fTimeDelta);//Damage Over Time

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

	//체력(O2)
	_float m_fMaxHp = 100.f;
	_float m_fHp = m_fMaxHp;

	//무게
	_float m_fMaxStorageWeight = 20.f; // 최대 수용 가능한 무게
	_float m_fCurStorageWeight = 0.f; // 현재 들고있는 무게

	//수심(Depth)
	_float m_fMaxDepth = 100.f; // 최대 잠수 가능한 깊이
	_float m_fCurDepth = 0.f; // 현재 잠수하고 있는 깊이
	_float m_fBestDepth = 0.f; // [LSY] 이번 다이브에서 최대로 잠수한 깊이

	_float m_fDoTTime = 0.f;


	_float m_fIvncTime = 0.f; // 피격 당한 후 시간
	_bool  m_bIsHit = false;
	_bool  m_bIsDie = false; // CGameObject의 m_bDead와 다른 용도!
	
	_bool	m_bFlip = false; // false면 오른쪽 보는거, true면 왼쪽 보는거

	_bool	m_bInitComplete = false; // 유니티 Start함수 처럼 써보기

	_bool  m_bSubMarine = false;

	_bool  m_bOverloaded = false;

	_vec3 m_vLastMoveDir; // 마지막으로 이동중이었던 방향
	_float m_fLastMoveSpeed; // 마지막으로 이동중이었던 스피드값

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

