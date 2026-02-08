#pragma once

#include "CBase.h"
#include "Engine_Define.h"
#include "CCollider.h"

BEGIN(Engine)

class ENGINE_DLL CColliderMgr : public CBase
{
	DECLARE_SINGLETON(CColliderMgr)

private:
	explicit CColliderMgr();
	virtual ~CColliderMgr();

public:
	bool Get_Render() const { return m_bRender; };
	void Set_Render(bool bRender) { m_bRender = bRender; }

public:
	map<const std::wstring, list<CCollider*>>* Get_Colliders() { return &m_ColliderGroup; }
	list<CCollider*>* Get_Colliders(std::wstring_view svObjTag);

public:
	void AddColliderGroup(std::wstring_view svLayerTag, CCollider* const pCollider);
	void Render();
	void Clear_ColliderGroup();

private:
	map<const std::wstring, list<CCollider*>> m_ColliderGroup;
	bool m_bRender;

private:
	virtual void		Free();
};

END
