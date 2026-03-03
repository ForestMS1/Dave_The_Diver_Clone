#include "CDiveDaveGun.h"
#include "CManagement.h"
#include "CGraphicDev.h"
#include "CDiveDave.h"
#include "CHelper.h"
#include "CDiveDaveBullet.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CDiveDaveGun::CDiveDaveGun(CGameMemMgr::CDaveInfo::DAVE_GUN eGun)
	: m_eCurGun(eGun)
{
}

CDiveDaveGun::CDiveDaveGun(const CDiveDaveGun& rhs)
	: CGameObject(rhs)
{
}

CDiveDaveGun::~CDiveDaveGun()
{
}

HRESULT CDiveDaveGun::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	switch (m_eCurGun)
	{
	case CGameMemMgr::CDaveInfo::GUN_DEFAULT:
		m_sTexName = L"Tex_BasicRifle";
		break;
	case CGameMemMgr::CDaveInfo::GUN_TRIPLE_ACCEL:
		m_sTexName = L"TripleAxel";
		break;
		//case CGameMemMgr::CDaveInfo::GUN_PENTA_ACCEL:
		//	break;
	default:
		m_sTexName = L"Tex_BasicRifle";
		break;
	}

	_vec3 vScale = { 0.4f, 0.4f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	CDiveDaveGun::Set_Size();

	return S_OK;
}

void CDiveDaveGun::Init()
{
	if (m_bInitComplete)
		return;
	static_cast<CDiveDave*>(m_pParentGameObject)->Set_WeaponSlot(this, EQUIPPED::GUN);
	m_bInitComplete = true;
}

_int CDiveDaveGun::Update_GameObject(const _float& fTimeDelta)
{
	CDiveDaveGun::Init();
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DIVEDAVESTATE::ATTACK)
		return 0;
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_CurEquipped() != EQUIPPED::GUN)
		return 0;

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	Set_ParentTransform();
	Rotate_ToMouse();
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	return iExit;
}

void CDiveDaveGun::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DIVEDAVESTATE::ATTACK)
		return;
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_CurEquipped() != EQUIPPED::GUN)
		return;
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CDiveDaveGun::Render_GameObject()
{
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DIVEDAVESTATE::ATTACK)
		return;
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_CurEquipped() != EQUIPPED::GUN)
		return;

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sTexName))
	{
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDiveDaveGun::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CAttackReadyArmTex, ID_STATIC>(L"Proto_AttackReadyArmBuffer", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CDiveDaveGun::Set_ParentTransform()
{
	_vec3 vParentPos;
	m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);


	_vec3 vOffSet = { 0.f, 0.5f, 0.f };
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_AttackSubState() == ATTACKSUBSTATE::ATTACK_FIGHT
		|| static_cast<CDiveDave*>(m_pParentGameObject)->Get_AttackSubState() == ATTACKSUBSTATE::ATTACK_FIRE)
	{
		if (m_bIsFlip)
			vOffSet = { 0.2f, 0.4f, 0.f };
		else
			vOffSet = { -0.2f, 0.4f, 0.f };
	}

	vOffSet.y *= m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->m_vScale.y;
	vParentPos += vOffSet;
	m_pTransformCom->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);
}

void CDiveDaveGun::Rotate_ToMouse()
{
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_AttackSubState() == ATTACKSUBSTATE::ATTACK_FIGHT)
		return;

	_vec3 vMousePos, vPlayerPos;

	CHelper::GetMousePointInWorld(&vMousePos);
	m_pTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3 vDir = vMousePos - vPlayerPos;

	float fLengthSq = vDir.x * vDir.x + vDir.y * vDir.y;

	// 너무 가까우면 회전 유지
	if (fLengthSq < 0.001f)
		return;

	float fDegree = D3DXToDegree(atan2f(vDir.y, vDir.x));

	if (vDir.x < 0.f)
	{
		m_pTransformCom->m_vAngle.x = 180.f;
		m_bIsFlip = true;
	}
	else
	{
		m_pTransformCom->m_vAngle.x = 0.f;
		m_bIsFlip = false;
	}

	m_pTransformCom->m_vAngle.z = fDegree;
}

