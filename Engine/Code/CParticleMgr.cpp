#include "CParticleMgr.h"
#include "Firework.h"
#include "CGunShot.h"
#include "CBullet.h"
#include "CWeather.h"


IMPLEMENT_SINGLETON(CParticleMgr)

CParticleMgr::CParticleMgr(): m_pCamera(nullptr)
{
	particles.resize(0);
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
	CWeather* weather = CWeather::Create(500);
	particles.push_back(weather);
	return S_OK;
}

void CParticleMgr::Update_Particle(float fTimeDelta)
{
	
	if (!particles.empty()) {

		for (auto particle : particles) {
			
			particle->update(fTimeDelta);
			
		}
	}

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
			particles[FIREWORK]->addParticle(origin, {1,1,1,1});
		}

	}
		break;
	case GUNSHOT:
	{
		for (int i = 0; i < numofPariticles; i++) {
			particles[GUNSHOT]->addParticle(origin, {1,1,1,1});
		}

	}
	break;

	case BULLET:
	{
		for (int i = 0; i < numofPariticles; i++) {
			particles[BULLET]->addParticle(origin,{1,1,1,1});
		}

	}
	break;
	}
}

void CParticleMgr::spwan_Weather(WEATHERTYPE type, _vec3 origin, int numofPariticles, D3DXCOLOR color)
{
	switch (type) {
	case SNOW:
		static_cast<CWeather*>(particles[WEATHER])->Set_Spawning(true);
		particles[WEATHER]->reset(origin, { 1,1,1,1 });
		break;
	case RAIN:
		static_cast<CWeather*>(particles[WEATHER])->Set_Spawning(true);
		particles[WEATHER]->reset(origin, { 0,0,1,1 });
		break;
	case DUST:
		static_cast<CWeather*>(particles[WEATHER])->Set_Spawning(true);
		particles[WEATHER]->reset(origin, { 1.f,0.67f,0.25f,1 });
		break;
	}
}

void CParticleMgr::Free()
{

	for (auto particle : particles) {
		particle->Free();
		Safe_Release(particle);
	}
}
