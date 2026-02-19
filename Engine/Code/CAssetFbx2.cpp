#include "CAssetFbx2.h"
#include "CHelper.h"

#include "CAssetTextureFromCreate.h"
#include "CAssetTextureFromInMem.h"
#include "CAssetMgr.h"

CAssetFbx2::CAssetFbx2(std::wstring_view m_sAssetPath)
    : CAsset(m_sAssetPath)
    , m_pImporter(make_unique<Assimp::Importer>())
    , m_iAniClip(0)
    , m_fAccAniTime(0.f)
{
}

CAssetFbx2::~CAssetFbx2()
{
}

aiVector3D CalcAniInterpolatedPosition(
    float animTime,
     aiNodeAnim const* nodeAnim)
{
    if (nodeAnim->mNumPositionKeys == 1)
        return nodeAnim->mPositionKeys[0].mValue;

    UINT index = 0;

    for (UINT i = 0; i < nodeAnim->mNumPositionKeys - 1; i++)
    {
        if (animTime < nodeAnim->mPositionKeys[i + 1].mTime)
        {
            index = i;
            break;
        }
    }

    UINT nextIndex = index + 1;

    float deltaTime =
        (float)(nodeAnim->mPositionKeys[nextIndex].mTime -
            nodeAnim->mPositionKeys[index].mTime);

    float factor =
        (animTime - (float)nodeAnim->mPositionKeys[index].mTime)
        / deltaTime;

    const aiVector3D& start =
        nodeAnim->mPositionKeys[index].mValue;

    const aiVector3D& end =
        nodeAnim->mPositionKeys[nextIndex].mValue;

    return start + factor * (end - start);
}

aiVector3D CalcAniInterpolatedScaling(
    float animTime,
     aiNodeAnim const* nodeAnim)
{
    if (nodeAnim->mNumScalingKeys == 1)
        return nodeAnim->mScalingKeys[0].mValue;

    UINT index = 0;

    for (UINT i = 0; i < nodeAnim->mNumScalingKeys - 1; i++)
    {
        if (animTime < nodeAnim->mScalingKeys[i + 1].mTime)
        {
            index = i;
            break;
        }
    }

    UINT nextIndex = index + 1;

    float deltaTime =
        (float)(nodeAnim->mScalingKeys[nextIndex].mTime -
            nodeAnim->mScalingKeys[index].mTime);

    float factor =
        (animTime - (float)nodeAnim->mScalingKeys[index].mTime)
        / deltaTime;

    const aiVector3D& start =
        nodeAnim->mScalingKeys[index].mValue;

    const aiVector3D& end =
        nodeAnim->mScalingKeys[nextIndex].mValue;

    return start + factor * (end - start);
}


aiQuaternion CalcAniInterpolatedRotation(
    float animTime,
     aiNodeAnim const* nodeAnim)
{
    if (nodeAnim->mNumRotationKeys == 1)
        return nodeAnim->mRotationKeys[0].mValue;

    UINT index = 0;

    for (UINT i = 0; i < nodeAnim->mNumRotationKeys - 1; i++)
    {
        if (animTime < nodeAnim->mRotationKeys[i + 1].mTime)
        {
            index = i;
            break;
        }
    }

    UINT nextIndex = index + 1;

    float deltaTime =
        (float)(nodeAnim->mRotationKeys[nextIndex].mTime -
            nodeAnim->mRotationKeys[index].mTime);

    float factor =
        (animTime - (float)nodeAnim->mRotationKeys[index].mTime)
        / deltaTime;

    const aiQuaternion& start =
        nodeAnim->mRotationKeys[index].mValue;

    const aiQuaternion& end =
        nodeAnim->mRotationKeys[nextIndex].mValue;

    aiQuaternion result;
    aiQuaternion::Interpolate(result, start, end, factor);
    result.Normalize();

    return result;
}

void CAssetFbx2::Update_Animation(const _float& fTimeDelta)
{
    if (!m_pScene || !m_pScene->HasAnimations())
        return;


   
    //static float time = 0.f;

    

    const aiAnimation* animation = m_pScene->mAnimations[m_iAniClip];

    float ticksPerSecond = (float)(animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.0f);

    m_fAccAniTime += ticksPerSecond * fTimeDelta;

    // float 나머지 연산
    float animTime = fmod(m_fAccAniTime, (float)animation->mDuration);

    _matrix matIdentity;
    D3DXMatrixIdentity(&matIdentity);

    NodeTraversalForBoneCalcuate(animTime, m_pScene->mRootNode,matIdentity);
    //ReadNodeHierarchy(animTime, m_pScene->mRootNode, matIdentity);
}


