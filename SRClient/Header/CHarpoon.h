#pragma once
#include "CGameObject.h"
#include "CAttackReadyArmTex.h"
#include "CLineBuffer.h"
class CHarpoon :
    public CGameObject
{
private:
	explicit CHarpoon();
	explicit CHarpoon(const CHarpoon& rhs);
	virtual ~CHarpoon();


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
	static CHarpoon* Create();

private:
	virtual void Free() override;
};

