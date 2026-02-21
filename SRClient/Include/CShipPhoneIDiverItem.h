#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CShipPhoneIDiverItem : public CGameObject
{
private:
	explicit CShipPhoneIDiverItem(_int iIdx, float fPosX, float fPosY);
	virtual ~CShipPhoneIDiverItem();

public:
	_uint	Get_Idx() const { return m_iIdx; }

	void	Set_Title(std::wstring_view svS) { m_sTitle = svS; }
	void	Set_LeftTop(std::wstring_view svS) { m_sLeftTop = svS; }
	void	Set_LeftMiddle(std::wstring_view svS) { m_sLeftMiddle = svS; }
	void	Set_LeftBottom(std::wstring_view svS) { m_sLeftBottom = svS; }
	void	Set_RightTop(std::wstring_view svS) { m_sRightTop = svS; }
	void	Set_RightMiddle(std::wstring_view svS) { m_sRightMiddle = svS; }
	void	Set_RightBottom(std::wstring_view svS) { m_sRightBottom = svS; }
	void	Set_AssetName(std::wstring_view svS) { m_sAssetName = svS; }
	void	Set_Money(std::wstring_view svS) { m_sMoney = svS; }
	void	Set_MoneyLack(bool bLack) { m_bMoneyLack = bLack; }

	void Ready_AfterCreate();

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
	const _uint m_iIdx;

	std::wstring m_sTitle;
	std::wstring m_sLeftTop;
	std::wstring m_sLeftMiddle;
	std::wstring m_sLeftBottom;
	std::wstring m_sRightTop;
	std::wstring m_sRightMiddle;
	std::wstring m_sRightBottom;
	std::wstring m_sMoney;
	std::wstring m_sAssetName;
	bool m_bMoneyLack;

private:
	CAABB* m_pAABB;

public:
	static CShipPhoneIDiverItem* Create(_int iIdx, float fPosX, float fPosY);

private:
	virtual void Free();
};

