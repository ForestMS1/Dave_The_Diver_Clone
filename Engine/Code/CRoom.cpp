#include "CRoom.h"


CRoom::CRoom(LPDIRECT3DDEVICE9 pGraphicDev) : m_pGraphicDev(pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CRoom::CRoom(LPDIRECT3DDEVICE9 pGraphicDev, const CScene* pScene) : m_pGraphicDev(pGraphicDev), m_pScene(pScene)
{
    m_pGraphicDev->AddRef();
}



CRoom::CRoom(const CRoom& rhs) : m_pGraphicDev(rhs.m_pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CRoom::~CRoom()
{
}



void CRoom::Free()
{
    Safe_Release(m_pGraphicDev);
}

