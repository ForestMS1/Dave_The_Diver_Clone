#pragma once
#include "CGameObject.h"
#include "CAABB.h"
enum class ITEMSTATE
{
	STARTDROP = 0,
	DROPPED,
	ACQUIRED,
	STATE_END
};

class CDiveItem : public CGameObject
{
protected:
	explicit CDiveItem(_vec3 vOriginPos);
	explicit CDiveItem(const CDiveItem& rhs);
	virtual ~CDiveItem();

public:
	void GetItem() { if(m_eCurState == ITEMSTATE::DROPPED) m_eCurState = ITEMSTATE::ACQUIRED; } // 플레이어쪽에서 호출 할 함수
	virtual void UseItem(CGameObject*) = 0;
	std::wstring_view Get_TexName() { return m_wsTexName; }

protected:
	void FSM(const _float& fTimeDelta);
	void StartDrop(const _float& fTimeDelta);
	void	Collision_With_DiveDave();

	// 충돌
	void	OnCollisionEnter(CCollider*);
	void	OnCollisionStay(CCollider*);
	void	OnCollisionExit(CCollider*);

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB; // DiveDave와 충돌체크 용
	ITEMSTATE m_eCurState = ITEMSTATE::STARTDROP;

protected:
	_vec3 m_vOriginPos; // 드랍 시작 위치, 드랍 멈출 위치
	_float m_fAccTime = 0.f;
	_bool m_bIsCollWithMe = false;
	wstring m_wsTexName;

protected:
	virtual void Free() override;
};

