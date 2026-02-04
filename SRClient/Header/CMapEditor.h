#pragma once
#include "CScene.h"


#include "CRoom.h"


class CMapEditor : public CScene
{
protected:
	explicit CMapEditor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapEditor();

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
	static CMapEditor* Create(LPDIRECT3DDEVICE9 pGraphicDev);


public:
	vector <CRoom*> vecRoom;

private:
	virtual void	Free();
};

