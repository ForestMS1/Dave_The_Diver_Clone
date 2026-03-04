#pragma once
#include "CDiveItem.h"
class CCommonItemRuby : public CDiveItem
{
private:
	explicit CCommonItemRuby(_vec3 vOriginPos);
	explicit CCommonItemRuby(const CCommonItemRuby& rhs);
	virtual ~CCommonItemRuby();


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
	static CCommonItemRuby* Create(_vec3 vOriginPos);

private:
	virtual void Free() override;
};

