#pragma once
#include "CDiveItem.h"

class CAmmoPack : public CDiveItem
{
private:
	explicit CAmmoPack(_vec3 vOriginPos);
	explicit CAmmoPack(const CAmmoPack& rhs);
	virtual ~CAmmoPack();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	virtual void GetItem() override; // 플레이어쪽에서 호출 할 함수

	// CDiveItem을(를) 통해 상속됨
	void UseItem(CGameObject*) override;
	void OpenItemDesc() override; //[LSY]

private:
	HRESULT Ready_Component();

public:
	static CAmmoPack* Create(_vec3 vOriginPos);

private:
	virtual void Free() override;
};

