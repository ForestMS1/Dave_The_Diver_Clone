#pragma once
#include "IObserver.h"
class CWPAmmoCntText :
	public IObserver
{
private:
	explicit CWPAmmoCntText(float fPosX, float fPosY);
	virtual ~CWPAmmoCntText();

public:
	void Set_ViewZ(float fViewZ) { m_fViewZ = fViewZ; }
	void Set_Txt(std::wstring svTxt) {
		m_sTxt = svTxt;
	}
	void Set_Opt(DWORD dwOpt) { m_dwOpt = dwOpt; }
	void Set_Color(D3DXCOLOR col) { m_color = col; }
	void Set_Font(std::wstring_view svFont) { m_sFont = svFont; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

	std::wstring m_sTxt;

private:
	Engine::CTransform* m_pTransformCom;
	DWORD m_dwOpt;
	D3DXCOLOR m_color;
	std::wstring m_sFont;
	_vec3 m_vPos;

	_uint m_iAmmoCnt = 0;

public:
	static CWPAmmoCntText* Create(float fPosX, float fPosY);

private:
	virtual void Free();

	// IObserver을(를) 통해 상속됨
	void OnNotify(const Event& e) override;
};

