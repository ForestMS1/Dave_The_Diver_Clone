#pragma once
#include "CGameObject.h"
#include "CFSM.h"
#include "CBaseState.h"
#include "CAABB.h"
#include "CDiveDaveIdle.h"
#include "CDiveDaveOpen.h"
#include "CDiveDavePickUp.h"

enum class EQUIPPED
{
	//MELEE, // 근접무기 -> 기본 장착
	HARPOON = 0, // 작살
	GUN,	// 총
	EQUIPPED_END
};

class CDiveDave : public CGameObject
{
	friend class CDiveDaveIdle;
	friend class CDiveDaveOpen;
	friend class CDiveDavePickUp;

private:
	explicit			CDiveDave();
	explicit			CDiveDave(const CDiveDave& rhs);
	virtual				~CDiveDave();


public:
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


	// With DiveItemBox
	void				Set_IsOnItemBox(_bool isOn)											{ m_bIsOnItemBox = isOn; }
	void				Set_CurOnItemBox(CGameObject* pItemBox)								{ m_pCurOnItemBox = pItemBox; }
	// With DiveItem
	void				Set_IsOnItem(_bool isOn)											{ m_bIsOnItem = isOn; }
	void				Set_CurOnItem(CGameObject* pItem)									{ m_pCurOnItem = pItem; }

	void				On_Hit(const _float& fDamage) 
	{ 
		if (m_fIvncTime > 0.f)
			return;

		m_bIsHit = true;
		m_fHp -= fDamage;
		if (m_fHp <= 0.f)
		{
			m_fHp = 0.f;
			On_Dead();
		}
	}
	// Die 상태로 전이하기위한 함수
	void				On_Dead() 															{ m_bIsDie = true; }
	// Hit 상태 탈출시 호출
	void				Hit_Free()															{ m_bIsHit = false; }
	_float				Get_HitTime()														{ return m_fIvncTime; }
	void				Acc_HitTime(const _float& fTimeDelta)								{ m_fIvncTime += fTimeDelta; }
	void				Reset_HitTime()														{ m_fIvncTime = 0.f; }
	// 전역 상태 바로 진입
	_bool				Check_GlobalState();

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

	_float m_fHp = 100.f;
	_float m_fIvncTime = 0.f; // 피격 당한 후 시간
	_bool  m_bIsHit = false;
	_bool  m_bIsDie = false; // CGameObject의 m_bDead와 다른 용도!

private:
	CGameObject* m_pCurOnItemBox = nullptr;
	CGameObject* m_pCurOnItem = nullptr;

public:
	static CDiveDave* Create();

private:
	virtual void Free() override;
};

