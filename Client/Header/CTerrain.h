#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CTerrainTex;
	class CTexture;
	class CTransform;
}

class CTerrain : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrain(const CGameObject& rhs);
	virtual ~CTerrain();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Add_Component();

private:
	Engine::CTerrainTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

