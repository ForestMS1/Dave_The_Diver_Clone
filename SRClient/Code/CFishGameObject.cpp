#include "CFishGameObject.h"

#include "CAssetMgr.h"
#include "CAssetSpine.h"
#include "CFishStopState.h"
#include "CFishSwimState.h"
#include "CFishManualState.h"
#include "CHelper.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include "CGetItemUI.h"
#include "CGameMemMgr.h"
#include "CDiveDave.h"

CFishGameObject::CFishGameObject()
    : m_sFishName({})
    , m_fCurrSpeed(0.f)
    , m_fSpeed(1.f)
    , m_fSprintSpeed(2.f)
    , m_fCurrRotateSpeed(D3DXToRadian(180.f))
    , m_fRotateSpeed(D3DXToRadian(180.f ))
    , m_fSprintRotateSpeed(D3DXToRadian(180.f * 1.5f))
    , m_fRotateAngleLimit(50.f)
    //, m_fsm(Fish::FS_STOP)
    , m_bManual(true)
    , m_eFishState(Fish::FS_SWIM)
    , m_eFishType(Fish::FT_NORMAL)
    , m_vMoveTarget({0.f, 0.f, 0.f})
    , m_fMoveTargetReLocateTimer(0.f)
    , m_bDamaged(false)
    , m_fDamagedTimer(0.f)
    , m_fDieTimer(0.f)
    , m_iHP(3)
    , m_fAttackToTimer(0.f)
    , m_sAttackSpineAniName(L"swim")
    , m_fRunFromTimer(0.f)
    , m_sRunFromSpineAniName(L"swim")
    , m_bIntersectHitboxDave(false)
    , m_bIntersectDetetboxDave(false)
    , m_bDieAndAcquire(false)
    , m_bMoveToRotateEnable(true)
{

}

void CFishGameObject::Update_ImGui()
{
    CGameObject::Update_ImGui();
   
    ImGui::Text("CurrState: %s", CHelper::WStringToString(Fish::Get_FishStateStr(m_eFishState)).c_str());

    ImGui::Text("IsManual: %s\n", m_bManual ? "true" : "false");
    if (ImGui::Button("ManualToggle"))
    {
        m_bManual = !m_bManual;
    }

    if (ImGui::Button("Damaged"))
    {
        Damaged(1);
    }

    if (ImGui::Button("Die"))
    {
        Die();
    }

    ImGui::Text("m_bIntersectHitboxDave: %s\n", m_bIntersectHitboxDave ? "true" : "false");
    ImGui::Text("m_bIntersectDetetboxDave: %s\n", m_bIntersectDetetboxDave ? "true" : "false");

    if (m_eFishType == Fish::FT_NORMAL)
    {
        ImGui::Text("m_eFishType: %s\n","FT_NORMAL");
    }
    else if(m_eFishType == Fish::FT_AGRESSIVE)
    {
        ImGui::Text("m_eFishType: %s\n", "FT_AGRESSIVE");
    }
    
}

void CFishGameObject::Damaged(int iDamage)
{
    if (m_eFishState == Fish::FS_DIE) return;

    m_bDamaged = true;
    m_pSpineCom->Set_ColorWhite(true);
    m_iHP -= iDamage;
    if (m_iHP <= 0)
    {
        Die();
    }
}

void CFishGameObject::Die()
{
    m_eFishState = Fish::FS_DIE;
    m_pSpineCom->Set_AniState(L"die", false);
}

void CFishGameObject::RunFrom(_vec3 const* pDavePos)
{
    if (m_eFishState == Fish::FS_DIE) return;

    if (m_eFishState != Fish::FS_RUNFROM)
    {
        m_pSpineCom->Set_AniState(m_sRunFromSpineAniName);
        m_eFishState = Fish::FS_RUNFROM;
    }

    m_fCurrSpeed = m_fSprintSpeed;
    m_fCurrRotateSpeed = m_fSprintRotateSpeed;

    _vec3 vMyPos;
    m_pTransformCom->Get_Info(INFO_POS, &vMyPos);

    _vec3 vDir = vMyPos - *pDavePos; 
    float fDist = D3DXVec3Length(&vDir);

    if (fDist > 0.0f) 
    {
        D3DXVec3Normalize(&vDir, &vDir);
    }
    else 
    {
        vDir = _vec3(1.f, 0.f, 0.f);
    }

    m_vMoveTarget = vMyPos + (vDir * 10.f);
}