void CAssetFbx2::NodeTraversalForBoneCalcuate(const _float& fAniTime, aiNode const* const pNode, const _matrix& matParentTransform)
{
    std::string nodeName(pNode->mName.C_Str());

    const aiAnimation* animation = m_pScene->mAnimations[0];

    D3DXMATRIX nodeTransform;
    aiMatrix4x4 m = pNode->mTransformation;
    D3DXMatrixTranspose(&nodeTransform, (D3DXMATRIX*)&m);
    //memcpy(&nodeTransform, &node->mTransformation, sizeof(D3DXMATRIX));


    // 해당 노드 애니메이션 채널 찾기
     aiNodeAnim const* nodeAnim = FindNodeAnim(animation, nodeName);
    if (nodeAnim)
    {
        // 위치
        aiVector3D pos = CalcAniInterpolatedPosition(fAniTime, nodeAnim);
        D3DXMATRIX T;
        D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);

        // 회전
        aiQuaternion rot = CalcAniInterpolatedRotation(fAniTime, nodeAnim);
        D3DXQUATERNION q(rot.x, rot.y, rot.z, rot.w);
        D3DXMATRIX R;
        D3DXMatrixRotationQuaternion(&R, &q);

        // 스케일
        aiVector3D scale = CalcAniInterpolatedScaling(fAniTime, nodeAnim);
        D3DXMATRIX S;
        D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);

        nodeTransform = S * R * T;
    }

    D3DXMATRIX globalTransform = nodeTransform * matParentTransform;

    // Bone 이름과 매칭
    for (auto& mesh : m_vecMeshes)
    {
        for (auto& bone : mesh.m_vecBones)
        {

            if (bone.sName == nodeName)
            {
                bone.matFinalTransform =
                    bone.matOffset * globalTransform;
            }
        }
    }

    // 자식 노드 재귀
    for (UINT i = 0; i < pNode->mNumChildren; i++)
    {
        NodeTraversalForBoneCalcuate(fAniTime,
            pNode->mChildren[i],
            globalTransform);
    }
}

HRESULT CAssetFbx2::Load()
{
    m_eAssetState = LOADING;

    //  임포트 플래그 설정
    unsigned int flags = aiProcess_Triangulate | // 삼각형화 (DX 필수)
        aiProcess_ConvertToLeftHanded |   // 오른손 -> 왼손 좌표계 (DX 필수)
        aiProcess_JoinIdenticalVertices | // 중복 정점 통합
        aiProcess_LimitBoneWeights |      // 최대 본 영향력 4개로 제한
        aiProcess_PopulateArmatureData;   // 뼈대 구조 최적화

    m_pScene = m_pImporter->ReadFile(CHelper::WStringToString(m_sAssetPath), flags);

    if (!m_pScene) {
        // 에러 출력: m_pImpl->importer.GetErrorString()
        m_eAssetState = LOADFAIL;
        return E_FAIL;
    }

    ProcessNode(m_pScene->mRootNode, m_pScene);

   

    m_eAssetState = LOADED;

    return S_OK;
}

HRESULT CAssetFbx2::Unload()
{
    return S_OK;
}

CAssetFbx2* CAssetFbx2::Create(std::wstring_view m_sAssetPath)
{
    return new CAssetFbx2{ m_sAssetPath };
}

void CAssetFbx2::ProcessNode(aiNode const* const pNode, aiScene const* const pScene)
{
    for (_uint i = 0; i < pNode->mNumMeshes; i++) {
        aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
        m_vecMeshes.push_back(this->ProcessMesh(mesh, pScene));
    }

    for (_uint i = 0; i < pNode->mNumChildren; i++) {
        this->ProcessNode(pNode->mChildren[i], pScene);
    }
}

CAssetFbx2::CFBX2Mesh CAssetFbx2::ProcessMesh(aiMesh const* const pMesh, aiScene const* const pScene)
{
    CAssetFbx2::CFBX2Mesh fbxMesh{};

    if (pMesh->HasBones())
    {
        fbxMesh.m_bHasBone = true;
        ProcessBone(pMesh, &fbxMesh);
    }

    for (_uint i = 0; i < pMesh->mNumVertices; i++)
    {
        VTXTEX vertex{};

        // 초기 정점 위치
        vertex.vPosition.x = (float)pMesh->mVertices[i].x;
        vertex.vPosition.y = (float)pMesh->mVertices[i].y;
        vertex.vPosition.z = (float)pMesh->mVertices[i].z;

        // 0번인 디퓨즈 텍스쳐만
        if (pMesh->HasTextureCoords(0))
        {
            vertex.vTexUV.x = (float)pMesh->mTextureCoords[0][i].x;
            vertex.vTexUV.y = (float)pMesh->mTextureCoords[0][i].y;
        }

        // 초기 노말
        if (pMesh->HasNormals())
        {
            vertex.vNormal.x = (float)pMesh->mNormals[i].x;
            vertex.vNormal.y = (float)pMesh->mNormals[i].y;
            vertex.vNormal.z = (float)pMesh->mNormals[i].z;
        }

        fbxMesh.m_vecVertex.push_back(vertex);
    }

    for (_uint i = 0; i < pMesh->mNumFaces; i++)
    {
        aiFace face = pMesh->mFaces[i];
        for (_uint j = 0; j < face.mNumIndices; j += 3)
        {
            INDEX32 idx;
            idx._0 = face.mIndices[j + 0];
            idx._1 = face.mIndices[j + 1];
            idx._2 = face.mIndices[j + 2];
            fbxMesh.m_vecIndex.push_back(idx);
        }
    }


    if (pMesh->mMaterialIndex >= 0) {
        aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];
        for (int type = aiTextureType_NONE; type <= aiTextureType_UNKNOWN; type++)
        {
            if (material->GetTextureCount((aiTextureType)type) > 0)
            {
                // 일단은 디퓨즈 텍스쳐만
                if (aiTextureType_DIFFUSE == type)
                {
                    fbxMesh.m_vecTexture = LoadMaterialTexture(material, aiTextureType_DIFFUSE, pScene);
                }
            }
        }
    }

    return fbxMesh;
}

