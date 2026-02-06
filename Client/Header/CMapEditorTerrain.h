#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CMapTerrainTex;
	class CTexture;
	class CTransform;
}

class CMapEditorTerrain : public CGameObject
{
private:
	explicit CMapEditorTerrain();
	explicit CMapEditorTerrain(const CGameObject& rhs);
	virtual ~CMapEditorTerrain();

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

	
public:
	static CMapEditorTerrain* Create();

private:
	virtual void Free();

};

