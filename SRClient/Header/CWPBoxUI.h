#pragma once
#include "IObserver.h"
class CWPBoxUI :
    public IObserver
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
	void		Move_Slot(const _float& fTimeDelta);

private:
	HRESULT Ready_Component();
	void	Draw_Item(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	_bool m_bIsGunSlot = false; // false면 HarpoonSlot true면 GunSlot
	_bool m_bIsChanging = false;
	_bool m_bIsFront;
	std::wstring m_wsTargetItemTexName;

public:
	static CWPBoxUI* Create(_bool isSub = false);

private:
	virtual void Free() override;

	// IObserver을(를) 통해 상속됨
	void OnNotify(const Event& e) override;
};

