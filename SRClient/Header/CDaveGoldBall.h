#pragma once
#include "CGameObject.h" 
#include "CGoldBallBuffer.h"
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
	Engine::CGoldBallBuffer* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;


private:
	_bool m_bIsFlip = false;
	_bool m_bInitComplete = false;

	_vec3 m_vOffDir;
public:
	static CDaveGoldBall* Create();

private:
	virtual void Free() override;
};

