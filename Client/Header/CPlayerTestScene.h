#pragma once
#include "CScene.h"
class CPlayerTestScene : public CScene
{
protected:
	explicit CPlayerTestScene();
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
	static CPlayerTestScene* Create();

private:
	virtual void	Free();
};

