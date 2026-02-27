#include "CParticleMgr.h"
#include "Firework.h"
#include "CBulletSpark.h"
#include "CBullet.h"
#include "CWeather.h"
#include "CBubble.h"
#include "CFlameShot.h"
#include "CAssetMgr.h"
#include "CManagement.h"
#include "CSeaBubble.h"
#include "CBlood.h"

IMPLEMENT_SINGLETON(CParticleMgr)

CParticleMgr::CParticleMgr(): m_pCamera(nullptr), m_pPlayer(nullptr)
{
	cameraOn = false;

}

CParticleMgr::~CParticleMgr()
{
	Free();
}

HRESULT CParticleMgr::Ready_Particle(HWND hWnd)
{
	particles.resize(PARTICLE_END);

	CBubble* bubble = CBubble::Create(m_pPlayer);
	particles[PARTICLE_BUBBLE]=(bubble);

	CBlood* blood = CBlood::Create();
	particles[PARTICLE_BLOOD]=(blood);

	CSeaBubble* SeaBubble = CSeaBubble::Create(m_pPlayer);
	particles[PARTICLE_SEABUBBLE]=(SeaBubble);
	//CAssetMgr::GetInstance()->LoadAsset();
	return S_OK;
}

void CParticleMgr::Update_Particle(float fTimeDelta)
{

	if (!particles.empty()) {

		for (auto particle : particles) {
			if (particle) {
				particle->update(fTimeDelta);
			}
			
			
		}
	}

}

void CParticleMgr::Render_Particle()
{
	for (auto particle : particles) {
		if (particle) {

			particle->render();
		}
	}
}

void CParticleMgr::spwan_Particle(PARTICLETYPE type, _vec3 origin, int numofPariticles)
{
	switch(type) {

	case PARTICLE_BUBBLE:
		for (int i = 0; i < numofPariticles; i++) {
			particles[PARTICLE_BUBBLE]->addParticle(origin, { 1,1,1,1 });
		}
		break;
	case PARTICLE_BLOOD:
		for (int i = 0; i < numofPariticles; i++) {
			particles[PARTICLE_BLOOD]->addParticle(origin, { 1,1,1,1 });
		}
		break;


	
	break;
	}
}

void CParticleMgr::spwan_Weather(WEATHERTYPE type, _vec3 origin, int numofPariticles, D3DXCOLOR color)
{
	_vec3 tempCenter, extents;
	tempCenter = { 0.f,0.f,0.f };
	extents = { 10.f,10.f,10.f };
	switch (type) {
	case SNOW:
		static_cast<CWeather*>(particles[WEATHER])->Set_Spawning(true);
		particles[WEATHER]->reset(origin, tempCenter,extents,{ 1,1,1,1 });
		break;
	case RAIN:
		static_cast<CWeather*>(particles[WEATHER])->Set_Spawning(true);
		particles[WEATHER]->reset(origin,  tempCenter, extents,{ 0,0,1,1 });
		break;
	case DUST:
		static_cast<CWeather*>(particles[WEATHER])->Set_Spawning(true);
		particles[WEATHER]->reset(origin,  tempCenter, extents,{ 1.f,0.67f,0.25f,1 });
		break;
	}
}

void CParticleMgr::Clear_Particle()
{
	for (auto particle : particles) {
		if (particle) {
			Safe_Delete(particle);
		}
	}
	particles.clear();
}

void CParticleMgr::Free()
{

	for (auto particle : particles) {
		if (particle) {
			particle->Free();
			Safe_Release(particle);
		}
	
	}
}
