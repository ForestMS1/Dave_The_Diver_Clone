#pragma once
#include "IObserver.h"
#include "CRcDynamicTex.h"
class CO2StrokeUI :
    public IObserver
{
private:
	explicit CO2StrokeUI();
	explicit CO2StrokeUI(const CO2StrokeUI& rhs);
	virtual ~CO2StrokeUI();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

private:
	HRESULT Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRcDynamicTex* m_pDynamicBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	_float m_fRatio = 1.f;			// 게이지 비율
	_float m_fPlayerHpRatio = 0.f; // 실제 플레이어의 체력 비율 ( curHp / MaxHp )

	_bool  m_IsRestore = false; // false면 깎이는 중, true면 회복 중
public:
	static CO2StrokeUI* Create();

private:
	virtual void Free() override;

	// IObserver을(를) 통해 상속됨
	void OnNotify(const Event& e) override;
};

