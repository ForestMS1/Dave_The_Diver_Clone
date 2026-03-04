#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CBancho : public CGameObject
{
private:
	explicit CBancho();
	explicit CBancho(const CGameObject& rhs);
	virtual ~CBancho();
	void	Update_ImGui() override;


public:
	enum State{IDLE, COOK, STATE_END};
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pIdleTextureCom;
	Engine::CTexture* m_pCookTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

public:
	static CBancho* Create();

private:
	State			curState;
	_float			m_fFrame;
	wstring			m_sFishName;
	wstring			m_sTexName;
	virtual void Free();
public:
	float			m_fGauge = -1.f;
	bool			wasabiUse;
	bool			spending;

};

