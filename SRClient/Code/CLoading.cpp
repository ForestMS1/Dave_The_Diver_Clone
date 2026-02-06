#include "pch.h"
#include "CLoading.h"
#include "CProtoMgr.h"
#include "CImguiMgr.h"

CLoading::CLoading()
    : m_bFinish(false)
    , m_eLoadingID(LOADING_END)
    , m_Crt({})
    , m_hThread(nullptr)
{
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
    InitializeCriticalSection(&m_Crt);

    m_eLoadingID = eID;


    m_hThread = (HANDLE)_beginthreadex(NULL, // 보안 속성(핸들의 상속 여부, NULL인 경우 상속에서 제외)
                                        0,  // 디폴트 스탯 사이즈(1 바이트)
                                        Thread_Main, // 구동할 쓰레드 함수
                                        this,          // 3번 매개 변수 함수를 통해 가공할 데이터 주소
                                        0,             // 쓰레드 생성 및 실행을 조정하기 위한 옵션
                                        NULL);         // 쓰레드 ID


    return S_OK;
}

_uint CLoading::Loading_Stage()
{
    {
        m_sLoading = L"Buffer Loading.....................................";
        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TriCol", Engine::CTriCol::Create())))
            return E_FAIL;

        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcCol", Engine::CRcCol::Create())))
            return E_FAIL;

        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TerrainTex", Engine::CTerrainTex::Create( VTXCNTX, VTXCNTZ, VTXITV))))
            return E_FAIL;

        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CubeTex", Engine::CCubeTex::Create())))
            return E_FAIL;
    }

    
    {
        m_sLoading = L"Texture Loading.....................................";
    }

    {
        m_sLoading = L"Etc Loading.....................................";

        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Transform", Engine::CTransform::Create())))
            return E_FAIL;

        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Calculator", Engine::CCalculator::Create())))
            return E_FAIL;
    }
    

     m_sLoading = L"Loading Complete !!!!";

     m_bFinish = true;     

     return 0;
}

_uint CLoading::Loading_MapEditor()
{
    {
        m_sLoading = L"Buffer Loading.....................................";
        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev))))
            return E_FAIL;

        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev))))
            return E_FAIL;

        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CubeTex", Engine::CCubeTex::Create(m_pGraphicDev))))
            return E_FAIL;

        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MapTerrainTex", Engine::CMapTerrainTex::Create(m_pGraphicDev, 14, 14, 1))))
            return E_FAIL;


        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MiniMapTerrainTex",Engine::CMapTerrainTex::Create(m_pGraphicDev,2,2,13))))
            return E_FAIL;
    }


    {
        m_sLoading = L"Texture Loading.....................................";
        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Tile0.jpg", 1))))
            return E_FAIL;

        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MiniMapBlankTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Map/mapBlack%d.png", 2))))
            return E_FAIL;


    }

    {
        m_sLoading = L"Etc Loading.....................................";

        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev))))
            return E_FAIL;

        if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Calculator", Engine::CCalculator::Create(m_pGraphicDev))))
            return E_FAIL;
    }


    m_sLoading = L"Loading Complete !!!!";

    m_bFinish = true;

    return 0;
}


unsigned int CLoading::Thread_Main(void* pArg)
{
    CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

    int iFlag(0);

    EnterCriticalSection(pLoading->Get_Crt());

    switch (pLoading->Get_LoadingID())
    {
    case LOADING_STAGE:
        iFlag = pLoading->Loading_Stage();
        break;
    case LOADING_MAPEDITOR:
        iFlag = pLoading->Loading_MapEditor();
        break;

    case LOADING_BOSS:
        break;
    }

    LeaveCriticalSection(pLoading->Get_Crt());

    //_endthreadex(0);

    return iFlag;       // 0 리턴 시, _endthreadex가 자동 호출
}

CLoading* CLoading::Create(LOADINGID eID)
{
    CLoading* pLoading = new CLoading;

    if (FAILED(pLoading->Ready_Loading(eID)))
    {
        Safe_Release(pLoading);

        MSG_BOX("CLoading Create Failed");
        return nullptr;
    }

    return pLoading;
}

void CLoading::Free()
{
    WaitForSingleObject(m_hThread, INFINITE);

    CloseHandle(m_hThread);

    DeleteCriticalSection(&m_Crt);
}
