#pragma once

#include	"CBase.h"
#include	"CScene.h"
#include "CRenderer.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit	CManagement();
	virtual		~CManagement();

public:
	CComponent* Get_FirstObjectComponent(COMPONENTID eID,
		std::wstring_view svLayerTag,
		std::wstring_view svObjTag,
		std::wstring_view svComponentTag);

	CScene* Get_Scene() const { return m_pScene; };

public:
	HRESULT			Set_Scene(CScene* pScene);
	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene(const _float& fTimeDelta);
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void			Update_ImGui();

private:
	CScene*					m_pScene;

public:
	virtual void			Free();
};

END