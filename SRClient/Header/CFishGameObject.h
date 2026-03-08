#pragma once
#include "CGameObject.h"
#include "CFishState.h"
#include "CFishStateMachine.h"
#include "CAABB.h"
#include "CHelper.h"
//namespace Fish
//{
//	class CFishStateMachine;
//}
//yellowbackFusilier: swim die
//BlueTang : swim die
//Clownfish : die swim sprint
//Smallspotted_dart : die swim sprint
//YellowTang : die swim sprint

//namespace Fish
//{
//	class CFishState;
//	class CFishSwimState;
//	class CFishStopState;
//}

class CFishGameObject : public CGameObject
{
//public:
//	friend class Fish::CFishState;
//	friend class Fish::CFishStopState;
//	friend class Fish::CFishSwimState;
public:
	explicit CFishGameObject(float fPosX, float fPosY, float fScale);
	//virtual ~CFishGameObject();

public:
	void Update_ImGui() override;

public:
	void Damaged(int iDamage);
	void Die();
	void RunFrom(_vec3 const* pPos);
	void QTE(_vec3 const* pJaksalPos, _vec3 const* pDavePos);
	void QTERelease();
	void AttackTo(_vec3 const* pPos);
	void Stop();
	void Swim();
	void AcquireTo(_vec3 const* pPos);
	void JacksalAcquire();

	void Pull_Fish(_vec3* vDir, _float fSpeed, _float fTimeDelta) { m_pTransformCom->Move_Pos(vDir, fSpeed, fTimeDelta); }

	bool TryAttackTimer(float fTimeDelta);

	void SliceComplete();
public:
	//Fish::CFishStateMachine& Get_StateMachin() { return m_fsm; }
	int Get_HP() const { return m_iHP; }
	void Set_HP(int iHP) { m_iHP = iHP; }

	Fish::FISH_TYPE Get_FishType() const { return m_eFishType; }
	Fish::FISH_STATE Get_FishState() const { return m_eFishState; }

	void Set_IntersectHitboxDave(bool b) { m_bIntersectHitboxDave = b; }
	void Set_IntersectDetectboxDave(bool b) { m_bIntersectDetetboxDave = b; }

	float Get_DieTimer() const { return m_fDieTimer; }

	bool Get_NeedSlice() const { return m_bNeedSlice; }

	int Get_AttackPower() const { return m_iAttackPower; }

public:
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	void Render(function<void()> beforeDrawLambda = nullptr);

protected:
	HRESULT			Ready(std::wstring_view svSpineName);
	void MoveTo(_vec3* vToPos, const float& fTimeDelta);

protected:


protected:
	Engine::CDynamicBuffer* m_pDynamicBuffer;
	Engine::CTransform* m_pTransformCom;
	Engine::CSpine* m_pSpineCom;
	Engine::CRcTex* m_pBufferCom;


public:
	std::wstring_view Get_FishName() const { return m_sFishName; }
	std::wstring_view Get_FishThumbNailAssetName() const { return m_sThumbNailAssetName ; }
	std::wstring_view Get_SushiThumbNailAssetName() const { return m_sSushiThumbNailAssetName; }
	_uint Get_Rank() const { return m_iRank; }
	_uint Get_Star() const { return m_iStar; }
	float Get_Weight() const { return m_fWeight; }
	_uint Get_Price() const { return m_iPrice; }
	_uint Get_SushiMoney() const { return m_iSushiMoney; }
	_uint Get_SushiLv() const { return m_iSushiLv; }

protected:
	const float m_fPosX;
	const float m_fPosY;
	const float m_fScale;

protected:
	std::wstring m_sFishName;
	std::wstring m_sThumbNailAssetName;
	std::wstring m_sSushiThumbNailAssetName;
	_uint m_iRank;
	_uint m_iStar;
	float m_fWeight;
	_uint m_iPrice;
	float m_fLength;
	_uint m_iMeatCnt;
	_uint m_iSushiMoney;
	_uint m_iSushiLv;

protected:
	std::wstring m_sSwimSpineAniName;

	float m_fCurrSpeed;
	float m_fSpeed;
	float m_fSprintSpeed;
	float m_fCurrRotateSpeed;
	float m_fRotateSpeed;
	float m_fSprintRotateSpeed;
	float m_fRotateAngleLimit;
	int m_iHP;
	_vec3 m_vMoveTarget;

	float m_fMoveTargetReLocateTimer;
	float m_fMoveTargetReLocateTimerRef;

	bool m_bDamaged;
	float m_fDamagedTimer;
	float m_fInvincibleTimer;

	float m_fDieTimer;

	float m_fRunFromTimer;
	std::wstring m_sRunFromSpineAniName;

	float m_fAttackToTimer;
	float m_fAttackIntervalTimer;
	std::wstring m_sAttackSpineAniName;
	int m_iAttackPower = 0;

	bool m_bIntersectHitboxDave;
	bool m_bIntersectDetetboxDave;

	bool m_bDieAndAcquire;


	bool m_bMoveToRotateEnable;

	bool m_bNeedSlice;

	float m_fForceMoveTimer = 0.f;

public:
	void Set_Frustum(_bool _Frustrum) { m_bFrustum = _Frustrum; }
	_bool Get_Frustrum() { return m_bFrustum; }
	void Frustrum();
private:
	bool m_bFrustum{ false };


public:
	void Set_HitboxAABB(CAABB* pAABB) { m_pAABBHitBox = pAABB; }
protected:
	CAABB* m_pAABBHitBox = nullptr;
	
protected:
	bool m_bManual;

protected:
	Fish::FISH_TYPE m_eFishType;
	Fish::FISH_STATE m_eFishState;
	//Fish::CFishStateMachine m_fsm;
public:
	void Free() override;
};