void CFishGameObject::QTE(_vec3 const* pJaksalPos, _vec3 const* pDavePos)
{
    if (m_eFishState == Fish::FS_DIE) return;

    m_eFishState = Fish::FS_QTE;

    _vec3 vMyPos;
    m_pTransformCom->Get_Info(INFO_POS, &vMyPos);

    _vec3 vDir = *pJaksalPos - *pDavePos;
    D3DXVec3Normalize(&vDir, &vDir);

    _vec3 vNewTarget = vMyPos + vDir * 0.5f;
    m_vMoveTarget = vNewTarget;
    m_fCurrSpeed = 5.f;
    m_fCurrRotateSpeed = D3DXToRadian(360.f * 10.f);
}

void CFishGameObject::QTERelease()
{
    Swim();
}

void CFishGameObject::AttackTo(_vec3 const* pDavePos)
{
    if (m_eFishState == Fish::FS_DIE) return;
    if (m_eFishState == Fish::FS_QTE) return;
   
    // State Enter
    if (m_eFishState != Fish::FS_ATTACKTO)
    {
        m_pSpineCom->Set_AniState(m_sAttackSpineAniName);
    }
    m_eFishState = Fish::FS_ATTACKTO;


    //m_fAttackToTimer = 0.f;
    m_fCurrSpeed = m_fSprintSpeed;
    m_fCurrRotateSpeed = m_fSprintRotateSpeed;

    m_vMoveTarget = *pDavePos;
}

void CFishGameObject::Stop()
{
    if (m_eFishState == Fish::FS_DIE) return;
    m_eFishState = Fish::FS_STOP;
}

void CFishGameObject::Swim()
{
    if (m_eFishState == Fish::FS_DIE) return;

    if (m_eFishState != Fish::FS_SWIM)
    {
        m_pSpineCom->Set_AniState(L"swim");
    }
    m_eFishState = Fish::FS_SWIM;

    m_fCurrSpeed = m_fSpeed;
    m_fCurrRotateSpeed = m_fRotateSpeed;
}

void CFishGameObject::AcquireTo(_vec3 const* pDavePos)
{
    //m_eFishState = Fish::FS_ATTACKTO;
    _vec3 vMyPos;
    m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
    _vec3 vMag = vMyPos - *pDavePos;

    m_bMoveToRotateEnable = false;

    m_fCurrSpeed = 5.f;

    float fLen = D3DXVec3Length(&vMag);
    if (fLen < 0.1f)
    {
        if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
        {
            auto pGetItemUI = CGetItemUI::Create(-500.f, 250.f);
            pGetItemUI->Set_Title(m_sFishName);
            pGetItemUI->Set_Rank(L"Rank " + ::to_wstring(m_iRank));

            std::wstringstream wss;
            wss << std::fixed << std::setprecision(1) << m_fWeight << L"kg";
            std::wstring result = wss.str();

            pGetItemUI->Set_Weight(result);
            pGetItemUI->Set_StarCnt(m_iStar);
            pGetItemUI->Set_ImgAssetName(m_sThumbNailAssetName);
            pGetItemUI->Ready_AfterCreate();
            pLayer->Add_GameObject(L"GetItemUI", pGetItemUI);
        }

        if (auto pDave = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CDiveDave>(L"DiveDave"))
        {
            pDave->Change_Weight(m_fWeight);
        }

        CGameMemMgr::CDiveInfo::DIVE_FISH fish{};
        fish.fWeight = m_fWeight;
        fish.iRank = m_iRank;
        fish.iStar = m_iStar;
        fish.sFishName = m_sFishName;
        fish.sThumbNailAssetName = m_sThumbNailAssetName;
        fish.iMeatCnt = m_iMeatCnt;
        fish.fLength = m_fLength;
        fish.sSushiThumbNailAssetName = m_sSushiThumbNailAssetName;
        fish.iSushiLv = m_iSushiLv;
        fish.iSushiMoney = m_iSushiMoney;
        fish.bFish = true;
        CGameMemMgr::GetInstance()->Get_DiveInfos().back().Add_FishFront(fish);

        Set_DeadCascade();
        

        return;
    }

    //m_fAttackToTimer = 0.f;
    //m_fCurrSpeed = m_fSprintSpeed;
    //m_fCurrRotateSpeed = m_fSprintRotateSpeed;

    m_bDieAndAcquire = true;
    m_vMoveTarget = *pDavePos;
}

