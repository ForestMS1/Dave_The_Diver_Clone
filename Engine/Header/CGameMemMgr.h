#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)


// 게임진행에 필요한 플래그라던지 전역적으로 접근가능한 데이터의 관리를 매니저
class ENGINE_DLL CGameMemMgr : public CBase
{
	DECLARE_SINGLETON(CGameMemMgr)

private:
	explicit CGameMemMgr();
	virtual ~CGameMemMgr();
public:
	typedef struct tagFish {
		string name;
		int quantity;
		int level;
		int cost;
	}FISH;
public:
	HRESULT Ready();
	void addFish(string name, int quantity);
	vector<FISH*>& getFishes() { return fishes; }

	void levelUp(string name);
private:
	virtual void		Free();
	vector<FISH*> fishes;
};

END