#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CLogoBtnArea : public CGameObject
{
private:
	explicit CLogoBtnArea(float fPosX, float fPosY);
	virtual ~CLogoBtnArea();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	CAABB* m_pAABB[5];
	_uint m_iSelected;

public:
	static CLogoBtnArea* Create(float fPosX, float fPosY);

private:
	virtual void Free(); 
};

