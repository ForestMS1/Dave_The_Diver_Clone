#pragma once
#include "CBase.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CComponent.h"
#include "Engine_Define.h"


BEGIN(Engine)


class CGameObject;
class CLayer;

class ENGINE_DLL CMapMgr : public CBase
{
	DECLARE_SINGLETON(CMapMgr)

private:
	explicit CMapMgr();
	virtual ~CMapMgr();



public:

	void    SetScene(CScene* Scene) { m_Scene = Scene;}
	void	Load();
	void	Save();

public:

	void	Show_GUI();
	void	Object_Show();
	void	Frame_Show();
	void	SaveLoad_Show();
public:

	void	Update_Map(const _float& fTimeDelta);
	void	LateUpdate_Map(const _float& fTimeDelta);
	void	Render_Map();


	


protected:
	CScene* m_Scene; 
	
private:
	virtual void Free();

	wstring StringToWString(const std::string& str);

	string wstringToString(const std::wstring& wstr);


};

END
