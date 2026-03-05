#include "CJohnNoStart.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CManagement.h"
#include "CSoundMgr.h"
#include "CJohnConversation.h"
#include "CDaveConversation.h"
CJohnNoStart::CJohnNoStart(CJohn* pOwner)
	: CBaseState<CJohn>(pOwner)
{
}

CJohnNoStart::~CJohnNoStart()
{
}

void CJohnNoStart::Enter()
{
	m_pOwner->Init_Frame();

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnAngry")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohnNoStart::Input(const _float& fTimeDelta)
{
}

_int CJohnNoStart::Update_State(const _float& fTimeDelta)
{
	m_pOwner->Update_ToTargetDir();

	if (m_pOwner->Check_TargetInRange(10.f))
	{
		if (!m_bSoundBgm)
		{
			m_bSoundBgm = true;
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
			CSoundMgr::GetInstance()->PlaySoundLoop(L"Sound_BGM_Boss_01", CSoundMgr::BOSS_BGM, 1.f);
		}
		_vec3 vRotDir;
		if (m_pOwner->Get_ToTargetDir().x > 0.f)
			vRotDir = { 0.f, 0.f, 0.f };
		else
			vRotDir = { 0.f,-180.f, 0.f };
		m_pOwner->Set_RotateDir(&vRotDir);

		m_pOwner->AddFrame(fTimeDelta, 5.f, 8, false);
		CDiveDaveCam* pCam = static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_CurCamera());
		_vec3 vCurPos;
		m_pOwner->Get_Pos(&vCurPos);
		pCam->Set_Target(&m_pOwner->GetTransformCom()->m_vInfo[INFO_POS]);
		m_pOwner->Update_ToTargetDir();

		if (m_pOwner->Check_TargetInRange(10.f))
		{
			if (!m_bSoundBgm)
			{
				m_bSoundBgm = true;
				CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
				CSoundMgr::GetInstance()->PlaySoundLoop(L"Sound_BGM_Boss_01", CSoundMgr::BOSS_BGM, 1.f);
			}
			_vec3 vRotDir;
			if (m_pOwner->Get_ToTargetDir().x > 0.f)
				vRotDir = { 0.f, 0.f, 0.f };
			else
				vRotDir = { 0.f,-180.f, 0.f };
			m_pOwner->Set_RotateDir(&vRotDir);


			m_pOwner->AddFrame(fTimeDelta, 5.f, 8, false);
			CDiveDaveCam* pCam = static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_CurCamera());
			_vec3 vCurPos;
			m_pOwner->Get_Pos(&vCurPos);
			pCam->Set_Target(&m_pOwner->GetTransformCom()->m_vInfo[INFO_POS]);

			if (!m_bJohnDialogCreated) {
				if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer"))
				{
					if (auto pObj = pLayer->Get_GameObjectFirst(L"JohnConversation"))
					{
						pObj->Set_DeadCascade();
					}
					else
					{
						CJohnConversation* JohnConversation = CJohnConversation::Create(m_pOwner->GetTransformCom()->m_vInfo[INFO_POS].x, m_pOwner->GetTransformCom()->m_vInfo[INFO_POS].y - 2.f);
						JohnConversation->SetCurrentConversation(CJohnConversation::CONVERSATION::CONV_1);
						pLayer->Add_GameObject(L"JohnConversation", JohnConversation);
						m_bJohnDialogCreated = true;
					}
				}
			}
			
			if (m_bJohnDialogCreated ) {
				if (!m_bDaveDialogCreated) {
					if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer"))
					{
						if (auto pObj = pLayer->Get_GameObjectFirst(L"JohnConversation"))
						{
						}
						else {
							if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer"))
							{
								if (auto pObj = pLayer->Get_GameObjectFirst(L"DaveConversation"))
								{
									pObj->Set_DeadCascade();
								}
								else
								{
									CDaveConversation* DaveConversation = CDaveConversation::Create(m_pOwner->GetTransformCom()->m_vInfo[INFO_POS].x, m_pOwner->GetTransformCom()->m_vInfo[INFO_POS].y - 2.f);
									DaveConversation->SetCurrentConversation(CDaveConversation::CONVERSATION::BOSS_1);
									pLayer->Add_GameObject(L"DaveConversation", DaveConversation);
									m_bDaveDialogCreated = true;
								}
							}
						}
					}
				}
				
			}
			if (m_bDaveDialogCreated) {
				if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer"))
				{
					if (auto pObj = pLayer->Get_GameObjectFirst(L"DaveConversation"))
					{
					}
					else {
						if (m_pOwner->Get_Frame() == 7)
						{
							m_pOwner->EncounterTarget();
							m_pOwner->Set_State(JOHNSTATE::IDLE);
						}
					}
				}
			}
		}
	}


	

	return 0;
}

void CJohnNoStart::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohnNoStart::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnAngry"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohnNoStart::Exit()
{
	CDiveDaveCam* pCam = static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_CurCamera());
	CTransform* pDiveTransform = static_cast<CTransform*>
		(CManagement::GetInstance()->Get_FirstObjectComponent(ID_DYNAMIC, L"0_GameLogic_Layer", L"DiveDave", L"Com_Transform"));
	pCam->Set_Target(&pDiveTransform->m_vInfo[INFO_POS]);

	Clear();
}

void CJohnNoStart::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnAngry")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

CJohnNoStart* CJohnNoStart::Create(CJohn* pOwner)
{
	CJohnNoStart* pState = new CJohnNoStart(pOwner);

	return pState;
}

void CJohnNoStart::Free()
{
}
