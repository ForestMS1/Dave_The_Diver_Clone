#include "CDiveDaveGun.h"
#include "CManagement.h"
#include "CGraphicDev.h"
#include "CDiveDave.h"
#include "CHelper.h"
#include "CDiveDaveBullet.h"
CDiveDaveGun::CDiveDaveGun()
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

	_vec3 vScale = { 0.4f, 0.4f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	_float fWidth = 32.f;
	_float fHeight = 12.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);
	return S_OK;
}

_int CDiveDaveGun::Update_GameObject(const _float& fTimeDelta)
{
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
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
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
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
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
		return;
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_CurEquipped() != EQUIPPED::GUN)
		return;

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDiveDaveGun::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CAttackReadyArmTex, ID_STATIC>(L"Proto_AttackReadyArmBuffer", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 텍스쳐
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_BasicRifleTexture", L"Com_Texture", &m_pTextureCom))))
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

void CDiveDaveGun::Fire()
{
	//총알 발사 후 IDLE
	_vec3 vOrigin, vDir;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vDir);
	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransformCom->Get_Info(INFO_POS, &vOrigin);
	CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->
		Add_GameObject(L"DiveDaveBullet", CDiveDaveBullet::Create(vOrigin, vDir, m_pTransformCom->m_vAngle.z));
	static_cast<CDiveDave*>(m_pParentGameObject)->Set_State(DiveState::IDLE);
}

CDiveDaveGun* CDiveDaveGun::Create()
{
	CDiveDaveGun* pGun = new CDiveDaveGun;

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
