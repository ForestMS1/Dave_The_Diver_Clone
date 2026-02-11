#include "CUIMgr.h"
//#include <yoga/Yoga.h>
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetDefaultFont.h"
#include "CInfoMgr.h"

IMPLEMENT_SINGLETON(CUIMgr)


CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{

}

void CUIMgr::Free()
{

}
HRESULT CUIMgr::Ready()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    if (FAILED(D3DXCreateSprite(pGraphicDev, &m_pSprite)))
    {
        wstring msg = L"CAssetDefaultFont Sprite Create Failed, Path: " ;
        MSG_BOX_STR(msg.c_str());
        return E_FAIL;
    }

    return S_OK;
}
void CUIMgr::Test()
{
    /*
    YGConfigRef config = YGConfigNew();
    YGConfigSetUseWebDefaults(config, false);

    YGNodeRef root = YGNodeNewWithConfig(config);
    YGNodeStyleSetWidth(root, 350);
    YGNodeStyleSetHeight(root, 350);
    YGNodeStyleSetPadding(root, YGEdgeAll, 20);

    for (int i = 0; i < 4; i++) {
        YGNodeRef child = YGNodeNewWithConfig(config);
        YGNodeStyleSetFlex(child, 1);
        YGNodeInsertChild(root, child, i);
    }

    YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

    // 결과 출력
    for (int i = 0; i < 4; i++) {
        YGNodeRef child = YGNodeGetChild(root, i);
        std::cout << "Child " << i
            << " x: " << YGNodeLayoutGetLeft(child)
            << " y: " << YGNodeLayoutGetTop(child)
            << " w: " << YGNodeLayoutGetWidth(child)
            << " h: " << YGNodeLayoutGetHeight(child)
            << std::endl;

        float x = YGNodeLayoutGetLeft(child);
        float y = YGNodeLayoutGetTop(child);
        float w = YGNodeLayoutGetWidth(child);
        float h = YGNodeLayoutGetHeight(child);
        int xx = 0;
    }

    YGNodeFreeRecursive(root);
    YGConfigFree(config);
    
    */
    
}
void CUIMgr::Render()
{
    //Tex_Button Tex_White1PX Tex_Player
    if (auto pButtonTex = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetTexture>(L"Tex_Button"))
    {
        m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
        D3DSURFACE_DESC desc;
        pButtonTex->Get_Texture()->GetLevelDesc(0, &desc);
        float fWidth = desc.Width * 1.f ;
        float fHeight = desc.Height * 1.f;
       
        float screenwidth = WINCX * float(CInfoMgr::GetInstance()->Get_ScaleFactor());
        float screenheight = WINCY * float(CInfoMgr::GetInstance()->Get_ScaleFactor());
        float rewidth = screenwidth / 2.f - fWidth / 2.f;
        float reheight = screenheight / 2.f - fHeight / 2.f;

        auto info = pButtonTex->Get_ImgInfo();
        float fov = pButtonTex->Get_ImgInfo()->Width / float(pButtonTex->Get_ImgInfo()->Height);
        _vec3 vPos = { rewidth , reheight , 0.f };
        _matrix mat, matScale, matWorld;
        D3DXMatrixTranslation(&mat, rewidth, reheight, 0.f);
        D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
        matWorld = matScale * mat;
        m_pSprite->SetTransform(&matWorld);
        m_pSprite->Draw(pButtonTex->Get_Texture(), nullptr, nullptr, nullptr, 0xFFFFFFFF);
        //m_pSprite->Draw(pButtonTex->Get_Texture(), nullptr, nullptr, nullptr, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

        //_vec2 pp = {pv.x, pv.y};
        CAssetDefaultFont* pDefFont3 = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Griun");

        _matrix mati;
        D3DXMatrixIdentity(&mati);
        m_pSprite->SetTransform(&mati);
        float movedx =  mat.m[3][0];
        float movedy = mat.m[3][1];
        
        RECT rc{};
        rc.left = movedx;
        rc.top = movedy;
        rc.right = movedx + (fWidth );
        rc.bottom = movedy + (fHeight );
        pDefFont3->Get_Font()->DrawTextW(m_pSprite, L"sssasdf", -1, &rc, DT_NOCLIP | DT_VCENTER | DT_CENTER , 0xFFFFFFFF);

        m_pSprite->End();
    }
}