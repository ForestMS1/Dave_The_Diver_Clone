#pragma once
#include "CGameObject.h"
#include "CAABB.h"

class CHoldFishUIImg;

class CHoldFishUIItemArea : public CGameObject
{
private:
	explicit CHoldFishUIItemArea(_uint iIdx, float fPosX, float fPosY);
	virtual ~CHoldFishUIItemArea();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	HRESULT Ready_AfterCreate();

public:
	void Set_PosY(float f) { m_fPosY = f; }
	_uint Get_Idx() const { return m_iIdx; }

private:
	HRESULT			Ready_Component();

public:
	void Set_EdgeVisible(bool bVisible);
	bool Get_EdgeVisible() const { return m_bEdgeVisible; }

public:
	virtual void Update_ImGui();
private:
	float m_fDbgX = 0.f;
	float m_fDbgY = 0.f;

private:
	const float m_fPosX;
	float m_fPosY;

	bool m_bEdgeVisible;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	CAABB* m_pAABB;

private:
	CHoldFishUIImg* m_pEdgeImg = nullptr;

public:
	void Set_Title(std::wstring_view svTitle) { m_sTitle = svTitle; }
	void Set_Thumbnail(std::wstring_view svName) { m_sThumbNailAssetName = svName; }
	void Set_MeatCnt(_uint i) { m_iMeatCnt = i; }
	void Set_Weight(float f) { m_fWeight = f; }
	void Set_Rank(_uint i) { m_iRank = i; }
	void Set_Star(_uint i) {
		m_iStar = i;
	}

private:
	std::wstring m_sTitle;
	const _uint m_iIdx = 0;
	std::wstring m_sThumbNailAssetName;
	_uint m_iMeatCnt;
	float m_fWeight;
	_uint m_iRank;
	_uint m_iStar;

public:
	static CHoldFishUIItemArea* Create(_uint m_iIdx, float fPosX, float fPosY);

private:
	virtual void Free();
};

