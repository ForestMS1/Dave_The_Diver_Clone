#include "CParticleMgr.h"
#include "Firework.h"
#include "CGunShot.h"
#include "CBullet.h"

IMPLEMENT_SINGLETON(CParticleMgr)

CParticleMgr::CParticleMgr(): m_pCamera(nullptr)
{
	particles.resize(0);
	temp.resize(0);
}

CParticleMgr::~CParticleMgr()
{
	Free();
}

HRESULT CParticleMgr::Ready_Particle(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev)
{
	
	return S_OK;
}

void CParticleMgr::Update_Particle(float fTimeDelta)
{
	if (!temp.empty()) {
		for (auto &particle : temp) {
			particles.push_back(particle);

		}
		temp.clear();
	}



	
	if (!particles.empty()) {

		for (auto particle : particles) {
			
			particle->update(fTimeDelta);
			
		}
	}
	vector <PSystem*>::iterator i;
	for (i = particles.begin(); i != particles.end();) {
		if ((*i)->empty == true) {
			Safe_Release(*i);
			i = particles.erase(i);
		}
		else {
			i++;
		}
	}
}

void CParticleMgr::Render_Particle(LPDIRECT3DDEVICE9 pGraphicDev)
{
	for (auto particle : particles) {
		particle->render();
	}
}

void CParticleMgr::spwan_Particle(LPDIRECT3DDEVICE9 pGraphicDev,PARTICLETYPE type, _vec3 origin, int numofPariticles)
{
	switch(type) {
	case FIREWORK: 
	{
		Firework* firework = Firework::Create(pGraphicDev, origin, numofPariticles);
		temp.push_back(firework);
	}
		break;
	case GUNSHOT:
	{
		CGunShot* gunshot = CGunShot::Create(pGraphicDev, origin);
		temp.push_back(gunshot);
	}
	break;

	case BULLET:
	{
		CBullet* bullet = CBullet::Create(pGraphicDev,m_pCamera);
		temp.push_back(bullet);
	}
	break;
	}
}

void CParticleMgr::Free()
{
	for (auto particle : temp) {
		particle->Free();
		Safe_Release(particle);
	}
	for (auto particle : particles) {
		particle->Free();
		Safe_Release(particle);
	}
}
