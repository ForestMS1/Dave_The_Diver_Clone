#pragma once
#include "CGameObject.h"
#include "CBaseState.h"
#include "CAABB.h"
#include "CFSM.h"

#include "CParticleMgr.h"

class CJohn : public CGameObject
{
private:
	explicit			CJohn(_float x, _float y, _float z);
	explicit			CJohn(const CJohn& rhs);
	virtual				~CJohn();

public:
	void				Start(); // 유니티 Start함수 처럼 써보기
	HRESULT				Ready_GameObject() override;
	_int				Update_GameObject(const _float& fTimeDelta) override;
	void				LateUpdate_GameObject(const _float& fTimeDelta) override;
	void				Render_GameObject() override;

public:
	JOHNSTATE			Get_State()									{ return m_pFSM->Get_State(); }
	void				Set_State(JOHNSTATE state)					{ m_pFSM->Set_State(state); }

	void				Multiply_Scale(_vec3* vScale)				{ m_pTransformCom->Multiply_Scale(vScale); }
	void				Move(_vec3* vDir, const _float& fTimeDelta);
	void				Set_RotateDir(_vec3* vDir)
	{
		m_pTransformCom->m_vAngle.x = vDir->x;
		m_pTransformCom->m_vAngle.y = vDir->y;
		m_pTransformCom->m_vAngle.z = vDir->z;
	}

	void				Get_Pos(_vec3* vPos)						{ return m_pTransformCom->Get_Info(INFO_POS, vPos); }
	void				Get_TargetPos(_vec3* vTargetPos)			{ if (m_pTargetTransform == nullptr) return; m_pTargetTransform->Get_Info(INFO_POS, vTargetPos); }

	_float				Get_Frame()									{ return m_fFrame; };
	void				Init_Frame()								{ m_fFrame = 0.f; }
	void				AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop = true);

	// 어느쪽 보고있는지
	_bool				Is_Flip()															const	{ return m_bFlip; }
	void				Set_Flip(_bool isFlip)														{ m_bFlip = isFlip; }

	// 전역 상태 바로 진입
	_bool				Check_GlobalState();
	void				On_Hit(const _float& fDamage)
	{
		if (m_fIvncTime > 0.f)
			return;

		_vec3 Pos{};
		m_pTransformCom->Get_Info(INFO_POS, &Pos);
		CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_BLOOD, Pos, 2);
		// 데미지가 일정 수준 이상일떄만 Hit 상태로 넘어간다
		//if(fDamage > 30.f)
			m_bIsHit = true;

		m_fHp -= fDamage;
		if (m_fHp <= 0.f)
		{
			m_fHp = 0.f;
			On_Dead();
		}
	}
	void				On_Dead() { m_bIsDie = true; }

	// Hit 상태 탈출시 호출
	void				Hit_Free() { m_bIsHit = false; }
	_float				Get_HitTime() { return m_fIvncTime; }
	void				Acc_HitTime(const _float& fTimeDelta) { m_fIvncTime += fTimeDelta; }
	void				Reset_HitTime() { m_fIvncTime = 0.f; }

	// 타겟 추적
	_bool				Check_TargetInRange(_float fRange = 10.f);
	void				Update_ToTargetDir();
	_vec3				Get_ToTargetDir() const { return m_vDirToTarget; }
	void				Shot_Bullet();
	_bool				Rush_ToTarget(const _float& fTimeDelta);
	void				Splash_Mine();

	_float				Get_Hp() const { return m_fHp; }
	_float				Get_Speed() const { return m_fSpeed; }

	_float				Get_BreakTime() const { return m_fBreakTime; }
	void				Add_BreakTime(const _float& fTimeDelta) { m_fBreakTime += fTimeDelta; }
	void				Reset_BreakTime() { m_fBreakTime = 0.f; }

	//플레이어랑 처음 마주쳐서 인트로
	void				EncounterTarget();

	CTransform* GetTransformCom() { return m_pTransformCom; };

	void				CollisionWithTarget();


private:
	HRESULT				Ready_Component();
	HRESULT				Add_State();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB; // 자기가 쏜 유도탄이랑 충돌 용
	_vec3 m_vCreatePos; //보스 생성 위치

private:
	_float m_fMaxHp = 100.f;
	_float m_fHp = 100.f;
	_float m_fIvncTime = 0.f;
	_float m_bIsHit = false;
	_bool m_bIsDie = false;

	_bool m_bRushStart = false;

	_float	m_fFrame = 0.f;
	_bool	m_bFlip = false;
	_bool   m_bStartCombat = false; // 플레이어가 처음 보스 마주치면 보스전 시작
	_bool	m_bInitComplete = false; // 유니티 Start함수 처럼 써보기
	_vec3   m_vDirToTarget;

	_float	m_fAccRushDist = 0.f;
	_float m_fSpeed = 10.f;

	_float m_fBreakTime = 0.f;

private:
	CFSM<CJohn, JOHNSTATE>* m_pFSM = nullptr;
	CTransform* m_pTargetTransform = nullptr;
	int test = 0;
public:
	static CJohn* Create(_float x = 0, _float y = 0, _float z = 0);

private:
	virtual void Free() override;
};

