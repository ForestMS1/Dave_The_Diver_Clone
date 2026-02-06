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
	explicit CMiniMapTerrain();
	explicit CMiniMapTerrain(const CGameObject& rhs);
	virtual ~CMiniMapTerrain();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void			Set_TypeNum(_int TypeNum) { m_iTypeNum  = TypeNum;}
	_int			Get_TypeNum() { return m_iTypeNum; }


	void			Set_RoomNum(_vec2 RoomNum) {m_RoomNum = RoomNum;}
	_vec2			Get_RoomNum() { return m_RoomNum; }

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
	_vec2				m_RoomNum{};
public:
	static CMiniMapTerrain* Create();

private:
	virtual void Free();

};

