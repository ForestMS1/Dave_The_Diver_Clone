#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CMapTerrainTex;
	class CTexture;
	class CTransform;
}

class CMiniMapTerrain : public CGameObject
{
private:
	explicit CMiniMapTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMiniMapTerrain(const CGameObject& rhs);
	virtual ~CMiniMapTerrain();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:


private:
	HRESULT			Add_Component();
	HRESULT			Ready_Material();

private:
	Engine::CMapTerrainTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	_int				m_iTypeNum;
public:
	static CMiniMapTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

