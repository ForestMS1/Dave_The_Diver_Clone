#include "CAssetGlb.h"
#include "CHelper.h"
#include "CGraphicDev.h"
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
struct CAssetGlb::Impl
{
	Assimp::Importer importer;
	const aiScene* scene = nullptr;
	aiMatrix4x4 rootTransform;
};


CAssetGlb::CAssetGlb(std::wstring_view m_sAssetPath) : CAsset(m_sAssetPath)
{
	m_pImpl = new Impl();

}

CAssetGlb::~CAssetGlb()
{
	delete m_pImpl;
}

HRESULT CAssetGlb::Load()
{
	m_dwVtxCnt = 0;
	m_dwTriCnt = 0;
	m_eAssetState = LOADING;
	unsigned int flags = aiProcess_Triangulate | // 삼각형화 (DX 필수)
		aiProcess_ConvertToLeftHanded |   // 오른손 -> 왼손 좌표계 (DX 필수)
		aiProcess_JoinIdenticalVertices | // 중복 정점 통합
		aiProcess_LimitBoneWeights |      // 최대 본 영향력 4개로 제한
		aiProcess_PopulateArmatureData;   // 뼈대 구조 최적화


	m_pImpl->scene = m_pImpl->importer.ReadFile(CHelper::WStringToString(m_sAssetPath), flags);

	if (!m_pImpl->scene || m_pImpl->scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pImpl->scene->mRootNode)
	{
		m_eAssetState = LOADFAIL;
		MSG_BOX("Assimp error: %s\n", m_pImpl->importer.GetErrorString());

	}

	size_t totalV = 0, totalF = 0;
	for (unsigned i = 0; i < m_pImpl->scene->mNumMeshes; ++i) {
		aiMesh* mesh = m_pImpl->scene->mMeshes[i];
		totalV += mesh->mNumVertices;
		totalF += mesh->mNumFaces;
	}
	m_dwVtxCnt = (DWORD)totalV;
	m_dwTriCnt = (DWORD)totalF;

	vertices.clear();
	vertices.resize(totalV);


	pIndex = new INDEX32[totalF];

	size_t vBase = 0;
	size_t fBase = 0;

	for (unsigned i = 0; i < m_pImpl->scene->mNumMeshes; ++i) {
		aiMesh* mesh = m_pImpl->scene->mMeshes[i];


		for (unsigned j = 0; j < mesh->mNumVertices; ++j) {
			auto& v = vertices[vBase + j];   
			auto p = mesh->mVertices[j];

			v.vPosition = { p.x, p.y, p.z };

			if (mesh->HasNormals()) {
				auto n = mesh->mNormals[j];
				v.vNormal = { n.x, n.y, n.z };
			}

			if (mesh->HasTextureCoords(0)) {
				auto uv = mesh->mTextureCoords[0][j];
				v.vTexUV = { uv.x, uv.y };
			}
		}

		
		for (unsigned j = 0; j < mesh->mNumFaces; ++j) {
			const aiFace& face = mesh->mFaces[j];
			pIndex[fBase + j]._0 = (UINT)(vBase + face.mIndices[0]); 
			pIndex[fBase + j]._1 = (UINT)(vBase + face.mIndices[1]);
			pIndex[fBase + j]._2 = (UINT)(vBase + face.mIndices[2]);
		}

		vBase += mesh->mNumVertices;
		fBase += mesh->mNumFaces;

		
		aiMaterial* mat = m_pImpl->scene->mMaterials[mesh->mMaterialIndex];
		aiString texPath;
		if (mat->GetTexture(aiTextureType_BASE_COLOR, 0, &texPath) != AI_SUCCESS)
		{
			mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
		}
		std::string path = texPath.C_Str();
		if (!path.empty() && path[0] == '*')
		{
			int idx = std::atoi(path.c_str() + 1);   // 0
			aiTexture* atex = m_pImpl->scene->mTextures[idx];

			tex = nullptr;

			HRESULT hr = D3DXCreateTextureFromFileInMemory(
				CGraphicDev::GetInstance()->Get_GraphicDev(),
				atex->pcData,
				(UINT)atex->mWidth,   
				&tex
			);

			if (FAILED(hr)) {
				m_eAssetState = LOADFAIL;
				MSG_BOX("Texture error: %s\n", m_pImpl->importer.GetErrorString());
			}
		}
	}

	
	m_eAssetState = LOADED;
	return S_OK;
}

HRESULT CAssetGlb::Unload()
{
	m_eAssetState = UNLOAD;
	return S_OK;
}

void CAssetGlb::Free()
{
	Safe_Delete_Array(pIndex);
	vertices.clear();
	
}

CAssetGlb* CAssetGlb::Create(std::wstring_view m_sAssetPath)
{
	return new CAssetGlb{ m_sAssetPath };
}