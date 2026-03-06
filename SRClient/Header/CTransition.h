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
		SCENE_ENDING,
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

	void Set_Finish() { m_bFinish = true; }

public:
	// init으로 올일이 없으니 이 전환 과정에서
	// 로드되는 에셋 프로토타입들은 계속 사용되는 애들이다.
	HRESULT Transition_INIT_TO_LOGO();
	HRESULT Transition_LOGO_TO_SHIP();
	HRESULT Transition_SHIP_TO_LOGO();
	HRESULT Transition_SHIP_TO_DIVE();
	HRESULT Transition_DIVE_TO_SHIP();
	HRESULT Transition_SHIP_TO_SUSHI();
	HRESULT Transition_SUSHI_TO_SHIP();
	HRESULT Transition_SUSHI_TO_ENDING();

private:
	HRESULT Common_SHIP_Load();
	HRESULT Common_SHIP_Unload();

	HRESULT Common_Logo_Env_Load();
	HRESULT Common_Logo_Env_Unload();


public:
	HRESULT			Ready_Environment_Layer(std::wstring_view svLayerTag);

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

	bool m_bFadeEnd;

	bool m_bLoadingStart;
private:
	virtual void	Free();

public:
	static void FadedTransition(SCENE_ID eSrcScene, SCENE_ID eDstScene);
	static bool s_LogoAssetLoaded;
	static bool s_ShipAssetLoaded;
	static bool s_DiveAssetLoaded;
	static bool s_SushiAssetLoaded;
	static bool s_EndingAssetLoaded;

private:
	void AddFadeIn(CScene* pScene, function<void()> funcOnEnd = nullptr);
	void AddFadeOut(CScene* pScene, function<void()> funcOnEnd = nullptr);

private:
	void Update_Camera();
};