void CFishGameObject::JacksalAcquire()
{
    if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
    {
        auto pGetItemUI = CGetItemUI::Create(-500.f, 250.f);
        pGetItemUI->Set_Title(m_sFishName);
        pGetItemUI->Set_Rank(L"Rank " + ::to_wstring(m_iRank));

        std::wstringstream wss;
        wss << std::fixed << std::setprecision(1) << m_fWeight << L"kg";
        std::wstring result = wss.str();

        pGetItemUI->Set_Weight(result);
        pGetItemUI->Set_StarCnt(m_iStar);
        pGetItemUI->Set_ImgAssetName(m_sThumbNailAssetName);
        pGetItemUI->Ready_AfterCreate();
        pLayer->Add_GameObject(L"GetItemUI", pGetItemUI);

    }

    if (auto pDave = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CDiveDave>(L"DiveDave"))
    {
        pDave->Change_Weight(m_fWeight);
    }


    CGameMemMgr::CDiveInfo::DIVE_FISH fish{};
    fish.fWeight = m_fWeight;
    fish.iRank = m_iRank;
    fish.iStar = m_iStar;
    fish.sFishName = m_sFishName;
    fish.sThumbNailAssetName = m_sThumbNailAssetName;
    fish.iMeatCnt = m_iMeatCnt;
    fish.fLength = m_fLength;
    fish.sSushiThumbNailAssetName = m_sSushiThumbNailAssetName;
    fish.iSushiLv = m_iSushiLv;
    fish.iSushiMoney = m_iSushiMoney;
    fish.bFish = true;
    CGameMemMgr::GetInstance()->Get_DiveInfos().back().Add_FishFront(fish);

    Set_DeadCascade();

}

