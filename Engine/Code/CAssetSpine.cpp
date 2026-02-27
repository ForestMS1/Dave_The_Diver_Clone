#include "CAssetSpine.h"
#include "CGraphicDev.h"
#include "CLog.h"
#include "CAssetTexture.h"
#include "CAssetMgr.h"
#include "CHelper.h"


class CAssetSpine::CMySpineTextureLoader : public spine::TextureLoader
{
public:
    CMySpineTextureLoader() : m_sSpineTexName({}) {}
public:
    void load(spine::AtlasPage& page, const spine::String& path) override
    {
        std::wstring s = CHelper::StringToWString(path.buffer());
        size_t pos = s.find_last_of(L"/\\");
        if (pos == std::wstring::npos)
        {
            m_sSpineTexName = L"SpineTex_" + s;
        }
        else
        {
            m_sSpineTexName = L"SpineTex_" + s.substr(pos + 1);
        }

        CAssetMgr::GetInstance()->AddAsset(m_sSpineTexName, CAssetTexture::Create(s));
        CAssetMgr::GetInstance()->LoadAsset(m_sSpineTexName);

        D3DXIMAGE_INFO const* pImgInfo = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetTexture>(m_sSpineTexName)->Get_ImgInfo();
        page.width = pImgInfo->Width;
        page.height = pImgInfo->Height;
    }
    void unload(void* texture) override
    {
        CAssetMgr::GetInstance()->DelAsset(m_sSpineTexName);
    }

public:
    wstring m_sSpineTexName;
};

