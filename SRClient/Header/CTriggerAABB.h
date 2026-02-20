#pragma once
#include "CGameObject.h"
#include "CAABB.h"
#include "CCollider.h"

class CTriggerAABB : public CGameObject
{
private:
	explicit CTriggerAABB(wstring_view svColliderTag, wstring_view svColliderGroupTag);
	virtual ~CTriggerAABB();

public:
	Engine::CTransform* Get_Transform() const { return m_pTransformCom; }

	void Set_OnTriggerEnter(function<void(CCollider*)> func) { m_funcOnEnter = func; };
	void Set_OnTriggerExit(function<void(CCollider*)> func) { m_funcOnExit = func; };
	void Set_OnTriggerStay(function<void(CCollider*)> func) { m_funcOnStay = func; };

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

	function<void(CCollider*)> m_funcOnEnter;
	function<void(CCollider*)> m_funcOnStay;
	function<void(CCollider*)> m_funcOnExit;

	const wstring m_sColliderTag;
	const wstring m_sColliderGroupTag;

	unordered_map<CCollider*, bool> m_mapTrigger;
	
public:
	static CTriggerAABB* Create(wstring_view svColliderTag, wstring_view svColliderGroupTag);

private:
	virtual void Free();
};

