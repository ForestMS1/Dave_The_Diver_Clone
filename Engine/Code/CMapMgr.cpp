#include "CMapMgr.h"

IMPLEMENT_SINGLETON(CMapMgr)



CMapMgr::CMapMgr()
{
}

CMapMgr::~CMapMgr()
{
	Free();
}



HRESULT CMapMgr::Ready_MiniMap(CGameObject* map[15][15], vector<CRoom*>* vecRoom )
{
	// Tile 정보 바꾸기 위해 주소값만 받아오는 거
	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			m_arrMiniMap[i][j] = map[i][j];
	
		}
	}

	//Room의 상태 바꾸기 위해 들고오기
	m_vecRoom = vecRoom;




	return S_OK;
}

void	CMapMgr::Update_Map(const _float& fTimeDelta) {

}
void	CMapMgr::LateUpdate_Map(const _float& fTimeDelta) {

}
void	CMapMgr::Render_Map() {

}



void CMapMgr::Render_GUI() {

}

void CMapMgr::Load()
{

}

void CMapMgr::Save()
{
}

void CMapMgr::Free()
{
	
}