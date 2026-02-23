#include "CSpine.h"
#include "CGraphicDev.h"
#include "CHelper.h"
#include "CDynamicBuffer.h"
#include "CAssetMgr.h"
#include "CAssetSpine.h"
CSpine::CSpine()
    :CComponent()
    , m_pSkeleton(nullptr)
    , m_pAniState(nullptr)
{
}

CSpine::CSpine(const CSpine& rhs)
    :CComponent(rhs)
    , m_pSkeleton(nullptr)
    , m_pAniState(nullptr)
{
}

CSpine::~CSpine()
{
}

_int CSpine::Update_Component(const _float& fTimeDelta)
{
    m_pAniState->update(fTimeDelta);
    m_pAniState->apply(*m_pSkeleton);
    m_pSkeleton->updateWorldTransform();

    return NOEVENT;
}

void CSpine::Set_AniState(std::wstring_view svAniName)
{
    m_pAniState->setAnimation(0, CHelper::WStringToString(svAniName).c_str(), true);
}

void CSpine::Vertex_Buffer_Lock(LPDIRECT3DVERTEXBUFFER9 pVB)
{
    auto skeleton = m_pSkeleton;
    if (!skeleton) return;

    VTXCOLTEX* pVertex;
    pVB->Lock(0, m_iNumVertex * sizeof(VTXCOLTEX), (void**)&pVertex, D3DLOCK_DISCARD);

    int vOffset = 0;
    auto& drawOrder = skeleton->getDrawOrder();

    // 미리 할당된 버퍼를 사용하여 vector 생성 비용 절감
    static std::vector<float> worldVertices;

    for (size_t i = 0; i < drawOrder.size(); ++i)
    {
        spine::Slot* slot = drawOrder[i];
        spine::Attachment* attachment = slot->getAttachment();
        if (!attachment || !attachment->getRTTI().isExactly(spine::MeshAttachment::rtti))
            continue;

        auto mesh = static_cast<spine::MeshAttachment*>(attachment);
        int numFloats = mesh->getWorldVerticesLength();

        if (worldVertices.size() < (size_t)numFloats) worldVertices.resize(numFloats);
        mesh->computeWorldVertices(*slot, 0, numFloats, worldVertices.data(), 0, 2);

        int numVertices = numFloats / 2;
        auto& uvs = mesh->getUVs();

        // 컬러 계산 (루프 밖으로 뺄 수 있는 부분 최적화)
        D3DCOLOR finalColor = D3DCOLOR_ARGB(
            (BYTE)(skeleton->getColor().a * slot->getColor().a * mesh->getColor().a * 255),
            (BYTE)(skeleton->getColor().r * slot->getColor().r * mesh->getColor().r * 255),
            (BYTE)(skeleton->getColor().g * slot->getColor().g * mesh->getColor().g * 255),
            (BYTE)(skeleton->getColor().b * slot->getColor().b * mesh->getColor().b * 255)
        );

        for (int v = 0; v < numVertices; ++v)
        {
            int idx = vOffset + v;
            pVertex[idx].vPosition.x = worldVertices[v * 2 + 0];
            pVertex[idx].vPosition.y = worldVertices[v * 2 + 1];
            pVertex[idx].vPosition.z = 0.0f;
            pVertex[idx].dwColor = finalColor;
            pVertex[idx].vTexUV.x = uvs[v * 2 + 0];
            pVertex[idx].vTexUV.y = uvs[v * 2 + 1];
        }
        vOffset += numVertices;
    }
    pVB->Unlock();
}

