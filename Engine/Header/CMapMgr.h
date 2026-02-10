#pragma once
#include "CBase.h"
#include "Engine_Define.h"

class CGameObject;
class CCamera;
class CRoom;

BEGIN(Engine)
class ENGINE_DLL CMapMgr : public CBase
{
	DECLARE_SINGLETON(CMapMgr)

private:
	explicit CMapMgr();
	virtual ~CMapMgr();

public:


	HRESULT Ready_MiniMap(CGameObject* map[15][15], vector <CRoom*>* vecRoom);

	void	Update_Map(const _float& fTimeDelta);
	void	LateUpdate_Map(const _float& fTimeDelta);
	void	Render_Map();

public:
	void	Render_GUI();
	void	Load();
	void	Save();



public:

	void	PickMiniMap();



private:
	string Stage;

	// 주소값만 들고 온거임 // Scene꺼
	CGameObject* m_arrMiniMap[15][15];



	vector <CRoom*>* m_vecRoom;
		
	//		// 개별 동적 Room Tile을 가지고 있는 Room vector // Room
	//		vector<vector<CGameObject*>> vecRoomTile;
	//		// 개별 정적 Object ex_플레이어,몬스터 // Room 
	//		vector <CGameObject*> vecRoomObject;





private:
	virtual void Free();

};

END


