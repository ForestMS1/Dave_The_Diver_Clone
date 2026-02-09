#include "pch.h"
#include "CTestCube.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CLog.h"
#include "CBoundingSphere.h"
#include "CBoundingFrustum.h"
#include "CHelper.h"
#include "CDInputMgr.h"

CTestCube::CTestCube(COLLIDERID eColl)
	: CGameObject()
	, m_fTimer(0.f)
	, m_iMoveDir(0)
	, m_fSpeed(0.f)
	, m_eColl(eColl)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pCollider(nullptr)
{
}

CTestCube::~CTestCube()
{
}

HRESULT CTestCube::Ready_GameObject(_vec3 const * vPos)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };

	if (m_eColl == COLL_AABB)
	{
		m_pCollider = CAABB::Create(vPos, &vExtents, L"SomeTag", this);
	}
	else if (m_eColl == COLL_OBB)
	{
		_vec3 vOri = { 0.f, 0.f, 0.f };
		m_pCollider = COBB::Create(vPos, &vExtents, &vOri, L"SomeTag", this);
	}
	else if (m_eColl == COLL_SPHERE)
	{
		// 111사각형 을감싸는 가장긴 반지름은 >> root 1 + 1 + 1 
		float fRadius = sqrtf(3.f);
		m_pCollider = CBoundingSphere::Create(vPos, fRadius, L"SomeTag", this);
	}
	else if (m_eColl == COLL_FRUSTUM)
	{
		_matrix matProj;
		D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.f), 1.f, 0.1f, 5.f);
		m_pCollider = CBoundingFrustum::Create(&matProj, L"SomeTag", this);
		m_pCollider->Set_OriginalColor(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	}
	
	// 트랜스폼 초기화
	m_pTransformCom->Set_Pos(vPos->x, vPos->y, vPos->z);
	_vec3 vScael = { 1.f, 1.f, 1.f };
	m_pTransformCom->Set_Scale(&vScael);
	return S_OK;
}

_int CTestCube::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);

	// 충돌체 그룹에 넣어줘야한다.
	CColliderMgr::GetInstance()->AddColliderGroup(L"Test", m_pCollider);

	// 충돌체 위치 업데이트
	// 월드 행렬로 하거나, scale, rotation, translation을 주거나
	bool bUseWorldMat = true;
	if (bUseWorldMat)
	{
		m_pCollider->Transform(m_pTransformCom->Get_World());
	}
	else
	{
		_vec3 vRadians;
		m_pTransformCom->Get_RotationRadian(&vRadians);
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pCollider->Transform(1.f, &vRadians, &vPos);
	}

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	m_pTransformCom->Move_Pos(&vLook, m_fSpeed, fTimeDelta);

	m_fTimer += fTimeDelta;
	if (m_fTimer > 1.f)
	{
		++m_iMoveDir %= 4;
	}

	if (m_iMoveDir == 0)
	{
		m_pTransformCom->Rotation(ROT_Y, 1.f);
	}
	else if (m_iMoveDir == 1)
	{
		m_pTransformCom->Rotation(ROT_Y, 1.f);
	}
	else if (m_iMoveDir == 2)
	{
		m_pTransformCom->Rotation(ROT_Y, 1.f);
	}
	else if (m_iMoveDir == 3)
	{
		m_pTransformCom->Rotation(ROT_Y, 1.f);
	}
	m_pTransformCom->Rotation(ROT_X, 3.f);
	m_pTransformCom->Rotation(ROT_Z, 1.f);

	return iExit;
}

void CTestCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	// Test 레이어에있는 충돌체 리스트를 들고온다. 널체크
	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Test"))
	{
		// 충돌체 순회
		for (auto& pCollider : *pColliders)
		{
			// 내가 아닌것들과 체크
			if (m_pCollider != pCollider)
			{
				// 충돌체 끼리 충돌 체크
				if (m_pCollider->Intersect(pCollider))
				{
					// Some Logic
				}
			}
			
			// 레이와 충돌 체크
			_vec3 vRayPos, vRayDir;
			CHelper::GetMousePointRay(&vRayPos, &vRayDir);
			float fDist{};
			if (m_pCollider->Intersect(&vRayPos, &vRayDir, fDist))
			{
				// Some Logic
				if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
				{
					// 만약 충돌한다면 태그비교후 보이드포인터 들고와서 캐스팅한다음 조작한다.
					if (m_pCollider->Get_Tag() == L"SomeTag")
					{
						reinterpret_cast<CGameObject*>(m_pCollider->Get_VoidPtr())->Set_Dead();
					}
				}
			}
		}
	}
}

void CTestCube::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTestCube::Add_Component()
{
	// buffer
	if (FAILED((AddComponent<CCubeCol, ID_STATIC>(L"Proto_CubeCol", L"Com_Buffer", &m_pBufferCom))))
	{
		return E_FAIL;
	}

	// transform
	if (FAILED((AddComponent<CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
	{
		return E_FAIL;
	}

	return S_OK;
}


CTestCube* CTestCube::Create(_vec3 const* vPos, COLLIDERID eColl)
{
	CTestCube* pCube = new CTestCube{ eColl };

	if (FAILED(pCube->Ready_GameObject(vPos)))
	{
		Safe_Release(pCube);
		MSG_BOX("pCube Create Failed");
		return nullptr;
	}
	
	return pCube;
}

void CTestCube::Free()
{
	CGameObject::Free();
	Safe_Release(m_pCollider);
}
