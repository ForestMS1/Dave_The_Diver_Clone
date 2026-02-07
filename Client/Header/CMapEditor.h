#pragma once
#include "CScene.h"


#include "CRoom.h"


class CMapEditor : public CScene
{
public:
	enum STATE { ROOM_PICKING = 0, ROOM_CHOICE = 1, ROOM_CREATE, ROOM_INTO_DELETE, ROOM_CUSTOM};
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
	void			Show_GUI();

	void			PickMiniMap();

	void			MoveCamera(const _vec3& pEye, const _vec3& pAt, const _vec3& pUp);



	HRESULT			CreateRoom();
	

public:
	static CMapEditor* Create();


public:
	vector<vector<CGameObject*>> vec_Map;
	_int						iRoom_Cnt{};
	vector<CGameObject*>*	m_pPickRoom;

	CGameObject* arrMiniMap[15][15];
	STATE	e_MapEditorLevel{ ROOM_CHOICE };
	CGameObject* m_pPickMiniMap{nullptr};




private:
	virtual void	Free();

};

