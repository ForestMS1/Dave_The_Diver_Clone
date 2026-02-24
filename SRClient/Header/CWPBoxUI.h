#pragma once
#include "CGameObject.h"
class CWPBoxUI :
    public CGameObject
{
private:
	explicit CWPBoxUI(_bool isSub = false);
	explicit CWPBoxUI(const CWPBoxUI& rhs);
	virtual ~CWPBoxUI();


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
	static CWPBoxUI* Create(_bool isSub = false);

private:
	virtual void Free() override;
};

