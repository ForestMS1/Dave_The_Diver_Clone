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
	void			Set_TypeNum(_int TypeNum) { m_iTypeNum = TypeNum; }
	_int			Get_TypeNum() { return m_iTypeNum; }


	void			Set_RoomNum(_vec2 RoomNum) { m_RoomNum = RoomNum; }
	_vec2			Get_RoomNum() { return m_RoomNum; }

	void			Set_bRender(_bool bRender) { m_bRender = bRender; }
	_bool			Get_bRender() { return m_bRender; }
private:
	_int				m_iTypeNum;
	_vec2				m_RoomNum{};

private:
	_bool				m_bRender{false};

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

