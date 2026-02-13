#pragma once
#include "CGameObject.h"
#include "CTargetArrowTex.h"
class CTargetArrow :
    public CGameObject
{
private:
	explicit CTargetArrow();
	explicit CTargetArrow(const CTargetArrow& rhs);
	virtual ~CTargetArrow();


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
	Engine::CTargetArrowTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	_vec3 m_vOffset;

public:
	static CTargetArrow* Create();

private:
	virtual void Free() override;
};

