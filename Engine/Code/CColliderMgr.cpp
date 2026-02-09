#include "CColliderMgr.h"

#include "CCollider.h"

IMPLEMENT_SINGLETON(CColliderMgr)

CColliderMgr::CColliderMgr()
	: m_bRender(false)
{
}

CColliderMgr::~CColliderMgr()
{
    Free();
}

void CColliderMgr::Clear_ColliderGroup()
{

	for (auto& colliderPair : m_ColliderGroup)
	{
		// 딜리트의 책임은 여기서 지지 않는다.
		//for_each(colliderPair.second.begin(), colliderPair.second.end(), CDeleteObj());
		colliderPair.second.clear();
	}
	m_ColliderGroup.clear();
}

list<CCollider*>* CColliderMgr::Get_Colliders(std::wstring_view svObjTag)
{
	auto	iter = find_if(m_ColliderGroup.begin(), m_ColliderGroup.end(),
		CTag_FinderSV(svObjTag));

	if (iter == m_ColliderGroup.end())
		return nullptr;

	return &(iter->second);
}

void CColliderMgr::AddColliderGroup(std::wstring_view svLayerTag, CCollider* const pCollider)
{
	if (nullptr == pCollider)
		return;

	auto	iter = find_if(m_ColliderGroup.begin(), m_ColliderGroup.end(),
		CTag_FinderSV(svLayerTag));

	// 그룹에 추가되면 색은 초기화 시켜줌
	pCollider->Set_CurrentColorOriginal();

	if (iter == m_ColliderGroup.end())
	{
		m_ColliderGroup.insert({ std::wstring(svLayerTag), {pCollider} });
	}
	else
	{
		iter->second.push_back(pCollider);
	}
}

void CColliderMgr::Render()
{
	if (m_bRender)
	{
		for (auto& colliderPair : m_ColliderGroup)
		{
			for (auto& pCollider : colliderPair.second)
			{
				pCollider->Render();
			}
		}
	}
}


void CColliderMgr::Free()
{
	Clear_ColliderGroup();
}
