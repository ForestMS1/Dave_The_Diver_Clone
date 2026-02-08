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

HRESULT CParticleMgr::Ready_Particle(HWND hWnd)
{
	Firework* firework = Firework::Create();
	particles.push_back(firework);
	CGunShot* gunshot = CGunShot::Create();
	particles.push_back(gunshot);
	CBullet* bullet = CBullet::Create(m_pCamera);
	particles.push_back(bullet);
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
	/*vector <PSystem*>::iterator i;
	for (i = particles.begin(); i != particles.end();) {
		if ((*i)->empty == true) {
			Safe_Release(*i);
			i = particles.erase(i);
		}
		else {
			i++;
		}
	}*/
}

void CParticleMgr::Render_Particle()
{
	for (auto particle : particles) {
		particle->render();
	}
}

void CParticleMgr::spwan_Particle(PARTICLETYPE type, _vec3 origin, int numofPariticles)
{
	switch(type) {
	case FIREWORK: 
	{
		for (int i = 0; i < numofPariticles; i++) {
			particles[FIREWORK]->addParticle(origin);
		}
		/*Firework* firework = Firework::Create( origin, numofPariticles);
		temp.push_back(firework);*/
	}
		break;
	case GUNSHOT:
	{
		for (int i = 0; i < numofPariticles; i++) {
			particles[GUNSHOT]->addParticle(origin);
		}
		/*CGunShot* gunshot = CGunShot::Create(origin);
		temp.push_back(gunshot);*/
	}
	break;

	case BULLET:
	{
		for (int i = 0; i < numofPariticles; i++) {
			particles[BULLET]->addParticle(origin);
		}
	/*	CBullet* bullet = CBullet::Create(m_pCamera);
		temp.push_back(bullet);*/
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
