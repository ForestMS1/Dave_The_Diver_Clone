#pragma once
#include "CScene.h"
class CShip : public CScene
{
private:
	explicit CShip();
	virtual ~CShip();

public:
	HRESULT		Ready_Scene() override;
	_int		Update_Scene(const _float& fTimeDelta) override;
	void		LateUpdate_Scene(const _float& fTimeDelta) override;
	void		Render_Scene() override;

private:
	HRESULT			Ready_GameLogic_Layer(std::wstring_view svLayerTag);

public:
	static CShip* Create();

private:
	void Free() override;

private:
	bool m_bResultOpend;
	bool m_bTalking;
	bool m_bFireworkSound;
	float m_fResultTimer;
	float m_fConvAppearTimer;
	float m_fFireworkTimer;
	float m_fFireworkSoundTimer;

};

