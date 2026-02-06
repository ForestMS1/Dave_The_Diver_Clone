#pragma once
#include "CScene.h"


#include "CRoom.h"


class CMapEditor : public CScene
{
public:
	enum STATE { ROOM_PICKING = 0, LEVEL1 = 1, LEVEL2, LEVEL3 };
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

	void			PickMiniMap();

	void			CreateRoom();
	

public:
	static CMapEditor* Create();


public:
	CGameObject* arrRoom[15][15];

	CGameObject* arrMiniMap[15][15];

	STATE	e_MapEditorLevel{ LEVEL1 };
	CGameObject* m_pPickMiniMap{nullptr};
	CGameObject* m_pPickDirMiniMap{nullptr};



private:
	virtual void	Free();
};

