#pragma once
#include "CGameObject.h"
#include "CFishState.h"
#include "CFishStateMachine.h"

//namespace Fish
//{
//	class CFishStateMachine;
//}
//yellowbackFusilier: swim die
//BlueTang : swim die
//Clownfish : die swim sprint
//Smallspotted_dart : die swim sprint
//YellowTang : die swim sprint

namespace Fish
{
	class CFishState;
	class CFishSwimState;
	class CFishStopState;
}

class CFishGameObject : public CGameObject
{
public:
	friend class Fish::CFishState;
	friend class Fish::CFishStopState;
	friend class Fish::CFishSwimState;
public:
	explicit CFishGameObject();
	//virtual ~CFishGameObject();

public:
	void Update_ImGui() override;

public:
	std::wstring_view Get_FishName() const { return m_sFishName; }
	Fish::CFishStateMachine& Get_StateMachin() { return m_fsm; }

public:
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	void Render(function<void()> beforeDrawLambda = nullptr);

protected:
	HRESULT			Ready(std::wstring_view svSpineName);
protected:
	Engine::CDynamicBuffer* m_pDynamicBuffer;
	Engine::CTransform* m_pTransformCom;
	Engine::CSpine* m_pSpineCom;

protected:
	std::wstring m_sFishName;
	float m_fSpeed;
	float m_fRotateSpeed;
	float m_fRotateAngleLimit;
	int m_iHP;
	_vec3 m_vChaseTarget;

	float m_fChaseTargetReLocateTimer;

protected:
	bool m_bManual;

protected:
	Fish::FISH_TYPE m_eFishType;
	Fish::FISH_STATE m_eFishState;
	Fish::CFishStateMachine m_fsm;
public:
	void Free() override;
};

