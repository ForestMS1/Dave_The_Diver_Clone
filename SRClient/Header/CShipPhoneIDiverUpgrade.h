#pragma once
#include "CGameObject.h"
class CShipPhoneIDiverUpgrade : public CGameObject
{
private:
	explicit CShipPhoneIDiverUpgrade(float fPosX, float fPosY);
	virtual ~CShipPhoneIDiverUpgrade();

public:
	void Set_AssetName(std::wstring_view sv) { m_sAssetName = sv; }

	void Set_Title(std::wstring_view sv) { m_sTitle = sv; }
	void Set_LeftTop(std::wstring_view sv) { m_sLeftTop = sv; }
	void Set_LeftMiddle(std::wstring_view sv) { m_sLeftMiddle = sv; }
	void Set_LeftBottom(std::wstring_view sv) { m_sLeftBottom = sv; }
	void Set_RightTop(std::wstring_view sv) { m_sRightTop = sv; }
	void Set_RightMiddle(std::wstring_view sv) { m_sRightMiddle = sv; }
	void Set_RightBottom(std::wstring_view sv) { m_sRightBottom = sv; }
	void Set_Desc(std::wstring_view sv) { m_sDesc = sv; }
	void Set_Money(std::wstring_view sv) { m_sMoney = sv; }

	void Set_MoneyLack(bool bLack) { m_bMoneyLack = bLack; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void	Ready_AfterCreate();

private:
	HRESULT			Ready_Component();
	

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	bool m_bSuccessOpen;

	std::wstring m_sAssetName;

	// Title
// LeftTop
	// LeftMiddle
	// LeftBottom
	// RgihtTop
	// RgihtMiddle
	// RgihtBottom
	// Desc
	std::wstring m_sTitle;
	std::wstring m_sLeftTop;
	std::wstring m_sLeftMiddle;
	std::wstring m_sLeftBottom;
	std::wstring m_sRightTop;
	std::wstring m_sRightMiddle;
	std::wstring m_sRightBottom;
	std::wstring m_sDesc;
	std::wstring m_sMoney;

	bool m_bMoneyLack;
public:
	static CShipPhoneIDiverUpgrade* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

