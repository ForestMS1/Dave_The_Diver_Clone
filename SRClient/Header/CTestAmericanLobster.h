#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CTestAmericanLobster : public CGameObject
{
private:
	explicit CTestAmericanLobster();
	explicit CTestAmericanLobster(const CTestAmericanLobster& rhs) = delete;
	virtual ~CTestAmericanLobster();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();
	void			Key_Input(const _float& fTimeDelta);
	void			Motion_Change(wstring_view svMotion);

private:
	Engine::CDynamicBuffer* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

	wstring m_sCurrentMotion;
	_uint			m_iFrame;
	_float m_fAccFrameDelta;
	bool m_bSeeRight;

public:
	static CTestAmericanLobster* Create();

private:
	virtual void Free();
};

