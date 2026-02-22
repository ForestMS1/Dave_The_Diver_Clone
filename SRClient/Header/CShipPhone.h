#pragma once
#include "CGameObject.h"
class CShipPhone : public CGameObject
{
private:
	explicit CShipPhone();
	explicit CShipPhone(const CShipPhone& rhs);
	virtual ~CShipPhone();

public:
	bool Get_OpenTween() const { return m_bOpenTween; }
	bool Get_CloseTween() const { return m_bCloseTween; }
	void Set_CloseTween(bool bCloseTween) {
		m_bCloseTween = bCloseTween; OnUnFocus_App();
	}

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	void Focus_App(wstring_view svFocusAppName);
	void UnFocus_App();

private:
	void OnFocus_App();
	void OnFocusing_App();
	void OnUnFocus_App();
	void OnUnFocusing_App();

private:
	HRESULT			Ready_Component();


private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	// PosY, RotX, RotY
	tweeny::tween<float, float, float> m_tweenOpen;
	bool m_bOpenTween;
	tweeny::tween<float> m_tweenClose;
	bool m_bCloseTween;

	// PosX, PosY, RotX, RotY
	tweeny::tween<float, float, float, float> m_tweenFocus;
	bool m_bFocus;
	bool m_bFocusing;

	// PosX, PosY, RotX, RotY
	tweeny::tween<float, float, float, float> m_tweenUnFocus;
	//bool m_bUnFocus;
	bool m_bUnFocusing;

	bool m_bOnFocus;
	bool m_bOnUnFocus;
	std::wstring m_sFocusedAppName;


public:
	static CShipPhone* Create();

private:
	virtual void Free();
};

