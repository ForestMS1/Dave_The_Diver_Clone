#pragma once
#include "CScene.h"
#include "CBoundingFrustum.h"
class CDive : public CScene
{
private:
	explicit CDive();
	virtual ~CDive();

public:
	HRESULT		Ready_Scene() override;
	_int		Update_Scene(const _float& fTimeDelta) override;
	void		LateUpdate_Scene(const _float& fTimeDelta) override;
	void		Render_Scene() override;
	void		Frustum();
	void		DeepDark();
private:
	HRESULT Ready_GameLogic_Layer(std::wstring_view svLayerTag);
	HRESULT Ready_Environment_Layer(std::wstring_view svLayerTag);
	HRESULT Ready_UI_Layer(std::wstring_view svLayerTag);
	HRESULT Ready_Fish_Layer(std::wstring_view svLayerTag);
public:
	static CDive* Create();

private:
	CGameObject* m_pDive = nullptr; // Dive구독용 // 여기서 해제하지않는다!
	CBoundingFrustum* m_pFrustumCollider{nullptr};

private:
	void Free() override;



};

