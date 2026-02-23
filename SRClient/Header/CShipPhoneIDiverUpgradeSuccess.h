#pragma once
#include "CGameObject.h"
class CShipPhoneIDiverUpgradeSuccess : public CGameObject
{
private:
	explicit CShipPhoneIDiverUpgradeSuccess(float fPosX, float fPosY);
	virtual ~CShipPhoneIDiverUpgradeSuccess();

public:
	void Set_Title(std::wstring_view sv) { m_sTitle = sv; }
	void Set_Top(std::wstring_view sv) { m_sTop = sv; }
	void Set_Bottom(std::wstring_view sv) { m_sBottom = sv; }
	void Set_Desc(std::wstring_view sv) { m_sDesc = sv; }
	void Set_AssetName(std::wstring_view sv) { m_sAssetName = sv; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
	void Ready_AfterCreate();

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	// Title
// TOP
	// Bottom
	// Desc

	std::wstring m_sTitle;
	std::wstring m_sTop;
	std::wstring m_sBottom;
	std::wstring m_sDesc;
	std::wstring m_sAssetName;

public:
	static CShipPhoneIDiverUpgradeSuccess* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

