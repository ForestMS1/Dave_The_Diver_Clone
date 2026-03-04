

#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CSmallMenu : public CGameObject
{
private:
	explicit CSmallMenu();
	explicit CSmallMenu(const CGameObject& rhs);
	virtual ~CSmallMenu();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
	void						Set_SushiTex(wstring tex) { fishName = tex; }
	void						Set_Total(wstring total) { totalQuantity = total; }
	void						Set_CurQuantity(wstring cur) { CurQuantity = cur; }
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	_vec3 screen;
	_vec3 screen1;



public:
	static CSmallMenu* Create();
public:

	wstring fishName;
	wstring sushiTex;
	wstring totalQuantity;
	wstring CurQuantity;
private:
	virtual void Free();

};

