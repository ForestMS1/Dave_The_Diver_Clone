#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine {
	class CGlbTex;

	
}
class CTerrian : public CGameObject
{
private:
	explicit CTerrian();
	explicit CTerrian(const wstring_view tex);
	explicit CTerrian(const wstring_view tex, const wstring_view Name);
	explicit CTerrian(const CTerrian& rhs);
	virtual ~CTerrian();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void						Set_CollisionName(wstring _CollsionName) { m_CollisionName = _CollsionName; }
	void						Set_Fog();

private:
	HRESULT			Ready_Component();
	HRESULT			Ready_Material();


private:
	Engine::CGlbTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	const wstring_view m_wsName;
	vector<CAABB*> m_pAABB;
	CAABB* m_pfrustomAABB;
private:
	wstring m_CollisionName;
public:
	static CTerrian* Create();
	static CTerrian* Create(const wstring_view tex);
	static CTerrian* Create(const wstring_view tex, const wstring_view Name);

private:
	virtual void Free();
};

