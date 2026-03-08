#include "CSushi.h"
#include "CAssetMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CTransition.h"
#include "CGraphicDev.h"
#include "CBackground.h"
#include "CSakura.h"
#include "CSushiCamera.h"
#include "CRoof.h"
#include "CFence.h"
#include "CWood.h"
#include "CHood.h"
#include "CTable.h"
#include "CMenu.h"
#include "CFishTank.h"
#include "CPatternFrame.h"
#include "CSign.h"
#include "CSushiLight.h"
#include "CChair.h"
#include "COpen.h"
#include "CKitchen.h"
#include "CSpeaker.h"
#include "CBancho.h"
#include "CSushiDave.h"
#include "CCustomer1.h"
#include "CColliderMgr.h"
#include "CMenuFrame.h"
#include "CDInputMgr.h"
#include "CFishConfirmFrame.h"
#include "COverlay.h"
#include "CWasabiObject.h"
#include "COpenShop.h"
#include "CGameMemMgr.h"
#include "CShipUIMoney.h"
#include "CTea.h"
#include "CSmallMenu.h"
#include "CSoundMgr.h"
#include "CParticleMgr.h"

CGameObject* g_pObject = nullptr;

CSushi::CSushi()
	: CScene()
{
	customerSpawn = 0.f;
	maxCustomer = 9;
	sushiOpen = false;
}
CSushi::~CSushi()
{
}

HRESULT CSushi::Ready_Scene()
{

	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;

	if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
		return E_FAIL;

	if (FAILED(Ready_UI_Layer(L"UI_Layer")))
		return E_FAIL;

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	//pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);      
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);       
	//pGraphicDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);   
	//CColliderMgr::GetInstance()->Set_Render(true);

	CAssetMgr::GetInstance()->AddAsset(L"Font_DefaultXX", CAssetDefaultFont::Create(L"바탕", 0, 16, FW_BOLD));
	CAssetMgr::GetInstance()->AddAsset(L"Font_Level", CAssetDefaultFont::Create(L"Arial", 5, 16, FW_BOLD));
	CSoundMgr::GetInstance()->PlaySoundLoop(L"Sound_BGM", CSoundMgr::BGM, 0.5f);
	m_fGameEnd = 0.f;
	return S_OK;

}

_int CSushi::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);
	Key_Input();
	if (CustomerLeave == 9) {
		sushiOpen = false;
	}
	if (sushiOpen) {
		customerSpawn += fTimeDelta;
		list<CGameObject*>* Chairs = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjects(L"Chair");
		list<CGameObject*>::iterator iter = Chairs->begin();
		for (iter; iter != Chairs->end(); iter++) {
			if (static_cast<CChair*>(*iter)->isEmtpy()) {
				if (maxCustomer > 0) {
					if (customerSpawn > 4.f) {
						CGameObject* pGameObject = CCustomer1::Create();
						CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Customer", pGameObject);
						maxCustomer--;
						customerSpawn = 0.f;
					}
				}
				return iExit;
			}
		}
	}
	if (CustomerLeave == 9 && !gameEnd) {
		if (m_fGameEnd == 0.f) {
			CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_CloseShop", CSoundMgr::SFX, 1.0f);
		}
		m_fGameEnd += fTimeDelta;
		if (m_fGameEnd > 2.f) {
			CTransition::FadedTransition(CTransition::SCENE_SUSHI, CTransition::SCENE_ENDING);
			gameEnd = true;
		}
		
	}
	return iExit;
}

