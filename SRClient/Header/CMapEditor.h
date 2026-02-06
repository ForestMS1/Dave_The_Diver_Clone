#pragma once
#include "CScene.h"


#include "CRoom.h"


class CMapEditor : public CScene
{
protected:
	explicit CMapEditor();
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

private:
	void			Show_Position();
	

public:
	static CMapEditor* Create();


public:
	list <CRoom*> vecRoom[15];
	list <CGameObject*> vecMiniMap[15];

private:
	virtual void	Free();
};

