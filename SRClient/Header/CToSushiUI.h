#pragma once
#include "CGameObject.h"
class CToSushiUI : public CGameObject
{
private:
	explicit CToSushiUI(float fPosX, float fPosY);
	virtual ~CToSushiUI();

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

private:
	bool m_bOpen;
	tweeny::tween<float> m_tweenOpen;
	bool m_bOpenTween;


	// Money
// LV
	// CNT
	// Title
	// RANK
public:
	// Money
	std::wstring_view Get_Money() const { return m_sMoney; }
	void Set_Money(std::wstring_view _sMoney) { m_sMoney = _sMoney; }

	// Lv
	std::wstring_view Get_Lv() const { return m_sLv; }
	void Set_Lv( std::wstring_view _sLv) { m_sLv = _sLv; }

	// Cnt
	std::wstring_view Get_Cnt() const { return m_sCnt; }
	void Set_Cnt( std::wstring_view _sCnt) { m_sCnt = _sCnt; }

	// Title
	std::wstring_view Get_Title() const { return m_sTitle; }
	void Set_Title( std::wstring_view _sTitle) { m_sTitle = _sTitle; }

	// Rank
	std::wstring_view Get_Rank() const { return m_sRank; }
	void Set_Rank( std::wstring_view _sRank) { m_sRank = _sRank; }
private:
	std::wstring m_sMoney;
	std::wstring m_sLv;
	std::wstring m_sCnt;
	std::wstring m_sTitle;
	std::wstring m_sRank;


public:
	static CToSushiUI* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

