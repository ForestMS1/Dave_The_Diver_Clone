#pragma once
#include "CGameObject.h"
class CDaveGoldBall :
    public CGameObject
{
private:
	explicit CDaveGoldBall();
	explicit CDaveGoldBall(const CDaveGoldBall& rhs);
	virtual ~CDaveGoldBall();


public:
	void		Init();
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

private:
	HRESULT Ready_Component();
	void	Set_ParentTransform();
	void	Rotate_ToMouse();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;


private:
	_bool m_bIsFlip = false;
	_bool m_bInitComplete = false;
public:
	static CDaveGoldBall* Create();

private:
	virtual void Free() override;
};

