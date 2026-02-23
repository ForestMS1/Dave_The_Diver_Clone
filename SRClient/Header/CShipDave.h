#pragma once
#include "CGameObject.h"
#include "CAABB.h"
#include "CTransition.h"
class CShipDave : public CGameObject
{
private:
	explicit CShipDave();
	explicit CShipDave(const CShipDave& rhs);
	virtual ~CShipDave();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	void DoDiveReady()
	{
		CTransition::FadedTransition(CTransition::SCENE_SHIP, CTransition::SCENE_DIVE);
		m_bDiveReady = true;
	};

private:
	HRESULT			Ready_Component();
	void			Key_Input(const _float& fTimeDelta);
	void			Motion_Change(wstring_view svMotion);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

	wstring m_sCurrentMotion;
	_uint			m_iFrame;
	_float m_fAccFrameDelta;
	bool m_bSeeRight;


	bool m_bDiveReady;

public:
	static CShipDave* Create();

private:
	virtual void Free();

};

