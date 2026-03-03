#pragma once
#include "CDiveItem.h"
class CItemTripleAxel :
    public CDiveItem
{
private:
	explicit CItemTripleAxel(_vec3 vOriginPos);
	explicit CItemTripleAxel(const CItemTripleAxel& rhs);
	virtual ~CItemTripleAxel();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	virtual void GetItem() override 
	{ 
		if (m_eCurState == ITEMSTATE::DROPPED) m_eCurState = ITEMSTATE::ACQUIRED; 

	} // 플레이어쪽에서 호출 할 함수

	// CDiveItem을(를) 통해 상속됨
	void UseItem(CGameObject*) override;

private:
	HRESULT Ready_Component();

public:
	static CItemTripleAxel* Create(_vec3 vOriginPos);

private:
	virtual void Free() override;
};

