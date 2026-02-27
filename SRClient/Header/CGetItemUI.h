#pragma once
#include "CGameObject.h"
class CGetItemUI : public CGameObject
{
private:
	explicit CGetItemUI(float fPosX, float fPosY);
	virtual ~CGetItemUI();

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

public:
	void Set_Title(std::wstring_view svTitle) { m_sTitle = svTitle; }
	void Set_Rank(std::wstring_view svRank) { m_sRank = svRank; }
	void Set_Weight(std::wstring_view svWeight) { m_sWeight = svWeight; }

private:
	tweeny::tween<float> m_tween;
	std::wstring m_sTitle;
	std::wstring m_sRank;
	std::wstring m_sWeight;
	int m_iStartCnt;

public:
	static CGetItemUI* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

