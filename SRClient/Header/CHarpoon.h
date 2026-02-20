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
	void	Update_Points();

private:
	Engine::CAttackReadyArmTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CLineBuffer* m_pLineBuffer;

private:
	_bool m_bIsFlip = false;
	vector<_vec3> m_vecHarpoonToProjectilePoints;

public:
	static CHarpoon* Create();

private:
	virtual void Free() override;
};

