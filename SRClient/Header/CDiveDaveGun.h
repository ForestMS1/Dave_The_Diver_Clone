#pragma once
#include "CGameObject.h"
#include "CAttackReadyArmTex.h"
#include "CGameMemMgr.h"
class CDiveDaveGun :
    public CGameObject
{
private:
	explicit CDiveDaveGun(CGameMemMgr::CDaveInfo::DAVE_GUN eGun);
	explicit CDiveDaveGun(const CDiveDaveGun& rhs);
	virtual ~CDiveDaveGun();


public:
	void		Init();
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

public:
	CGameMemMgr::CDaveInfo::DAVE_GUN Get_eGun() const { return m_eCurGun; }
	// 게임 중간에 총 변경
	void		Change_Gun(CGameMemMgr::CDaveInfo::DAVE_GUN eGun);

	void		Fire();

private:
	HRESULT Ready_Component();
	void	Set_ParentTransform();
	void	Rotate_ToMouse();

	void	Set_Size();
	void	Reset_Size();

private:
	Engine::CAttackReadyArmTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	_bool m_bIsFlip = false;
	_bool m_bInitComplete = false;


	CGameMemMgr::CDaveInfo::DAVE_GUN m_eCurGun = CGameMemMgr::CDaveInfo::DAVE_GUN::GUN_DEFAULT;

public:
	static CDiveDaveGun* Create(CGameMemMgr::CDaveInfo::DAVE_GUN eGun = CGameMemMgr::CDaveInfo::DAVE_GUN::GUN_DEFAULT);

private:
	virtual void Free() override;
};

