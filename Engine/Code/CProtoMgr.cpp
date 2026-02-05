#include "CProtoMgr.h"
IMPLEMENT_SINGLETON(CProtoMgr)

CProtoMgr::CProtoMgr()
{

}

CProtoMgr::~CProtoMgr()
{
	Free();
}

HRESULT CProtoMgr::Ready_Prototype(std::wstring_view svComponentTag,
									CComponent* pComponent)
{
	CComponent* pInstance = Find_Prototype(svComponentTag);

	if (nullptr != pInstance)
		return E_FAIL;

	m_mapPrototype.insert({ std::wstring(svComponentTag), pComponent });

	return S_OK;
}

CComponent* CProtoMgr::Clone_Prototype(std::wstring_view svComponentTag)
{
	CComponent* pInstance = Find_Prototype(svComponentTag);

	if (nullptr == pInstance)
		return nullptr;
	
	
	return pInstance->Clone();
}

CComponent* CProtoMgr::Find_Prototype(std::wstring_view svComponentTag)
{
	auto		iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(),
		CTag_FinderSV(svComponentTag));

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}

void CProtoMgr::Free()
{
	for_each(m_mapPrototype.begin(), m_mapPrototype.end(), CDeleteMap());
	m_mapPrototype.clear();
}
