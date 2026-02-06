#pragma once
#include "CScene.h"
#include "CLoading.h"

class CLogo :   public CScene
{
protected:
	explicit CLogo();
	virtual ~CLogo();

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

private:
	HRESULT			Ready_Environment_Layer(std::wstring_view svLayerTag);
	HRESULT			Ready_GameLogic_Layer(std::wstring_view svLayerTag) { return S_OK; }
	HRESULT			Ready_UI_Layer(std::wstring_view svLayerTag)			{ return S_OK; }

private:
	HRESULT			Ready_Prototype();

private:
	CLoading* m_pLoading;

public:
	static CLogo* Create();

private:
	virtual void	Free();

};