void CSpine::Index_Buffer_Lock(LPDIRECT3DINDEXBUFFER9 pIB)
{
    auto skeleton = m_pSkeleton;
    if (!skeleton) return;

    INDEX16* pIndex;
    // 인덱스 버퍼는 고정된 경우가 많으므로 D3DLOCK_DISCARD 대신 0을 쓰기도 하지만, 
    // 여기서는 매번 새로 채운다는 가정하에 작성합니다.
    pIB->Lock(0, m_iNumTri * sizeof(INDEX16), (void**)&pIndex, 0);

    int vOffset = 0;
    int iOffset = 0;

    auto& drawOrder = skeleton->getDrawOrder();
    for (size_t i = 0; i < drawOrder.size(); ++i)
    {
        spine::Slot* slot = drawOrder[i];
        spine::Attachment* attachment = slot->getAttachment();
        if (!attachment || !attachment->getRTTI().isExactly(spine::MeshAttachment::rtti))
            continue;

        auto mesh = static_cast<spine::MeshAttachment*>(attachment);
        auto& indices = mesh->getTriangles();
        int numVertices = mesh->getWorldVerticesLength() / 2;

        size_t triCount = indices.size() / 3;
        for (size_t t = 0; t < triCount; ++t)
        {
            pIndex[iOffset + t]._0 = indices[t * 3 + 0] + vOffset;
            pIndex[iOffset + t]._1 = indices[t * 3 + 1] + vOffset;
            pIndex[iOffset + t]._2 = indices[t * 3 + 2] + vOffset;
        }

        iOffset += static_cast<int>(triCount);
        vOffset += numVertices;
    }
    pIB->Unlock();
}

HRESULT CSpine::Ready_Spine()
{
    return S_OK;
}

void CSpine::Render(CDynamicBuffer* pDynamicBuffer)
{
    if (auto pAssSpine = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetSpine>(m_sSpineAssetName))
    {
        auto pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
        pGraphicDev->SetTexture(0, CAssetMgr::GetInstance()->Get_AssetFirst<CAssetTexture>(pAssSpine->Get_TextureName())->Get_Texture());
        Vertex_Buffer_Lock(pDynamicBuffer->Get_VertexBuffer());
        pDynamicBuffer->Render_Buffer();
    }
}

HRESULT CSpine::Ready_AfterCreate(
    spine::SkeletonData * pSkeletonData, 
    spine::AnimationStateData * pAnimationStateData,
    std::wstring_view svSpineAssetName
)
{
    m_sSpineAssetName = svSpineAssetName;
    pAnimationStateData->setDefaultMix(0.1f);
#ifdef new
#undef new  
#endif
    m_pSkeleton = new spine::Skeleton(pSkeletonData);
    m_pAniState = new spine::AnimationState(pAnimationStateData);
#ifdef _DEBUG
#define new DBG_NEW
#endif
    
    auto skeleton = m_pSkeleton;
    auto& drawOrder = skeleton->getDrawOrder();

    int myvtxcnt = 0;
    int mytricnt = 0;
    int failcnt = 0;
    for (size_t i = 0; i < drawOrder.size(); ++i)
    {
        spine::Slot* slot = drawOrder[i];
        if (!slot) continue;

        spine::Attachment* attachment = slot->getAttachment();
        if (!attachment) continue;

        if (!attachment->getRTTI().isExactly(spine::MeshAttachment::rtti))
        {
            ++failcnt;
            continue;
        }

        auto mesh = static_cast<spine::MeshAttachment*>(attachment);

        int numFloats = mesh->getWorldVerticesLength();
        int numVertices = numFloats / 2;

        myvtxcnt += numVertices;

        auto& indices = mesh->getTriangles();

        mytricnt += indices.size();
    }
    m_iNumVertex = myvtxcnt;
    m_iNumTri = mytricnt / 3;
    return S_OK;
}

CSpine* CSpine::Create()
{
    CSpine* pSpine = new CSpine;

    if (FAILED(pSpine->Ready_Spine()))
    {
        Safe_Release(pSpine);
        MSG_BOX("pSpine Create Failed");
        return nullptr;
    }

    return pSpine;
}

CComponent* CSpine::Clone()
{
    return new CSpine(*this);
}

void CSpine::Free()
{
    CComponent::Free();
    Safe_Delete(m_pSkeleton);
    Safe_Delete(m_pAniState);
}