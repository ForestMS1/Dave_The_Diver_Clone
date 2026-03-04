#pragma once
#include "CDiveItem.h"
class CCommonItemRope : public CDiveItem
{
private:
	explicit CCommonItemRope(_vec3 vOriginPos);
	explicit CCommonItemRope(const CCommonItemRope& rhs);
	virtual ~CCommonItemRope();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	virtual void GetItem() override; // 플레이어쪽에서 호출 할 함수
	void UseItem(CGameObject*) override {};
	void OpenItemDesc() override;
private:
	HRESULT Ready_Component();

public:
	static CCommonItemRope* Create(_vec3 vOriginPos);

private:
	virtual void Free() override;
};

