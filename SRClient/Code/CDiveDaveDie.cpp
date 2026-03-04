#include "CDiveDaveDie.h"
#include "CDiveDave.h"

#include "CManagement.h"

#include "CBackToShipUI.h"

CDiveDaveDie::CDiveDaveDie(CDiveDave* pOwner)
	:CBaseState<CDiveDave>(pOwner)
{
}

CDiveDaveDie::~CDiveDaveDie()
{
}

void CDiveDaveDie::Enter()
{
	m_pOwner->Init_Frame();
	_float fWidth = 45.f;
	_float fHeight = 57.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
	m_pOwner->Set_TextureCom(L"Com_DieTexture");

	if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
	{
		if (auto pUI = pLayer->Get_GameObjectFirst(L"BackToShipUI"))
		{
			pUI->Set_DeadCascade();
		}
		else
		{
			auto pBackToShipUI = CBackToShipUI::Create(0.f, 0.f);
			pLayer->Add_GameObject(L"BackToShipUI", pBackToShipUI);
		}
	}
}

void CDiveDaveDie::Input(const _float& fTimeDelta)
{

}

_int CDiveDaveDie::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	_float fFrame = m_pOwner->Get_Frame();
	m_pOwner->AddFrame(fTimeDelta, 10.f, 23);
	return 0;
}

void CDiveDaveDie::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveDie::Render_State()
{
	CTexture* pPlayerTextureCom = m_pOwner->Get_TextureCom();

	_float fFrame = m_pOwner->Get_Frame();

	pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CDiveDaveDie::Exit()
{
	_float fWidth = 45.f;
	_float fHeight = 57.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	Clear();
}

void CDiveDaveDie::Clear()
{
	
}


CDiveDaveDie* CDiveDaveDie::Create(CDiveDave* pOwner)
{
	CDiveDaveDie* pState = new CDiveDaveDie(pOwner);

	return pState;
}

void CDiveDaveDie::Free()
{
}
