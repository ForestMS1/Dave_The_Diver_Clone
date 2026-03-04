#pragma once
#include "CGameObject.h"
class CDiveItemDescUI : public CGameObject
{
private:
	explicit CDiveItemDescUI(float fPosX, float fPosY);
	virtual ~CDiveItemDescUI();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	HRESULT Ready_AfterCreate();

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	void Set_Title(std::wstring_view svTitle) { m_sTitle = svTitle; }
	void Set_Desc(std::wstring_view svDesc) { m_sDesc = svDesc; }

private:
	std::wstring m_sTitle;
	std::wstring m_sDesc;

public:
	static CDiveItemDescUI* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

