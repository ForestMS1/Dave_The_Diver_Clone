#include "pch.h"
#include "CLoading.h"
#include "CProtoMgr.h"
#include "CGraphicDev.h"

#include "CAssetTexture.h"
#include "CAssetCubeTexture.h"
#include "CAssetFmodSound.h"
#include "CAssetMgr.h"
#include "CAssetFbx.h"

CLoading::CLoading()
    : m_bFinish(false), m_eLoadingID(LOADING_END)
{
    //ZeroMemory(m_szLoading, sizeof(m_szLoading));
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
    // lstrcpy(m_szLoading, L"Buffer Loading.....................................");
    m_sLoading = L"Buffer Loading.....................................";

    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TriCol", Engine::CTriCol::Create())))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcCol", Engine::CRcCol::Create())))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CubeCol", Engine::CCubeCol::Create(D3DXCOLOR(1.f, 0.f, 1.f, 1.f)))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TerrainTex", Engine::CTerrainTex::Create(VTXCNTX, VTXCNTZ, VTXITV))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CubeTex", Engine::CCubeTex::Create())))
        return E_FAIL;



    // 기존 로드하던 에셋들
    {
        CAssetMgr::GetInstance()->AddAsset(L"Tex_Player", CAssetTexture::Create(L"../Bin/Resource/Texture/Player/Ma.jpg"));
        CAssetMgr::GetInstance()->AddAsset(L"Tex_Terrain0", CAssetTexture::Create(L"../Bin/Resource/Texture/Terrain/Terrain0.png"));
        for (int i = 0; i < 2; ++i)
        {
            wstring s = L"../Bin/Resource/Texture/Terrain/Grass_" + ::to_wstring(i) + L".tga";
            CAssetMgr::GetInstance()->AddAsset(L"Tex_TerrainGrass", CAssetTexture::Create(s.c_str()));
        }
        for (int i = 0; i < 4; ++i)
        {
            wstring s = L"../Bin/Resource/Texture/SkyBox/burger" + ::to_wstring(i) + L".dds";
            CAssetMgr::GetInstance()->AddAsset(L"CubeTex_Burger", CAssetCubeTexture::Create(s.c_str()));
        }
        CAssetMgr::GetInstance()->AddAsset(L"CubeTex_160", CAssetCubeTexture::Create(L"../Bin/Resource/Texture/160.dds"));
        for (int i = 0; i < 90; ++i)
        {
            wstring s = L"../Bin/Resource/Texture/Explosion/Explosion" + ::to_wstring(i) + L".png";
            CAssetMgr::GetInstance()->AddAsset(L"Tex_Explosion", CAssetTexture::Create(s.c_str()));
        }

        CAssetMgr::GetInstance()->AddAsset(L"Sound_TADA", CAssetFmodSound::Create(L"../Bin/Resource/Sound/TADA.mp3"));
        CAssetMgr::GetInstance()->AddAsset(L"Sound_Success", CAssetFmodSound::Create(L"../Bin/Resource/Sound/Success.wav"));

        CAssetMgr::GetInstance()->AddAsset(L"Fbx_Dancing", CAssetFbx::Create(L"../Bin/Resource/Fbx/Dancing.fbx"));

        CAssetMgr::GetInstance()->AddAsset(L"Tex_Button", CAssetTexture::Create(L"../Bin/Resource/Texture/Button.png"));
        CAssetMgr::GetInstance()->AddAsset(L"Tex_White1PX", CAssetTexture::Create(L"../Bin/Resource/Texture/White1PX.png"));
    }
    CAssetMgr::GetInstance()->LoadAsset();

    auto mgr = CAssetMgr::GetInstance();

    //lstrcpy(m_szLoading, L"Texture Loading.....................................");

    m_sLoading = L"Texture Loading.....................................";

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_PlayerTexture", Engine::CTexture::Create(L"Tex_Player"))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TerrainTexture", Engine::CTexture::Create(L"Tex_Terrain0"))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TerrainTexture2", Engine::CTexture::Create(L"Tex_TerrainGrass"))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SkyBoxTexture", Engine::CTexture::Create(L"CubeTex_Burger"))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SkyBoxTexture2", Engine::CTexture::Create(L"CubeTex_160"))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_EffectTexture", Engine::CTexture::Create(L"Tex_Explosion"))))
        return E_FAIL;

   // lstrcpy(m_szLoading, L"Etc Loading.....................................");
    m_sLoading = L"Etc Loading.....................................";

     if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Transform", Engine::CTransform::Create())))
     	return E_FAIL;

     if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Calculator", Engine::CCalculator::Create())))
         return E_FAIL;

     //lstrcpy(m_szLoading, L"Loading Complete !!!!");
     m_sLoading = L"Loading Complete !!!! 컴프리트";

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
