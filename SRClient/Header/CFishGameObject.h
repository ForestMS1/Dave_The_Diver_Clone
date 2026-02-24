#pragma once
#include "CGameObject.h"

//yellowbackFusilier: swim die
//BlueTang : swim die
//Clownfish : die swim sprint
//Smallspotted_dart : die swim sprint
//YellowTang : die swim sprint
class CFishGameObject : public CGameObject
{
public:
	//explicit CFishGameObject();
	//virtual ~CFishGameObject();

public:
	void Update_ImGui() override;

public:
	void Render(function<void()> beforeDrawLambda = nullptr);
protected:
	HRESULT			Ready_Component(std::wstring_view svSpineName);
protected:
	Engine::CDynamicBuffer* m_pDynamicBuffer;
	Engine::CTransform* m_pTransformCom;
	Engine::CSpine* m_pSpineCom;
};

