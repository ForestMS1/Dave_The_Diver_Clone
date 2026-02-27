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
	void Set_Rank(std::wstring_view svRank) { m_sRank = svRank; }
	void Set_Weight(std::wstring_view svWeight) { m_sWeight = svWeight; }

private:
	std::wstring m_sTitle;
	std::wstring m_sRank;
	std::wstring m_sWeight;
	const _uint m_iIdx = 0;



public:
	static CHoldFishUIItemArea* Create(_uint m_iIdx, float fPosX, float fPosY);

private:
	virtual void Free();
};

