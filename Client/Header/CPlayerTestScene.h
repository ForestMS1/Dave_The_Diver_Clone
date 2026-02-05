#pragma once
#include "CScene.h"
class CPlayerTestScene : public CScene
{
protected:
	explicit CPlayerTestScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerTestScene();

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

private:
	HRESULT			Ready_Environment_Layer(std::wstring_view svLayerTag);
	HRESULT			Ready_GameLogic_Layer(std::wstring_view svLayerTag);
	HRESULT			Ready_UI_Layer(std::wstring_view svLayerTag);

	HRESULT			Ready_Light();

public:
	static CPlayerTestScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free();
};

