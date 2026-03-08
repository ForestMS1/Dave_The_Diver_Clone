#include "CLightMgr.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr()
{
}

CLightMgr::~CLightMgr()
{
    Free();
}

void CLightMgr::UpdateLight() {
    for (auto list : m_LightList) {
        for (auto Light : list) {
            if (list.size() != 0) {
                Light->Ready_Light(Light->Get_Index());
            }
        }
      
        
    }
}

void CLightMgr::OffLight() {
    for (auto list : m_LightList) {
        for (auto Light : list) {
            if (list.size() != 0) {
                Light->Off_Light();
            }
        }
   
    }
}

void CLightMgr::ResetLight() {
    for (auto list : m_LightList) {
        for_each(list.begin(), list.end(), CDeleteObj());
        list.clear();
    }
}

HRESULT CLightMgr::Add_Light(CLight* Light, LIGHTID _ID)
{
    m_LightList[_ID].push_back(Light);
    return S_OK;
}

void CLightMgr::Free()
{
    for (auto list : m_LightList) {
        for_each(list.begin(), list.end(), CDeleteObj());
        list.clear();
    }
  
}