HRESULT CAssetSpine::LoadFromFile(std::wstring_view atlasPath, std::wstring_view skeletonPath)
{
   
    m_pAtlas = make_unique<spine::Atlas>(std::string(atlasPath.begin(), atlasPath.end()).c_str(), m_pTexLoader.get());
    //auto pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    SkeletonBinary binary(m_pAtlas.get());

    // Optionally set the scale
    //binary.setScale(0.5f); // Scale skeleton to 50%

    // Load skeleton data from file
    m_pSkeletonData.reset(binary.readSkeletonDataFile(std::string(skeletonPath.begin(), skeletonPath.end()).c_str()));
    m_pAniStateData = make_unique<spine::AnimationStateData>(m_pSkeletonData.get());

    //for (size_t i = 0; i < m_pSkeletonData->getAnimations().size(); ++i) {
    //    spine::Animation* anim = m_pSkeletonData->getAnimations()[i];
    //    anim->getName().buffer();
    //    auto a = anim->getName();
    //    CLog::Debug(L"Animation %zu: %s\n", i, anim->getName().buffer());
    //    //printf("Animation %zu: %s\n", i, anim->getName().buffer());
    //}
    // Set the default mix time between any pair of animations in seconds
    //m_pAniStateData->setDefaultMix(0.1f);

    m_pSkeleton = make_unique<spine::Skeleton>(m_pSkeletonData.get());
    m_pAniState = make_unique<spine::AnimationState>(m_pAniStateData.get());

    // 기본 애니메이션 설정 테스트 (Idle이 있다면)
    //m_pAniState->setAnimation(0, "move", true);


    auto skeleton = m_pSkeleton.get();
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

CAssetSpine::CAssetSpine(std::wstring_view m_sAssetPath)
	: CAsset(m_sAssetPath)
    , m_pTexLoader(make_unique<CMySpineTextureLoader>())
    , m_iNumVertex(0)
    , m_iNumTri(0)
    //, m_pImpl(make_unique<CAssetSpine::Impl>())
{
}

CAssetSpine::~CAssetSpine()
{
}

HRESULT CAssetSpine::Load()
{
    m_eAssetState = LOADING;

    std::wstring atlasPath = m_sAssetPath + L".atlas.txt";
    std::wstring skelPath = m_sAssetPath + L".skel.bytes";
   
    if (FAILED(LoadFromFile(atlasPath, skelPath)))
    {
        m_eAssetState = LOADFAIL;
        wstring msg = L"CAssetSpine Load Fail, Path: " + m_sAssetPath;
        MSG_BOX_STR(msg.c_str());
        return E_FAIL;
    }

    m_eAssetState = LOADED;
	return S_OK;
}

HRESULT CAssetSpine::Unload()
{
    m_pAniState.reset();
    m_pAniStateData.reset();
    m_pSkeleton.reset();
    m_pSkeletonData.reset();
    m_pAtlas.reset();
    m_pTexLoader.reset();
   
    m_eAssetState = UNLOAD;
	return S_OK;
}

std::wstring_view CAssetSpine::Get_TextureString()
{
    return m_pTexLoader.get()->m_sSpineTexName;
}

void CAssetSpine::TempUpdate(const float& fTimeDelta)
{
    m_pAniState->update(fTimeDelta);
    m_pAniState->apply(*m_pSkeleton);
    m_pSkeleton->updateWorldTransform();
}



void CAssetSpine::TempRender()
{
    auto skeleton = m_pSkeleton.get();
    if (!skeleton) return;

    auto pDevice = CGraphicDev::GetInstance()->Get_GraphicDev();

     auto& drawOrder = skeleton->getDrawOrder();

     pDevice->SetFVF(FVF_COLTEX);

     int mycnt = 0;
     int myidxcnt = 0;
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
         std::vector<float> worldVertices(numFloats);
         mesh->computeWorldVertices(*slot, 0, numFloats, worldVertices.data(), 0, 2);
         int numVertices = numFloats / 2;

         mycnt += numVertices;

          auto& uvs = mesh->getUVs();
          auto& indices = mesh->getTriangles();

          myidxcnt += indices.size();
          
         IDirect3DTexture9* tex = static_cast<IDirect3DTexture9*>(mesh->getRendererObject());
         if (!tex) continue;

       
         std::vector<VTXCOLTEX> verts(numVertices);

         auto computeColor = [&](int v)
             {
                 float a = skeleton->getColor().a * slot->getColor().a * mesh->getColor().a;
                 float r = skeleton->getColor().r * slot->getColor().r * mesh->getColor().r;
                 float g = skeleton->getColor().g * slot->getColor().g * mesh->getColor().g;
                 float b = skeleton->getColor().b * slot->getColor().b * mesh->getColor().b;
                 return D3DCOLOR_ARGB((BYTE)(a * 255), (BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
             };

         for (int v = 0; v < numVertices; ++v)
         {
             verts[v].vPosition.x = worldVertices[v * 2 + 0];
             verts[v].vPosition.y = worldVertices[v * 2 + 1];
             verts[v].vPosition.z = 0.0f;
             verts[v].dwColor = computeColor(v);
             verts[v].vTexUV.x = uvs[v * 2 + 0];
             verts[v].vTexUV.y = uvs[v * 2 + 1];
         }
         
         pDevice->SetTexture(0, CAssetMgr::GetInstance()->Get_AssetFirst<CAssetTexture>(m_pTexLoader->m_sSpineTexName)->Get_Texture());


         // BlendMode에 따라 Render State 설정
         
         spine::BlendMode blend = slot->getData().getBlendMode();
         switch (blend)
         {
         case spine::BlendMode_Normal:
             pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
             pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
             break;
         case spine::BlendMode_Additive:
             pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
             pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
             break;
         case spine::BlendMode_Multiply:
             pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
             pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
             break;
         case spine::BlendMode_Screen:
             pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
             pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
             break;
         }

         pDevice->DrawIndexedPrimitiveUP(
             D3DPT_TRIANGLELIST,
             0,
             numVertices,
             indices.size() / 3,
             indices.buffer(),
             D3DFMT_INDEX16,
             verts.data(),
             sizeof(VTXCOLTEX)
         );
     }

     int x = 0;
}

CAssetSpine* CAssetSpine::Create(std::wstring_view m_sAssetPath)
{
	return new CAssetSpine{ m_sAssetPath };
}

void CAssetSpine::TEMP_LOCK_BUFFER(LPDIRECT3DVERTEXBUFFER9 pVB, LPDIRECT3DINDEXBUFFER9 pIB)
{
    auto skeleton = m_pSkeleton.get();
    if (!skeleton) return;

    auto pDevice = CGraphicDev::GetInstance()->Get_GraphicDev();


    VTXCOLTEX* pVertex;
    INDEX16* pIndex;
    pVB->Lock(0, m_iNumVertex * sizeof(VTXCOLTEX), (void**)&pVertex, D3DLOCK_DISCARD);
    pIB->Lock(0, m_iNumTri * sizeof(INDEX16), (void**)&pIndex, D3DLOCK_DISCARD);

    int vOffset = 0;
    int iOffset = 0;


    auto& drawOrder = skeleton->getDrawOrder();
    for (size_t i = 0; i < drawOrder.size(); ++i)
    {
        

        spine::Slot* slot = drawOrder[i];
        if (!slot) continue;

        spine::Attachment* attachment = slot->getAttachment();
        if (!attachment) continue;

        if (!attachment->getRTTI().isExactly(spine::MeshAttachment::rtti))
            continue;

        auto mesh = static_cast<spine::MeshAttachment*>(attachment);

        int numFloats = mesh->getWorldVerticesLength();
        std::vector<float> worldVertices(numFloats);
        mesh->computeWorldVertices(*slot, 0, numFloats, worldVertices.data(), 0, 2);
        int numVertices = numFloats / 2;

        auto& uvs = mesh->getUVs();
        auto& indices = mesh->getTriangles();


        auto computeColor = [&](int v)
            {
                float a = skeleton->getColor().a * slot->getColor().a * mesh->getColor().a;
                float r = skeleton->getColor().r * slot->getColor().r * mesh->getColor().r;
                float g = skeleton->getColor().g * slot->getColor().g * mesh->getColor().g;
                float b = skeleton->getColor().b * slot->getColor().b * mesh->getColor().b;
                return D3DCOLOR_ARGB((BYTE)(a * 255), (BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
            };
        // Vertex 채우기
        for (int v = 0; v < numVertices; ++v)
        {
            int idx = vOffset + v;
            pVertex[idx].vPosition.x = worldVertices[v * 2 + 0];
            pVertex[idx].vPosition.y = worldVertices[v * 2 + 1];
            pVertex[idx].vPosition.z = 0.0f;
            pVertex[idx].dwColor = computeColor(v);
            pVertex[idx].vTexUV.x = uvs[v * 2 + 0];
            pVertex[idx].vTexUV.y = uvs[v * 2 + 1];
        }



        // Index 채우기 (vOffset 보정)
        size_t triCount = indices.size() / 3;
        for (size_t t = 0; t < triCount; ++t)
        {
            pIndex[iOffset + t]._0 = indices[t * 3 + 0] + vOffset;
            pIndex[iOffset + t]._1 = indices[t * 3 + 1] + vOffset;
            pIndex[iOffset + t]._2 = indices[t * 3 + 2] + vOffset;
        }
        iOffset += static_cast<int>(triCount);


        vOffset += numVertices;
        //iOffset += static_cast<int>(indices.size());
       
    }
    

    pVB->Unlock();
    pIB->Unlock();
}

void CAssetSpine::Set_AniState(std::wstring_view svAniName)
{
    m_pAniState->setAnimation(0, CHelper::WStringToString(svAniName).c_str(), true);
}

std::wstring CAssetSpine::Get_TextureName() const
{
    return m_pTexLoader->m_sSpineTexName;
}

void CAssetSpine::Free()
{
    CAsset::Free();
    Unload();
}
