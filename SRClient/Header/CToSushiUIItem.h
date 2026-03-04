#pragma once
#include "CGameObject.h"
class CToSushiUIItem : public CGameObject
{
private:
	explicit CToSushiUIItem(float fPosX, float fPosY);
	virtual ~CToSushiUIItem();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	HRESULT Ready_AfterCreate();

public:
	void Update_ImGui() override;

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	float m_fDbgX;
	float m_fDbgY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CToSushiUIItem* Create(float fPosX, float fPosY);

public:
	// Money
	std::wstring_view Get_Money() const { return m_sMoney; }
	void Set_Money(std::wstring_view _sMoney) { m_sMoney = _sMoney; }

	// Lv
	std::wstring_view Get_Lv() const { return m_sLv; }
	void Set_Lv(std::wstring_view _sLv) { m_sLv = _sLv; }

	// Cnt
	std::wstring_view Get_Cnt() const { return m_sCnt; }
	void Set_Cnt(std::wstring_view _sCnt) { m_sCnt = _sCnt; }

	// Title
	std::wstring_view Get_Title() const { return m_sTitle; }
	void Set_Title(std::wstring_view _sTitle) { m_sTitle = _sTitle; }

	// Rank
	std::wstring_view Get_Rank() const { return m_sRank; }
	void Set_Rank(std::wstring_view _sRank) { m_sRank = _sRank; }

	void Set_FishImgAssetName(std::wstring_view fish) { m_sFishImgAssetName = fish; }
	void Set_SushiImgAssetName(std::wstring_view fish) { m_sSushiImgAssetName = fish; }


private:
	std::wstring m_sMoney;
	std::wstring m_sLv;
	std::wstring m_sCnt;
	std::wstring m_sTitle;
	std::wstring m_sRank;
	std::wstring m_sFishImgAssetName;
	std::wstring m_sSushiImgAssetName;

private:
	virtual void Free();
};

