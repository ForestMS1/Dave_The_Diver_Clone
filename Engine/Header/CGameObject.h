#pragma once
#include "CBase.h"
#include "Engine_Define.h"


namespace Engine
{
	class CComponent;
}

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject();
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent* Get_Component(COMPONENTID eID, std::wstring_view svComponentTag);
	_float		Get_ViewZ() { return m_fViewZ; }

public:
	virtual void Update_ImGui();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() = 0;

public:
	void			Compute_ViewZ(const _vec3* pPos);
	void			Set_Dead() { m_bDead = true; }
	bool			Get_Dead() const { return m_bDead; }
	void			Set_Tag(std::wstring_view svTag) { m_sTag; }
	wstring_view	Get_Tag() { return m_sTag; }
	
protected:
	map<const std::wstring, CComponent*>	m_mapComponent[ID_END];

	_float									m_fViewZ;
	bool									m_bDead;
	std::wstring							m_sTag;

// 계층 관련
public:
	CGameObject * const Get_Parent() const { return m_pParentGameObject; }
	list<CGameObject*> * const  Get_Children()  { return &m_childGameObjectList; }
	void Set_Parent(CGameObject* const pNewParent);
protected:
	CGameObject* m_pParentGameObject;
	list<CGameObject*> m_childGameObjectList;
private:
	void EraseChildGameObject(CGameObject* pParentGameObject, CGameObject* pDelTarget);



protected:
	// - 사용예시
	// if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
	// 	return E_FAIL;
	// 
	// - 템플릿
	// T1 다이나믹 캐스팅할 타입(포인터붙이이말고)
	// T2 COMPONENTID
	// 
	// - 파라미터
	// P1 std::wstring_view svProtoTag:  프로토 매니저에서 가져올 원형 이름
	// P2 std::wstring_view svComponentName:  추가할 컴포넌트의 이름
	// P3 T** ppComponent: 만들어진 컴포넌트 주소를 받아오고싶으면 넣으시고 필요없으면 넣지않으면됨
	template<typename T, COMPONENTID eComID>
	HRESULT AddComponent(std::wstring_view svProtoTag, std::wstring_view svComponentName, T** ppComponent = nullptr);
	
public:
	template<typename T, COMPONENTID eComID>
	T* GetComponent(std::wstring_view svComponentTag);

private:
	CComponent* Find_Component(COMPONENTID eID, std::wstring_view svComponentTag);

protected:
	virtual		void		Free();
};

END

// TODO: inl 화
#include "CProtoMgr.h" // TODO: 헤더충돌이 날수도 있으니 고민
template<typename T, COMPONENTID eComID>
inline HRESULT CGameObject::AddComponent(std::wstring_view svProtoTag, std::wstring_view svComponentName, T** ppComponent)
{
	T* pComponent = dynamic_cast<T*>(CProtoMgr::GetInstance()->Clone_Prototype(svProtoTag));
	if (nullptr == pComponent)
		return E_FAIL;

	if (ppComponent)
	{
		*ppComponent = pComponent;
	}
		
	pComponent->Set_GameObject(this);

	m_mapComponent[eComID].insert({ std::wstring(svComponentName), pComponent });

	return S_OK;
}

template<typename T, COMPONENTID eComID>
inline T* CGameObject::GetComponent(std::wstring_view svComponentTag)
{
	CComponent* pComponent = Find_Component(eComID, svComponentTag);
	if (nullptr == pComponent)
		return nullptr;

	return dynamic_cast<T*>(pComponent);
}
