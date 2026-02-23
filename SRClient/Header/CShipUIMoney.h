#pragma once
#include "CGameObject.h"
class CShipUIMoney : public CGameObject
{
private:
	explicit CShipUIMoney(float fPosX, float fPosY);
	virtual ~CShipUIMoney();

public:
	 void Update_ImGui() override;

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

	float m_fFontOffsetX;
	float m_fFontOffsetY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CShipUIMoney* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