_int CFishGameObject::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (m_bManual)
    {
        if (CDInputMgr::GetInstance()->Key_Pressing(DIK_M))
        {
            CHelper::GetMousePointInWorld(&m_vMoveTarget);
        }
    }


    if (m_bDamaged)
    {
        m_fDamagedTimer += fTimeDelta;

        if (m_fDamagedTimer > 0.1f)
        {
            m_pSpineCom->Set_ColorWhite(false);
            m_fDamagedTimer = 0.f;
            m_bDamaged = false;
        }
    }



    if (m_eFishState == Fish::FS_STOP)
    {

    }
    else if (m_eFishState == Fish::FS_SWIM)
    {
        m_fMoveTargetReLocateTimer += fTimeDelta;
        if (m_fMoveTargetReLocateTimer > 2.f)
        {
            //m_vMoveTarget();

            float randX = rand() % 300;
            float randY = rand() % 300;
            randX /= 10;
            randY /= 10;
            _vec3 vNewMoveTarget = { randX , randY , 0.f };
            m_vMoveTarget = vNewMoveTarget;

            m_fMoveTargetReLocateTimer = 0.f;
        }

        MoveTo(&m_vMoveTarget, fTimeDelta);
    }
    else if (m_eFishState == Fish::FS_DIE)
    {
        m_fDieTimer += fTimeDelta;

        float fDarkNess = m_pSpineCom->Get_ColorDarkness();
        if (fDarkNess > 0.6f)
        {
            m_pSpineCom->Set_ColorDarkness(fDarkNess - (1.f * fTimeDelta));
        }

        if (m_fDieTimer > 6.f)
        {
            Set_DeadCascade();
            return OBJ_DEAD;
        }

        if (m_bDieAndAcquire)
        {
            MoveTo(&m_vMoveTarget, fTimeDelta);
        }
    }
    else if (m_eFishState == Fish::FS_RUNFROM)
    {
        MoveTo(&m_vMoveTarget, fTimeDelta);

        m_fRunFromTimer += fTimeDelta;
        if (m_fRunFromTimer > 1.f)
        {
            //Swim();
            m_fRunFromTimer = 0.f;
        }

        if (!m_bIntersectDetetboxDave)
        {
            Swim();
        }
    }
    else if (m_eFishState == Fish::FS_ATTACKTO)
    {
        MoveTo(&m_vMoveTarget, fTimeDelta);

        m_fAttackToTimer += fTimeDelta;
        if (m_fAttackToTimer > 0.2f)
        {
            //Swim();
            //m_fMoveTargetReLocateTimer = 99.f;
            m_fAttackToTimer = 0.f;
        }

        if (!m_bIntersectDetetboxDave)
        {
            Swim();
        }
    }
    else if (m_eFishState == Fish::FS_QTE)
    {
        MoveTo(&m_vMoveTarget, fTimeDelta);
    }
    else if (m_eFishState == Fish::FS_ACQUIRETO)
    {
        float fDarkNess = m_pSpineCom->Get_ColorDarkness();
        if (fDarkNess > 0.6f)
        {
            m_pSpineCom->Set_ColorDarkness(fDarkNess - (1.f * fTimeDelta));
        }

        MoveTo(&m_vMoveTarget, fTimeDelta);
    }

    return iExit;
}

void CFishGameObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
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

    bool bColorWhite = m_pSpineCom->Get_ColorWhite();
    if (bColorWhite)
    {
        pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
        pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }

    m_pSpineCom->Render_Spine(m_pDynamicBuffer);

    if (bColorWhite)
    {
        pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    }

    



    if (m_eFishState == Fish::FS_ATTACKTO)
    {
        
        {
            if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_AggresiveFishPoint"))
            {
                if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
                {
                    pGraphicDev->SetTexture(0, pTexture->Get_Texture());
                }
            }
            _matrix matTrs;
            D3DXMatrixTranslation(&matTrs, 0.0f, 5.5f, 0.f);

            _matrix matWorld = *m_pTransformCom->Get_World();
            for (int i = 0; i < 3; ++i) {
                _vec3 vAxis = *(_vec3*)&matWorld.m[i][0]; // 행렬의 각 축(Right, Up, Look) 추출
                D3DXVec3Normalize(&vAxis, &vAxis);        // 방향만 남기고 정규화
                vAxis *= 0.1f;                            // 원하는 스케일(0.1) 곱하기
                memcpy(&matWorld.m[i][0], &vAxis, sizeof(_vec3)); // 다시 행렬에 삽입
            }

            _matrix res = matTrs * matWorld;

            pGraphicDev->SetTransform(D3DTS_WORLD, &res);
            m_pBufferCom->Render_Buffer();
        }



        {
            if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_AgressiveMark"))
            {
                if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
                {
                    pGraphicDev->SetTexture(0, pTexture->Get_Texture());
                }
            }

            _matrix matTrs;
            D3DXMatrixTranslation(&matTrs, 5.5f, 5.5f, 0.f);

            _matrix matWorld = *m_pTransformCom->Get_World();

            for (int i = 0; i < 3; ++i) {
                _vec3 vAxis = *(_vec3*)&matWorld.m[i][0]; // 행렬의 각 축(Right, Up, Look) 추출
                D3DXVec3Normalize(&vAxis, &vAxis);        // 방향만 남기고 정규화
                vAxis *= 0.1f;                            // 원하는 스케일(0.1) 곱하기
                memcpy(&matWorld.m[i][0], &vAxis, sizeof(_vec3)); // 다시 행렬에 삽입
            }

            _matrix res = matTrs * matWorld;

            pGraphicDev->SetTransform(D3DTS_WORLD, &res);
            m_pBufferCom->Render_Buffer();
        }
    }




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

    // 다이나믹 버퍼
    if (FAILED((AddComponent<Engine::CDynamicBuffer, ID_STATIC>(L"Proto_DynamicBuffer_Spine", L"Com_DynamicBuffer", &m_pDynamicBuffer))))
        return E_FAIL;
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
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


    //m_fsm.AddState(Fish::FS_STOP, make_unique<Fish::CFishStopState>(this));
    //m_fsm.AddState(Fish::FS_SWIM, make_unique<Fish::CFishSwimState>(this));
    //m_fsm.AddState(Fish::FS_MANUAL, make_unique<Fish::CFishManualState>(this));


    //m_fsm.Get_CurrentState()->Enter();

    //Swim();
	return S_OK;
}