void CSushi::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CSushi::Render_Scene()
{
#ifdef _DEBUG
	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(L"Here is CSushi", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	ImGui::Begin("Curr Scene: CSushi");
	if (ImGui::Button("Go Ship Scene"))
	{
		//CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SUSHI, CTransition::SCENE_SHIP));
		CTransition::FadedTransition(CTransition::SCENE_SUSHI, CTransition::SCENE_SHIP);
	}
	ImGui::End();

	if (ImGui::Button("Ending"))
	{
		CTransition::FadedTransition(CTransition::SCENE_SUSHI, CTransition::SCENE_ENDING);
	}
#endif

}

CSushi* CSushi::Create()
{
	CSushi* pSushi = new CSushi;

	if (FAILED(pSushi->Ready_Scene()))
	{
		MSG_BOX("CSushi Create Failed");
		Safe_Release(pSushi);
		return nullptr;
	}
	return pSushi;
}

HRESULT CSushi::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;
	
	pGameObject = CBackground::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Background", pGameObject)))
		return E_FAIL;

	pGameObject = CSakura::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Sakura", pGameObject)))
		return E_FAIL;


	pGameObject = CRoof::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Roof", pGameObject)))
		return E_FAIL;

	pGameObject = CFence::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Fence", pGameObject)))
		return E_FAIL;


	pGameObject = CHood::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Hood", pGameObject)))
		return E_FAIL;

	pGameObject = CTable::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Table", pGameObject)))
		return E_FAIL;


	pGameObject = CWood::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Wood", pGameObject)))
		return E_FAIL;

	pGameObject = CMenu::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Menu", pGameObject)))
		return E_FAIL;

	pGameObject = CFishTank::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"FishTank", pGameObject)))
		return E_FAIL;

	pGameObject = CPatternFrame::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Frame", pGameObject)))
		return E_FAIL;
	
	pGameObject = CSign::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Sign", pGameObject)))
		return E_FAIL;

	pGameObject = COpen::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Open", pGameObject)))
		return E_FAIL;
	pGameObject = CKitchen::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Kitchen", pGameObject)))
		return E_FAIL;

	pGameObject = CBancho::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Bancho", pGameObject)))
		return E_FAIL;
	for (int i = 0; i < 4; i++) {
		pGameObject = CSushiLight::Create();
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pTransform->m_vScale = { 0.4f,1.3f,1.f };
		pTransform->m_vInfo[INFO_POS] = {-4.8f + (3.f*i),3.f,-3.f};
		if (nullptr == pGameObject)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(L"Light", pGameObject)))
			return E_FAIL;
	}
	
	for (int i = 0; i < 6; i++) {
		pGameObject = CChair::Create();
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pTransform->m_vScale = { 0.3f,0.3f,1.f };
		pTransform->m_vInfo[INFO_POS] = { -3.4f + (1.5f * i),-2.4f,-3.f };
		if (nullptr == pGameObject)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(L"Chair", pGameObject)))
			return E_FAIL;
	}


	for (int i = 0; i < 2; i++) {
		pGameObject = CSpeaker::Create();
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pTransform->m_vScale = { 0.4f,0.5f,1.f };
		pTransform->m_vInfo[INFO_POS] = { -2.5f + (6.7f * i),0.8f,-2.8f };
		if (nullptr == pGameObject)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(L"Speaker", pGameObject)))
			return E_FAIL;
	}
	pGameObject = CWasabiObject::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"WasabiObject", pGameObject)))
		return E_FAIL;
	static_cast<CWasabiObject*>(pGameObject)->created = true;

	pGameObject = CTea::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Tea", pGameObject)))
		return E_FAIL;
	pGameObject = COverlay::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Overlay", pGameObject)))
		return E_FAIL;

	pGameObject = COpenShop::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"OpenShop", pGameObject)))
		return E_FAIL;

	pGameObject = CShipUIMoney::Create(-7.5f, 4.5f);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"CShipUIMoney", pGameObject)))
		return E_FAIL;

	//pGameObject = CSmallMenu::Create();

	//if (nullptr == pGameObject)
	//	return E_FAIL;

	//if (FAILED(pLayer->Add_GameObject(L"SmallMenu", pGameObject)))
	//	return E_FAIL;

	//static_cast<CSmallMenu*>(pGameObject)->Set_SushiTex(L"")
	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;

}

HRESULT CSushi::Ready_GameLogic_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;


	pGameObject = CSushiDave::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Dave", pGameObject)))
		return E_FAIL;


	


	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });
	return S_OK;
}

HRESULT CSushi::Ready_UI_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	_vec3 m_vEye{ 0.f, 0.f, -10.f };   
	_vec3 m_vAt{ 0.f, 0.f, 0.f };      
	_vec3 m_vUp{ 0.f, 1.f, 0.f };      

	pGameObject = CSushiCamera::Create(&m_vEye, &m_vAt, &m_vUp);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Camera", pGameObject)))
		return E_FAIL;

	pGameObject = CMenuFrame::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"MenuFrame", pGameObject)))
		return E_FAIL;

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });
	return S_OK;
}

void CSushi::Key_Input()
{
	if (CDInputMgr::GetInstance()->Key_Up(DIKEYBOARD_I))
	{
		list<CGameObject*>* button = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"MenuFrame");
		list<CGameObject*>::iterator iter = button->begin();
		for (iter; iter != button->end(); iter++) {
			static_cast<CMenuFrame*>(*iter)->Show();
		}
		CGameObject* button2 = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Close_3");
		button2->Set_Render(true);
		//CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_OpenButton", CSoundMgr::SFX, 1.0f);

	}

	/*if (CDInputMgr::GetInstance()->Key_Up(DIKEYBOARD_O))
	{
		list<CGameObject*>* button = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"MenuFrame");
		list<CGameObject*>::iterator iter = button->begin();
		for (iter; iter != button->end(); iter++) {
			static_cast<CMenuFrame*>(*iter)->Hide();
		}
	}*/
}

void CSushi::Free()
{
	CSoundMgr::GetInstance()->StopAll();
	CParticleMgr::GetInstance()->Clear_Particle();
	CScene::Free();
	
}
