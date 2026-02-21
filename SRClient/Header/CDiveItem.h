#pragma once
#include "CGameObject.h"

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
	virtual void UseItem() = 0;

protected:
	void FSM(const _float& fTimeDelta);
	void StartDrop(const _float& fTimeDelta);


protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	ITEMSTATE m_eCurState = ITEMSTATE::STARTDROP;

protected:
	_vec3 m_vOriginPos; // 드랍 시작 위치, 드랍 멈출 위치
	_float m_fAccTime = 0.f;

protected:
	virtual void Free() override;
};

