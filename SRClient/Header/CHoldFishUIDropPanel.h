#pragma once
#include "CGameObject.h"

class CHoldFishUIDropPanel : public CGameObject
{
private:
	explicit CHoldFishUIDropPanel(float fPosX, float fPosY);
	virtual ~CHoldFishUIDropPanel();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

public:
	virtual void Update_ImGui();

private:
	float m_fDbgX = 0.f;
	float m_fDbgY = 0.f;

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	void Set_Title(std::wstring_view svTitle) { m_sTitle = svTitle; }

private:
	std::wstring m_sTitle;


public:
	static CHoldFishUIDropPanel* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};
