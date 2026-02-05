#pragma once
#include "CBase.h"
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent* Get_Component(COMPONENTID eID, std::wstring_view svComponentTag);
	_float		Get_ViewZ() { return m_fViewZ; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() = 0;

public:
	void			Compute_ViewZ(const _vec3* pPos);
	void			Set_Dead() { m_bDead = true; }
	bool			Get_Dead() const { return m_bDead; }
	
	

protected:
	map<const std::wstring, CComponent*>	m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9						m_pGraphicDev;

	_float									m_fViewZ;
	bool									m_bDead;

// °èÃþ °ü·Ã
public:
	CGameObject * const Get_Parent() const { return m_pParentGameObject; }
	list<CGameObject*> * const  Get_Children()  { return &m_childGameObjectList; }
	void Set_Parent(CGameObject* const pNewParent);
protected:
	CGameObject* m_pParentGameObject;
	list<CGameObject*> m_childGameObjectList;
private:
	void EraseChildGameObject(CGameObject* pParentGameObject, CGameObject* pDelTarget);




private:
	CComponent* Find_Component(COMPONENTID eID, std::wstring_view svComponentTag);

protected:
	virtual		void		Free();
};

END