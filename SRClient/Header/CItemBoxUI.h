#pragma once
#include "CGameObject.h"
class CItemBoxUI :
    public CGameObject
{
private:
	explicit CItemBoxUI(_bool isSub = false);
	explicit CItemBoxUI(const CItemBoxUI& rhs);
	virtual ~CItemBoxUI();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

private:
	HRESULT Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	_bool m_bIsSub = false;

public:
	static CItemBoxUI* Create(_bool isSub = false);

private:
	virtual void Free() override;
};

