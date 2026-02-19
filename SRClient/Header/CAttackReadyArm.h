#pragma once
#include "CGameObject.h"
#include "CAttackReadyArmTex.h"
class CAttackReadyArm : public CGameObject
{
private:
	explicit CAttackReadyArm();
	explicit CAttackReadyArm(const CAttackReadyArm& rhs);
	virtual ~CAttackReadyArm();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

private:
	HRESULT Ready_Component();
	void	Set_ParentTransform();
	void	Rotate_ToMouse();

private:
	Engine::CAttackReadyArmTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	_bool m_bIsFlip = false;

public:
	static CAttackReadyArm* Create();

private:
	virtual void Free() override;
};

