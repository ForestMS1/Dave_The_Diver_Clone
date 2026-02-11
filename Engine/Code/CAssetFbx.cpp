
#include "CAssetFbx.h"

#include "CHelper.h"

// 우리가 메모리릭 검출을 위해 사용한 매크로 new재정의로인해
// assimp의 커스텀 new 오퍼레이터가 영향을 받아버리고 있다
// 그래서 assimp 인클루드 하는동안은 new에 걸었던매크로를 해제시키고
// assimp 인클루드가 끝나면 다시 new에 매크로를 걸고 있다.

#ifdef new
#undef new  
#endif

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef _DEBUG
#define new DBG_NEW
#endif

// 클라에 assimp 노출을 막기위해 assimp에 관련한 것은 여기서 정의할 예정
struct CAssetFbx::Impl
{
	Assimp::Importer importer; 
	const aiScene* scene = nullptr;
	aiMatrix4x4 rootTransform;
};

CAssetFbx::CAssetFbx(std::wstring_view m_sAssetPath)
	: CAsset(m_sAssetPath)
{
	m_pImpl = new Impl();
}

CAssetFbx::~CAssetFbx()
{
	delete m_pImpl;
}

HRESULT CAssetFbx::Load()
{
    m_eAssetState = LOADING;
    //  임포트 플래그 설정
    unsigned int flags = aiProcess_Triangulate | // 삼각형화 (DX 필수)
        aiProcess_ConvertToLeftHanded |   // 오른손 -> 왼손 좌표계 (DX 필수)
        aiProcess_JoinIdenticalVertices | // 중복 정점 통합
        aiProcess_LimitBoneWeights |      // 최대 본 영향력 4개로 제한
        aiProcess_PopulateArmatureData;   // 뼈대 구조 최적화

    // 파일 읽기
    m_pImpl->scene = m_pImpl->importer.ReadFile(CHelper::WStringToString(m_sAssetPath), flags);

    if (!m_pImpl->scene) {
        // 에러 출력: m_pImpl->importer.GetErrorString()
        m_eAssetState = LOADFAIL;
        return E_FAIL;
    }

    // 루트 노드의 역행렬 보관 (전역 공간 보정용)
    m_pImpl->rootTransform = m_pImpl->scene->mRootNode->mTransformation;
    m_pImpl->rootTransform.Inverse();

    // TODO:

    m_eAssetState = LOADED;
	return S_OK;
}

HRESULT CAssetFbx::Unload()
{
    m_eAssetState = UNLOAD;
	return S_OK;
}

CAssetFbx* CAssetFbx::Create(std::wstring_view m_sAssetPath)
{
	return new CAssetFbx{ m_sAssetPath };
}

void CAssetFbx::Free()
{
}
