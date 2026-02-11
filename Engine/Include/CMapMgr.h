#pragma once
#include "CBase.h"
#include "Engine_Define.h"


BEGIN(Engine)


class CGameObject;
class CFrustrum;

class ENGINE_DLL CMapMgr : public CBase
{
	DECLARE_SINGLETON(CMapMgr)

private:
	explicit CMapMgr();
	virtual ~CMapMgr();

public:

	void	Show_GUI();

	
	
	void	Update_Map(const _float& fTimeDelta);
	void	LateUpdate_Map(const _float& fTimeDelta);
	void	Render_Map();


	


private:
	unordered_map< GAMEOBJECTID, vector <CGameObject*>> SceneObj;

private:
	virtual void Free();

};

END
