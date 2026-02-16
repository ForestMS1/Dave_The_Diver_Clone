#pragma once

#include "CBase.h"
#include "Engine_Define.h"

namespace Engine
{
	class CGameObject;
}

BEGIN(Engine)

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	CGameObject* const Get_GameObject() const { return m_pGameObject; }
	void Set_GameObject(CGameObject * pGameObject) { m_pGameObject = pGameObject; }


public:
	virtual void Update_ImGui() {};

public:
	virtual _int Update_Component(const _float& fTimeDelta) { return 0; }
	virtual void LateUpdate_Component() { }

protected:
	_bool						m_bClone;
	CGameObject*				m_pGameObject;

public:
	virtual CComponent* Clone()	PURE;		// 프로토타입 사용 예

protected:
	virtual void				Free();
};

END