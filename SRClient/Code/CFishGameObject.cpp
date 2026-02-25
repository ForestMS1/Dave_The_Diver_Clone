#include "CFishGameObject.h"

#include "CAssetMgr.h"
#include "CAssetSpine.h"
#include "CFishStopState.h"
#include "CFishSwimState.h"
#include "CFishManualState.h"
#include "CHelper.h"
#include "CDInputMgr.h"

CFishGameObject::CFishGameObject()
    : m_sFishName({})
    , m_fSpeed(0.f)
    , m_fRotateSpeed(D3DXToRadian(180.f))
    , m_fRotateAngleLimit(45.f)
    , m_fsm(Fish::FS_STOP)
    , m_bManual(true)
    , m_eFishState(Fish::FS_SWIM)
    , m_eFishType(Fish::FT_NORMAL)
    , m_vChaseTarget({0.f, 0.f, 0.f})
    , m_fChaseTargetReLocateTimer(0.f)
{

}

void CFishGameObject::Update_ImGui()
{
    CGameObject::Update_ImGui();
   
    ImGui::Text("CurrState: %s", CHelper::WStringToString(Fish::Get_FishStateStr(m_eFishState)).c_str());

    if (ImGui::Button("SWIM"))
    {
        //m_fsm.RequestChangeState(Fish::FS_SWIM);
        m_eFishState = Fish::FS_STOP;
    }

    ImGui::Text("IsManual: %s\n", m_bManual ? "true" : "false");
    if (ImGui::Button("ManualToggle"))
    {
        m_bManual = !m_bManual;
    }
}

_int CFishGameObject::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (m_bManual)
    {
        if (CDInputMgr::GetInstance()->Key_Pressing(DIK_M))
        {
            CHelper::GetMousePointInWorld(&m_vChaseTarget);
        }
    }

    m_fChaseTargetReLocateTimer += fTimeDelta;

    if (m_fChaseTargetReLocateTimer > 2.f)
    {
        //m_vMoveTarget();

        float randX = rand() % 100;
        float randY = rand() % 100;
        randX /= 10;
        randY /= 10;
        _vec3 vNewMoveTarget = { randX , randY , 0.f };
        m_vChaseTarget = vNewMoveTarget;

        m_fChaseTargetReLocateTimer = 0.f;
    }


    if (m_eFishState == Fish::FS_STOP)
    {

    }
    else if (m_eFishState == Fish::FS_SWIM)
    {
        // Right가 물고기 대가리 방향
        _vec3 vRight;
        m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
        D3DXVec3Normalize(&vRight, &vRight);

        // 현재 물고기 위치
        _vec3 vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        _vec3 vChaseMag = m_vChaseTarget - vPos;
        float vChaseLength = D3DXVec3Length(&vChaseMag);

        // 쫓을 방향
        _vec3 vChaseDir;
        D3DXVec3Normalize(&vChaseDir, &vChaseMag);

        // 쫒을거리가 0.01 이상이어야 쫒느다.
        if (vChaseLength > 0.01f)
        {
            // 물고기 대가리가 쫒는방향과 다르다면 이미지 Y 뒤집기
            // 뒤집으면서 Z각도도 반전시켜주기
            if (vRight.x * vChaseDir.x < 0)
            {
                _vec3 vScale;
                m_pTransformCom->Get_Scale(&vScale);
                vScale.x *= -1.f;
                m_pTransformCom->Set_Scale(&vScale);

                _vec3 vAngle;
                m_pTransformCom->Get_Rotation(&vAngle);
                vAngle.z *= -1.f;
                m_pTransformCom->Set_Rotation(&vAngle);
            }

            // 물고기 움직이는 로직 근데 체이스타겟보다 멀리가면 안되니까 분기처리
            float fMoveDistance = m_fSpeed * fTimeDelta;  // 이번에 갈거리 
            if (vChaseLength <= fMoveDistance)
            {
                m_pTransformCom->Set_Pos(m_vChaseTarget.x, m_vChaseTarget.y, m_vChaseTarget.z);
            }
            else
            {
                m_pTransformCom->Move_Pos(&vChaseDir, m_fSpeed, fTimeDelta);
            }

            // 물고기 대가리 돌리는로직
            // fAngleLimit 각도 이상 안돌아가게 처리
            _vec3 vCurrAngle;
            m_pTransformCom->Get_Rotation(&vCurrAngle);
            if (fabsf(vCurrAngle.z) < m_fRotateAngleLimit)
            {
                float fRadian = acosf(D3DXVec3Dot(&vRight, &vChaseDir));

                // 돌아야하는 라디안이 0.01이상만 돌린다.
                if (fRadian > 0.01f)
                {
                    float fRotationAmount = m_fRotateSpeed * fTimeDelta; // 이번에 돌릴 각도

                    // 만약 fRotationAmount가 돌아야될 각도를 넘겨버리면 세팅
                    if (fRotationAmount > fRadian)
                    {
                        fRotationAmount = fRadian;
                    }

                    // 대가리 돌릴 방향 정하는 외적
                    _vec3 vAxis;
                    D3DXVec3Cross(&vAxis, &vRight, &vChaseDir);
                    float fDirSign = (vAxis.z >= 0.f) ? 1.f : -1.f;
                    m_pTransformCom->Rotation(ROT_Z, D3DXToDegree(fRotationAmount * fDirSign));
                }
            }
            // 원래 위에 if를 타야하는데
            // 못타는경우임, 그경우엔 다시 if탈수있도록 세팅
            else
            {
                if (vCurrAngle.z < 0)
                {
                    vCurrAngle.z = -(m_fRotateAngleLimit - 0.5f);
                }
                else
                {
                    vCurrAngle.z = (m_fRotateAngleLimit - 0.5f);
                }
                m_pTransformCom->Set_Rotation(&vCurrAngle);
            }
        }
    }

    //if (m_fsm.IsRequestedChange())
    //{
    //    m_fsm.ChangeState();
    //}
    //m_fsm.Get_CurrentState()->Update(fTimeDelta);
    return iExit;
}

void CFishGameObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
    //m_fsm.Get_CurrentState()->LateUpdate(fTimeDelta);
}

void CFishGameObject::Render(function<void()> beforeDrawLambda)
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (beforeDrawLambda)
    {
        beforeDrawLambda();
    }
    m_pSpineCom->Render(m_pDynamicBuffer);

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFishGameObject::Ready(std::wstring_view svSpineName)
{
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    // 버퍼
    if (FAILED((AddComponent<Engine::CDynamicBuffer, ID_STATIC>(L"Proto_DynamicBuffer_Spine", L"Com_DynamicBuffer", &m_pDynamicBuffer))))
        return E_FAIL;

    // 스파인
    if (FAILED((AddComponent<Engine::CSpine, ID_DYNAMIC>(L"Proto_Spine", L"Com_Spine", &m_pSpineCom))))
        return E_FAIL;

    if (auto pAssSpine = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetSpine>(svSpineName))
    {
        m_pDynamicBuffer->Set_VertexCnt(pAssSpine->Get_NumVertex());
        m_pDynamicBuffer->Set_TriCnt(pAssSpine->Get_NumTri());
        m_pDynamicBuffer->Ready_Buffer();

        m_pSpineCom->Ready_AfterCreate(pAssSpine->Get_SkeletonData(), pAssSpine->Get_AnimationStateData(), svSpineName);
        m_pSpineCom->Index_Buffer_Lock(m_pDynamicBuffer->Get_IndexBuffer());
    };


    m_fsm.AddState(Fish::FS_STOP, make_unique<Fish::CFishStopState>(this));
    m_fsm.AddState(Fish::FS_SWIM, make_unique<Fish::CFishSwimState>(this));
    m_fsm.AddState(Fish::FS_MANUAL, make_unique<Fish::CFishManualState>(this));


    m_fsm.Get_CurrentState()->Enter();
	return S_OK;
}

void CFishGameObject::Free()
{
    CGameObject::Free();
}
