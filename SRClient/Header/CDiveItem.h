#pragma once
#include "CGameObject.h"
#include "CAABB.h"
#include "CGameMemMgr.h"
enum class ITEMSTATE
{
	STARTDROP = 0,
	DROPPED,
	ACQUIRED,
	STATE_END
};

enum class ITEMTYPE
{
	USEITEM = 0,
	USEIMMEDIATELY,
	WEAPONITEM,
	COMMONITEM, // [LSY]
	ITEMTYPE_END
};

class CDiveItem : public CGameObject
{
protected:
	explicit CDiveItem(_vec3 vOriginPos);
	explicit CDiveItem(const CDiveItem& rhs);
	virtual ~CDiveItem();

public:
	// [LSY] 아이템 획득 UI 호출
	virtual void GetItem();// 플레이어쪽에서 호출 할 함수
	virtual void UseItem(CGameObject*) = 0;
	std::wstring_view Get_TexName() { return m_wsTexName; }
	ITEMTYPE	GetItemType() const { return m_eItemType; }
	CGameMemMgr::CDaveInfo::DAVE_GUN GetGunType() const { return m_eGunType; }
	// [LSY] derived들이 아이템 설명 여는 함수
	virtual void OpenItemDesc() {};

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

	ITEMTYPE m_eItemType = ITEMTYPE::ITEMTYPE_END;
	CGameMemMgr::CDaveInfo::DAVE_GUN m_eGunType;

protected:
	virtual void Free() override;
};

