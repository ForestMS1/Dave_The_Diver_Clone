#pragma once
#include "CGameObject.h"
class CHoldFishUIItemGroup : public CGameObject
{
private:
	explicit CHoldFishUIItemGroup(float fPosX, float fPosY);
	virtual ~CHoldFishUIItemGroup();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	void Update_ImGui() override;

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;
	float m_fOffsetY;
	float m_fVelocityY = 0.f;

private:
	float m_fDbgX;
	float m_fDbgY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	LPDIRECT3DTEXTURE9 m_pRenderTargetTex = nullptr;
	LPDIRECT3DSURFACE9 m_pRenderTargetSurf = nullptr;

public:
	static CHoldFishUIItemGroup* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

