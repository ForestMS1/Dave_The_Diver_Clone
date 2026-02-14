#include "CAssetGlb.h"
#include "CHelper.h"

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


	aiMesh* mesh = m_pImpl->scene->mMeshes[0];
	m_dwVtxCnt = mesh->mNumVertices;
	m_dwTriCnt = mesh->mNumFaces;

	vertices.resize(mesh->mNumVertices);
	

	
	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		auto p = mesh->mVertices[i];

	
		vertices[i].vPosition.x = p.x;
		vertices[i].vPosition.y = p.y;
		vertices[i].vPosition.z = p.z;

		if (mesh->HasNormals()) {
			auto n = mesh->mNormals[i];
			vertices[i].vNormal.x = n.x;
			vertices[i].vNormal.y = n.y;
			vertices[i].vNormal.z = n.z;
		}

		if (mesh->HasTextureCoords(0)) {
			auto uv = mesh->mTextureCoords[0][i];
			vertices[i].vTexUV.x = uv.x;
			vertices[i].vTexUV.y = uv.y;
		}

		
	
	}

	pIndex = new INDEX32[m_dwTriCnt];
	for (int i = 0; i < m_dwTriCnt; ++i) {
		const aiFace& face = mesh->mFaces[i];
		pIndex[i]._0 = face.mIndices[0];
		pIndex[i]._1 = face.mIndices[1];
		pIndex[i]._2 = face.mIndices[2];
	}
	aiMaterial* mat = m_pImpl->scene->mMaterials[m_pImpl->scene->mNumMeshes];

	aiString texPath;
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
	{
		const aiTexture* tex = m_pImpl->scene->GetEmbeddedTexture(texPath.C_Str());

		if (tex)
		{
			if (tex->mHeight == 0)
			{
				// compressed (jpg/png)
				// tex->pcData = binary blob
				// tex->mWidth = data size
			}
			else
			{
				// raw RGBA
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