void CAssetFbx2::ProcessBone(aiMesh const* const pMesh, CFBX2Mesh* const pFbxMesh)
{ 
    // 본 정보
    pFbxMesh->m_vecBones.resize(pMesh->mNumBones);

    //  버텍스 수만큼 weight 공간 확보
    pFbxMesh->m_vecVertexWeights.resize(pMesh->mNumVertices);

    for (_uint i = 0; i < pMesh->mNumBones; i++)
    {
        aiBone* bone = pMesh->mBones[i];
        BONE_INFO& boneInfo = pFbxMesh->m_vecBones[i];

        boneInfo.sName = bone->mName.C_Str();

        aiMatrix4x4 m = bone->mOffsetMatrix;
        D3DXMatrixTranspose(&boneInfo.matOffset, (D3DXMATRIX*)&m);
        //memcpy(&boneInfo.OffsetMatrix, &m, sizeof(D3DXMATRIX));

        D3DXMatrixIdentity(&boneInfo.matFinalTransform);

        //  여기 중요
        for (UINT j = 0; j < bone->mNumWeights; j++)
        {
            aiVertexWeight weight = bone->mWeights[j];

            int vertexID = weight.mVertexId;
            float w = weight.mWeight;

            pFbxMesh->m_vecVertexWeights[vertexID].vecBoneIDs.push_back(i);
            pFbxMesh->m_vecVertexWeights[vertexID].vecWeights.push_back(w);
        }
    }
}

aiNodeAnim const* CAssetFbx2::FindNodeAnim(aiAnimation const* const pAni, std::string_view sNodeName)
{
    for (_uint i = 0; i < pAni->mNumChannels; i++)
    {
        if (sNodeName == pAni->mChannels[i]->mNodeName.C_Str())
            return pAni->mChannels[i];
    }
    return nullptr;
}


vector<wstring> CAssetFbx2::LoadMaterialTexture(aiMaterial const* const pMat, aiTextureType eAiTexType, aiScene const* const pScene)
{
    std::vector<wstring> textures;
    for (_uint i = 0; i < pMat->GetTextureCount(eAiTexType); i++) {
        aiString str;
        pMat->GetTexture(eAiTexType, i, &str);
        wstring sTexName = L"TexFBX_" + CHelper::StringToWString(str.C_Str());
       
        bool skip = false;
        if (CAssetMgr::GetInstance()->Get_Asset(sTexName)
            && !CAssetMgr::GetInstance()->Get_Asset(sTexName)->empty())
        {
            textures.push_back(sTexName);
            skip = true;
        }
        
        if (!skip) {  
            HRESULT hr;

            const aiTexture* embeddedTexture = pScene->GetEmbeddedTexture(str.C_Str());
            if (embeddedTexture != nullptr) {
                LoadEmbbedableTexture(embeddedTexture, sTexName);
                textures.push_back(sTexName);
            }
            else {
                // TODO: 일단 여기는 미구현
                // fbx의 텍스쳐들은 일단 임베드텍스쳐만 쓰자
                MSG_BOX("FBX 텍스쳐 임베드텍스쳐가 아님");
            }
        }
    }

    return textures;
}
HRESULT CAssetFbx2::LoadEmbbedableTexture( aiTexture const* const embeddedTexture, std::wstring_view svAssetName)
{
    if (embeddedTexture->mHeight != 0)
    {
        auto pAssetTex =CAssetTextureFromCreate::Create(embeddedTexture->mWidth, embeddedTexture->mHeight );
        D3DLOCKED_RECT lockedRect;
        pAssetTex->Get_Texture()->LockRect(0, &lockedRect, nullptr, 0);
        memcpy(lockedRect.pBits,
            embeddedTexture->pcData,
            embeddedTexture->mWidth * embeddedTexture->mHeight * 4);
        pAssetTex->Get_Texture()->UnlockRect(0);

        CAssetMgr::GetInstance()->AddAsset(svAssetName, pAssetTex);
        CAssetMgr::GetInstance()->LoadAsset(svAssetName);

        return S_OK;
    }

    // 압축
    const size_t size = embeddedTexture->mWidth;
    CAssetMgr::GetInstance()->AddAsset(svAssetName, CAssetTextureFromInMem::Create(embeddedTexture->pcData, size));
    CAssetMgr::GetInstance()->LoadAsset(svAssetName);
    
    return S_OK;
}