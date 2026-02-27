#pragma once
#include "CGameObject.h"
class CDaggerThumbnailUI :
    public CGameObject
{
private:
	explicit CDaggerThumbnailUI();
	explicit CDaggerThumbnailUI(const CDaggerThumbnailUI& rhs);
	virtual ~CDaggerThumbnailUI();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	void		Refresh_Size();

private:
	HRESULT Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CDaggerThumbnailUI* Create();

private:
	virtual void Free() override;
};

