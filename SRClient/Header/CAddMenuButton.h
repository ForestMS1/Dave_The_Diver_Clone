#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CAddMenuButton : public CGameObject
{
private:
	explicit CAddMenuButton();
	explicit CAddMenuButton(const CGameObject& rhs);
	virtual ~CAddMenuButton();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
	void			Set_Selected(bool flag) { m_bSelected = flag; }
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pAddTextureCom;
	Engine::CTexture* m_pSelectTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CAddMenuButton* Create();
	bool render;

private:
	virtual void Free();
	bool m_bSelected;

};

