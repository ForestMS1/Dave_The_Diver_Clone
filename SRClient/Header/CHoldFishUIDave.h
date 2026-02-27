#pragma once
#include "CGameObject.h"
class CHoldFishUIDave : public CGameObject
{
private:
	explicit CHoldFishUIDave();
	explicit CHoldFishUIDave(const CHoldFishUIDave& rhs) = delete;
	virtual ~CHoldFishUIDave();

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
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTextureFrame* m_pTextureFrameCom;

public:
	static CHoldFishUIDave* Create();

private:
	virtual void Free();
};

