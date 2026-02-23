#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CCloseButton : public CGameObject
{
private:
	explicit CCloseButton();
	explicit CCloseButton(const CGameObject& rhs);
	virtual ~CCloseButton();


public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pCloseTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

public:
	static CCloseButton* Create();
	wstring Get_Frame() { return whichFrame; }
	void Set_Frame(wstring purpose) { whichFrame = purpose; }
private:
	virtual void Free();
	wstring whichFrame;
};

