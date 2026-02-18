#pragma once
#include "CAsset.h"
#include "CDynamicBuffer.h"


namespace Assimp
{
	class Importer;
}
class aiScene;
class aiNode;
class aiMesh;
class aiMaterial;
class aiTexture;
class aiAnimation;
class aiNodeAnim;
enum aiTextureType;

BEGIN(Engine)

class ENGINE_DLL CAssetFbx2 : public CAsset
{
public:
	typedef struct tagBoneInfo {
		std::string sName;
		_matrix matOffset;     
		_matrix matFinalTransform;   
		std::vector<_uint> vecChildren;   
	} BONE_INFO;

	typedef struct tagVertexWeight {
		std::vector<_uint> vecBoneIDs;
		std::vector<_float> vecWeights;  
	} VERTEX_WEIGHT;

	class CFBX2Mesh
	{
	public:
		void VertexBuffer_Lock(LPDIRECT3DVERTEXBUFFER9 pVB) const
		{
			if (m_bHasBone)
			{
				VTXTEX* pV;
				pVB->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

				for (size_t i = 0; i < m_vecVertex.size(); i++)
				{
					D3DXVECTOR3 skinnedPos(0, 0, 0);
					D3DXVECTOR3 skinnedNormal(0, 0, 0); // 노말 합계를 위한 변수

					auto& weights = m_vecVertexWeights[i];

					for (size_t j = 0; j < weights.vecBoneIDs.size(); j++)
					{
						int boneID = weights.vecBoneIDs[j];
						float w = weights.vecWeights[j];

						// 1. 위치(Position) 계산
						D3DXVECTOR4 vPos(m_vecVertex[i].vPosition, 1.0f);
						D3DXVECTOR4 transformedPos;
						D3DXVec4Transform(&transformedPos, &vPos, &m_vecBones[boneID].matFinalTransform);
						skinnedPos += D3DXVECTOR3(transformedPos.x, transformedPos.y, transformedPos.z) * w;

						// 2. 노말(Normal) 계산
						D3DXVECTOR3 transformedNormal;
						// TransformNormal은 행렬의 이동(Translation) 성분을 무시하고 방향만 회전시킵니다.
						D3DXVec3TransformNormal(&transformedNormal, &m_vecVertex[i].vNormal, &m_vecBones[boneID].matFinalTransform);
						skinnedNormal += transformedNormal * w;
					}

					pV[i].vPosition = skinnedPos;

					// 3. 노말 정규화 (보간된 노말은 길이가 1이 아닐 수 있으므로 반드시 정규화)
					D3DXVec3Normalize(&pV[i].vNormal, &skinnedNormal);

					pV[i].vTexUV = m_vecVertex[i].vTexUV;
				}

				pVB->Unlock();
			}
			else
			{
				VTXTEX* pV;
				pVB->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

				auto vCnt = m_vecVertex.size();
				memcpy(pV, m_vecVertex.data(), sizeof(VTXTEX) * vCnt);

				pVB->Unlock();
			}
		}
		void IndexBuffer_Lock(LPDIRECT3DINDEXBUFFER9 pIB) const
		{
			INDEX32* pV;
			pIB->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

			auto vCnt = m_vecIndex.size();
			memcpy(pV, m_vecIndex.data(), sizeof(INDEX32) * vCnt);

			pIB->Unlock();
		}
	public:
		vector<VTXTEX> m_vecVertex;
		vector<INDEX32> m_vecIndex;
		vector<wstring> m_vecTexture;

		vector<BONE_INFO> m_vecBones;
		vector<VERTEX_WEIGHT> m_vecVertexWeights;
		bool m_bHasBone = false;
	};

private:
	explicit CAssetFbx2(std::wstring_view m_sAssetPath);
	virtual ~CAssetFbx2();

public:
	std::vector<CFBX2Mesh> const* Get_Meshes() const { return &m_vecMeshes; }

	void Set_Animation(_uint clip) { m_iAniClip = clip; }
	void Update_Animation(const _float& fTimeDelta);

public:
	HRESULT Load() override;
	HRESULT Unload() override;

public:
	static CAssetFbx2* Create(std::wstring_view m_sAssetPath);

private:
	void ProcessNode(aiNode const* const pNode, aiScene const* const pScene);
	CFBX2Mesh ProcessMesh(aiMesh const* const pMesh, aiScene const* const pScene);
	vector<wstring> LoadMaterialTexture(aiMaterial const* const pMat, aiTextureType eAiTexType, aiScene const* const pScene);
	HRESULT LoadEmbbedableTexture(aiTexture const* const  pEmbeddedTexture, std::wstring_view svAssetName);

	void ProcessBone(aiMesh const* const pMesh, CFBX2Mesh* const pFbxMesh);
	aiNodeAnim const* FindNodeAnim(aiAnimation const* const pAni, std::string_view sNodeName);
	void NodeTraversalForBoneCalcuate(const _float& fAniTime, aiNode const* const pNode, const _matrix& matParentTransform);
private:
	unique_ptr<Assimp::Importer> m_pImporter;
	const aiScene* m_pScene;
	std::vector<CFBX2Mesh> m_vecMeshes;
	unordered_map<std::string, void*> m_mapBone;

	_uint m_iAniClip;
	_float m_fAccAniTime;
};

END