void CDiveDaveGun::Change_Gun(CGameMemMgr::CDaveInfo::DAVE_GUN eGun)
{
	if (m_bInitComplete == false)
		return;

	// 기존 총 사이즈 해제
	Reset_Size();

	// 총 변경
	m_eCurGun = eGun;

	switch (m_eCurGun)
	{
	case CGameMemMgr::CDaveInfo::GUN_DEFAULT:
		m_sTexName = L"Tex_BasicRifle";
		break;
	case CGameMemMgr::CDaveInfo::GUN_TRIPLE_ACCEL:
		m_sTexName = L"TripleAxel";
		break;
		//case CGameMemMgr::CDaveInfo::GUN_PENTA_ACCEL:
		//	break;
	default:
		m_sTexName = L"Tex_BasicRifle";
		break;
	}

	// 새로 바꾼 총으로 사이즈 조정
	Set_Size();

	dynamic_cast<CDiveDave*>(m_pParentGameObject)->Set_WeaponSlot(this, EQUIPPED::GUN);
}

void CDiveDaveGun::Fire()
{
	_vec3 vOrigin, vDir;

	_float baseAngle;
	_float spreadAngle = 15.f;
	_float rad;
	_matrix matLeft, matRight;
	_vec3 vLeftDir, vRightDir;

	auto pLayer = CManagement::GetInstance()
		->Get_Scene()
		->Get_Layer(L"0_GameLogic_Layer");

	switch (m_eCurGun)
	{
	case CGameMemMgr::CDaveInfo::GUN_DEFAULT:
		//총알 발사 후 IDLE
		m_pTransformCom->Get_Info(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Get_Info(INFO_POS, &vOrigin);
		pLayer->Add_GameObject(L"DiveDaveBullet", CDiveDaveBullet::Create(vOrigin, vDir, m_pTransformCom->m_vAngle.z));
		static_cast<CDiveDave*>(m_pParentGameObject)->Set_State(DIVEDAVESTATE::IDLE);
		break;
	case CGameMemMgr::CDaveInfo::GUN_TRIPLE_ACCEL:
		//총알 발사 후 IDLE
		m_pTransformCom->Get_Info(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		m_pTransformCom->Get_Info(INFO_POS, &vOrigin);

		baseAngle = m_pTransformCom->m_vAngle.z;

		spreadAngle = 15.f;

		rad = D3DXToRadian(spreadAngle);

		// 좌/우 회전용
		D3DXMatrixRotationZ(&matLeft, rad);
		D3DXMatrixRotationZ(&matRight, -rad);

		D3DXVec3TransformNormal(&vLeftDir, &vDir, &matLeft);
		D3DXVec3TransformNormal(&vRightDir, &vDir, &matRight);

		D3DXVec3Normalize(&vLeftDir, &vLeftDir);
		D3DXVec3Normalize(&vRightDir, &vRightDir);

		pLayer->Add_GameObject(L"DiveDaveBullet",
			CDiveDaveBullet::Create(vOrigin, vDir, baseAngle));

		pLayer->Add_GameObject(L"DiveDaveBullet",
			CDiveDaveBullet::Create(vOrigin, vLeftDir, baseAngle + spreadAngle));

		pLayer->Add_GameObject(L"DiveDaveBullet",
			CDiveDaveBullet::Create(vOrigin, vRightDir, baseAngle - spreadAngle));

		static_cast<CDiveDave*>(m_pParentGameObject)
			->Set_State(DIVEDAVESTATE::IDLE);
		break;
		//case CGameMemMgr::CDaveInfo::GUN_PENTA_ACCEL:
		//	break;
	default:
		break;
	}
}

CDiveDaveGun* CDiveDaveGun::Create(CGameMemMgr::CDaveInfo::DAVE_GUN eGun)
{
	CDiveDaveGun* pGun = new CDiveDaveGun(eGun);

	if (FAILED(pGun->Ready_GameObject()))
	{
		Safe_Release(pGun);
		MSG_BOX("DiveDaveGun Create Failed");
		return nullptr;
	}
	return pGun;
}

void CDiveDaveGun::Free()
{
	CGameObject::Free();
}


void	CDiveDaveGun::Set_Size()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_sTexName)->at(0))->Get_ImgInfo();
	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);
}
void	CDiveDaveGun::Reset_Size()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_sTexName)->at(0))->Get_ImgInfo();
	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);
}