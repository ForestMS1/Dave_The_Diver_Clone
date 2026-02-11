#pragma once
#include "CScene.h"
class CTransition : public CScene
{
public:
	enum SCENE_ID
	{
		SCENE_INIT,
		SCENE_LOGO,
		SCENE_SHIP,
		SCENE_DIVE,
		SCENE_SUSHI,
		SCENE_ID_END
	};

protected:
	explicit CTransition(SCENE_ID eSrcScene, SCENE_ID eDstScene);
	virtual ~CTransition();

public:
	pair<std::wstring, map<const std::wstring, CLayer*>> Before_SceneChange() override;
	void After_SceneChange(const pair<std::wstring, map<const std::wstring, CLayer*>>& transfer) override;

private:
	pair < std::wstring, map<const std::wstring, CLayer*>> m_reserveTransfer;

public:
	SCENE_ID Get_SrcScene() const { return m_eSrcScene; }
	SCENE_ID Get_DstScene() const { return m_eDstScene; }

	CRITICAL_SECTION* Get_Crt() { return &m_Crt; }

public:
	HRESULT Transition_INIT_TO_LOGO();
	HRESULT Transition_LOGO_TO_SHIP();
	HRESULT Transition_SHIP_TO_LOGO();
	HRESULT Transition_SHIP_TO_DIVE();
	HRESULT Transition_DIVE_TO_SHIP();
	HRESULT Transition_SHIP_TO_SUSHI();
	HRESULT Transition_SUSHI_TO_SHIP();


public:
	HRESULT		Ready_Scene() override;
	_int		Update_Scene(const _float& fTimeDelta) override;
	void		LateUpdate_Scene(const _float& fTimeDelta) override;
	void		Render_Scene() override;

public:
	static CTransition* Create(SCENE_ID eSrcScene, SCENE_ID eDstScene);

public:
	static unsigned int CALLBACK Thread_Main(void* pArg);

private:
	const SCENE_ID m_eSrcScene;
	const SCENE_ID m_eDstScene;

	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	_bool				m_bFinish;

	wstring m_sComment;

private:
	virtual void	Free();
};

