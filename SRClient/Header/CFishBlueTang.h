#pragma once
#include "CFishGameObject.h"

// swim, die
class CFishBlueTang : public CFishGameObject
{
private:
	explicit CFishBlueTang(float fPosX, float fPosY);
	explicit CFishBlueTang(const CFishBlueTang& rhs) = delete;
	virtual ~CFishBlueTang();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	Engine::CDynamicBuffer* m_pDynamicBuffer;
	//Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CSpine* m_pSpineCom;

private:
	const float m_fPosX;
	const float m_fPosY;

public:
	static CFishBlueTang* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