void CFishGameObject::MoveTo(_vec3* vToPos, const float& fTimeDelta)
{ // Right가 물고기 대가리 방향
    _vec3 vRight;
    m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
    D3DXVec3Normalize(&vRight, &vRight);

    // 현재 물고기 위치
    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    _vec3 vChaseMag = *vToPos - vPos;
    float vChaseLength = D3DXVec3Length(&vChaseMag);

    // 쫓을 방향
    _vec3 vChaseDir;
    D3DXVec3Normalize(&vChaseDir, &vChaseMag);

    // 쫒을거리가 0.01 이상이어야 쫒느다.
    if (vChaseLength > 0.001f)
    {
        // 물고기 움직이는 로직 근데 체이스타겟보다 멀리가면 안되니까 분기처리
        float fMoveDistance = m_fCurrSpeed * fTimeDelta;  // 이번에 갈거리 
        if (vChaseLength <= fMoveDistance)
        {
            m_pTransformCom->Set_Pos(vToPos->x, vToPos->y, vToPos->z);
        }
        else
        {
            m_pTransformCom->Move_Pos(&vChaseDir, m_fCurrSpeed, fTimeDelta);
        }

        if (m_bMoveToRotateEnable)
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
                    float fRotationAmount = m_fCurrRotateSpeed * fTimeDelta; // 이번에 돌릴 각도

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



                    _vec3 vCurrAngle2;
                    m_pTransformCom->Get_Rotation(&vCurrAngle2);
                    if (fabsf(vCurrAngle2.z) > m_fRotateAngleLimit)
                    {
                        if (vCurrAngle2.z < 0)
                        {
                            vCurrAngle2.z = -(m_fRotateAngleLimit - 0.1f);
                            //vCurrAngle.z += fTimeDelta * 10.f;
                        }
                        else if (vCurrAngle2.z > 0)
                        {
                            vCurrAngle2.z = (m_fRotateAngleLimit - 0.1f);
                            //vCurrAngle.z -= fTimeDelta * 10.f;
                        }
                        m_pTransformCom->Set_Rotation(&vCurrAngle2);
                    }

                }
            }
            // 원래 위에 if를 타야하는데
            // 못타는경우임, 그경우엔 다시 if탈수있도록 세팅
            else
            {
                if (vCurrAngle.z < 0)
                {
                    vCurrAngle.z = -(m_fRotateAngleLimit - 0.00001f);
                    //vCurrAngle.z += fTimeDelta * 10.f;
                }
                else if (vCurrAngle.z > 0)
                {
                    vCurrAngle.z = (m_fRotateAngleLimit - 0.00001f);
                    //vCurrAngle.z -= fTimeDelta * 10.f;
                }
                m_pTransformCom->Set_Rotation(&vCurrAngle);
            }
        }
        
    }
}

void CFishGameObject::Free()
{
    CGameObject::Free();
}
