#include "CAttackReadyArm.h"
#include "CManagement.h"
#include "CGraphicDev.h"
#include "CDiveDave.h"
#include "CHelper.h"
CAttackReadyArm::CAttackReadyArm()
{
}

CAttackReadyArm::CAttackReadyArm(const CAttackReadyArm& rhs)
	: CGameObject(rhs)
{
}

CAttackReadyArm::~CAttackReadyArm()
{
}

HRESULT CAttackReadyArm::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	_vec3 vScale = { 0.2f, 0.2f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	_float fWidth = 39.f;
	_float fHeight = 14.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	return S_OK;
}

_int CAttackReadyArm::Update_GameObject(const _float& fTimeDelta)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	_vec3 vParentPos;
	m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);
	_vec3 vOffSet = { 0.f, 0.4f, 0.f };
	vOffSet.y *= m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->m_vScale.y;
	vParentPos += vOffSet;
	m_pTransformCom->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	Rotate_ToMouse();
	return iExit;
}

void CAttackReadyArm::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CAttackReadyArm::Render_GameObject()
{
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
		return;

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CAttackReadyArm::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CAttackReadyArmTex, ID_STATIC>(L"Proto_AttackReadyArmBuffer", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 텍스쳐
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_AttackReadyArmTexture", L"Com_Texture", &m_pTextureCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CAttackReadyArm::Rotate_ToMouse()
{
	_vec3 vRayPos, vRayDir;
	CHelper::GetMousePointRay(&vRayPos, &vRayDir);

	if (vRayDir.x <= 0.f)
	{
		_vec3 vRotateDir = { 0.f, -180.f, 0.f };
		m_pTransformCom->m_vAngle.x = vRotateDir.x;
		m_pTransformCom->m_vAngle.y = vRotateDir.y;
		m_pTransformCom->m_vAngle.z = vRotateDir.z;
	}
	else
	{
		_vec3 vRotateDir = { 0.f, 0.f, 0.f };
		m_pTransformCom->m_vAngle.x = vRotateDir.x;
		m_pTransformCom->m_vAngle.y = vRotateDir.y;
		m_pTransformCom->m_vAngle.z = vRotateDir.z;
	}

	//m_pTransformCom->Rotation(ROT_Z, 30.f);
}

CAttackReadyArm* CAttackReadyArm::Create()
{
	CAttackReadyArm* pArm = new CAttackReadyArm;

	if (FAILED(pArm->Ready_GameObject()))
	{
		Safe_Release(pArm);
		MSG_BOX("AttackReadyArm Create Failed");
		return nullptr;
	}
	return pArm;
}

void CAttackReadyArm::Free()
{
	CGameObject::Free();
}
