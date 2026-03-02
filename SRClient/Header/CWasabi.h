#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CWasabi : public CGameObject
{
private:
	explicit CWasabi();
	explicit CWasabi(const CGameObject& rhs);
	virtual ~CWasabi();
	void	Update_ImGui() override;


public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pWasabiTextureCom;
	Engine::CTransform* m_pTransformCom;
public:
	static CWasabi* Create();

private:
	_float			m_fFrame;
	virtual void Free();
	void	Key_Input();


private:
	bool	frameMove;
	bool	wasabiCreated;
	float	m_fScale;
	float   good = 0.f;
public:
	float   gauge = -4.04f;

};

