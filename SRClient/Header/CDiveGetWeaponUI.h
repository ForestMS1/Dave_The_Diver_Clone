#pragma once
#include "CGameObject.h"
class CDiveGetWeaponUI : public CGameObject
{
private:
	explicit CDiveGetWeaponUI(float fPosX, float fPosY);
	virtual ~CDiveGetWeaponUI();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	void Update_ImGui();

public:
	HRESULT Ready_AfterCreate();

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;
	float m_fDbgX;
	float m_fDbgY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	void Set_Title(std::wstring_view svTitle) { m_sTitle = svTitle; }
	void Set_Desc(std::wstring_view svDesc) { m_sDesc = svDesc; }
	void Set_ImgAssetName(std::wstring_view sv) { m_sImgAssetName = sv; }

private:
	tweeny::tween<float> m_tween;
	std::wstring m_sTitle;
	std::wstring m_sDesc;
	std::wstring m_sImgAssetName;

public:
	static CDiveGetWeaponUI* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

