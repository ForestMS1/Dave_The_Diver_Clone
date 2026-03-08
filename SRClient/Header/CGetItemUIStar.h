#pragma once
#include "CGameObject.h"
class CGetItemUIStar : public CGameObject
{
private:
	explicit CGetItemUIStar(float fPosX, float fPosY);
	virtual ~CGetItemUIStar();

public:
	void Update_ImGui() override;

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	void Set_Empty() { m_bEmpty = true; }

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;
	float m_fDbgX = 0.f;
	float m_fDbgY = 0.f;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	bool m_bEmpty = false;

public:
	static CGetItemUIStar* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

