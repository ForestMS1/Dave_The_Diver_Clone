#pragma once
#include "CGameObject.h"
#include "CBaseState.h"
#include "CAABB.h"
#include "CFSM.h"
class CJohn : public CGameObject
{
private:
	explicit			CJohn();
	explicit			CJohn(const CJohn& rhs);
	virtual				~CJohn();

public:
	HRESULT				Ready_GameObject() override;
	_int				Update_GameObject(const _float& fTimeDelta) override;
	void				LateUpdate_GameObject(const _float& fTimeDelta) override;
	void				Render_GameObject() override;

public:
	JOHNSTATE			Get_State()									{ return m_pFSM->Get_State(); }
	void				Set_State(JOHNSTATE state)					{ m_pFSM->Set_State(state); }

	void				Multiply_Scale(_vec3* vScale)				{ m_pTransformCom->Multiply_Scale(vScale); }
	void				Move(_vec3* vDir, const _float& fTimeDelta);
	void				Set_RotateDir(_vec3* vDir)
	{
		m_pTransformCom->m_vAngle.x = vDir->x;
		m_pTransformCom->m_vAngle.y = vDir->y;
		m_pTransformCom->m_vAngle.z = vDir->z;
	}

	void				Get_Pos(_vec3* vPos)						{ return m_pTransformCom->Get_Info(INFO_POS, vPos); }

	_float				Get_Frame()									{ return m_fFrame; };
	void				Init_Frame()								{ m_fFrame = 0.f; }
	void				AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size);

	// 어느쪽 보고있는지
	_bool				Is_Flip()															const	{ return m_bFlip; }
	void				Set_Flip(_bool isFlip)														{ m_bFlip = isFlip; }

	// 전역 상태 바로 진입
	_bool				Check_GlobalState();

private:
	HRESULT				Ready_Component();
	HRESULT				Add_State();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB; // 아이템 상자랑 충돌 용


private:
	_float	m_fFrame = 0.f;
	_bool	m_bFlip = false;


private:
	CFSM<CJohn, JOHNSTATE>* m_pFSM = nullptr;

public:
	static CJohn* Create();

private:
	virtual void Free() override;
};

