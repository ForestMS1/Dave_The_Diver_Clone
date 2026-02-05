#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCubeTex;
	class CTexture;
	class CTransform;
}

class CCube : public CGameObject
{
private:
	explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCube(const CGameObject& rhs);
	virtual ~CCube();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	CTransform* Get_Transform()
	{
		return m_pTransformCom;
	}

private:
	HRESULT			Add_Component();
private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

