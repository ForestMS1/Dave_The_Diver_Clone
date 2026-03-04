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
#include "CCoin.h"
#include "CBloomBubble.h"
#include "CMapMgr.h"
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
	preparticles.resize(PRE_PARTICLE_END);
	postparticles.resize(POST_PARTICLE_END);

	CBubble* bubble = CBubble::Create(m_pPlayer);
	particles[PARTICLE_BUBBLE]=(bubble);


	CSeaBubble* SeaBubble = CSeaBubble::Create(m_pPlayer);
	postparticles[POST_PARTICLE_SEABUBBLE] = (SeaBubble);

	CBloomBubble* bloombubble = CBloomBubble::Create();
	postparticles[POST_PARTICLE_BLOOMBUBBLE] = (bloombubble);


	CBlood* blood = CBlood::Create();
	postparticles[POST_PARTICLE_BLOOD] = (blood);

	CCoin* coin = CCoin::Create();
	postparticles[POST_PARTICLE_COIN] = (coin);

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
	if (!postparticles.empty()) {

		for (auto particle : postparticles) {
			if (particle) {
				particle->update(fTimeDelta);
			}
		}
	}


}

void CParticleMgr::Render_Particle()
{
	if (m_bRenderOn) {
		for (auto particle : particles) {
			if (particle) {

				particle->render();
			}
		}

	}

}


void CParticleMgr::PreRender_Particle()
{
}

void CParticleMgr::PostRender_Particle()
{
	if (m_bRenderOn) {
		for (auto particle : postparticles) {
			if (particle) {

				particle->render();
			}
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
	case PARTICLE_BLOOMBUBBLE:
		for (int i = 0; i < numofPariticles; i++) {
			postparticles[POST_PARTICLE_BLOOMBUBBLE]->addParticle(origin, { 1,1,1,1 });
		}
		break;
	case PARTICLE_BLOOD:
		for (int i = 0; i < numofPariticles; i++) {
			postparticles[POST_PARTICLE_BLOOD]->addParticle(origin, { 1,1,1,1 });
		}
		break;
	case PARTICLE_COIN:
		for (int i = 0; i < numofPariticles; i++) {
			postparticles[POST_PARTICLE_COIN]->addParticle(origin, { 1,1,1,1 });
		}
		break;
	case PARTICLE_SEABUBBLE:
		for (int i = 0; i < numofPariticles; i++) {
			postparticles[POST_PARTICLE_SEABUBBLE]->addParticle(origin, { 1,1,1,0 });
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
		if (particle != nullptr) {
			list<Attribute>* atrributes = particle->GetAtrribute();

			for (auto i = atrributes->begin(); i != atrributes->end(); i++) {
				const float gravity = 1.2f;
				i->_isAlive = false;
			}


			auto j = atrributes->begin();

			while (j != atrributes->end()) {
				if (j->_isAlive == false) {
					j = atrributes->erase(j);
				}
				else {
					j++;
				}
			}
		}
		
	}
	for (auto particle : postparticles) {
		if (particle != nullptr) {
			list<Attribute>* atrributes = particle->GetAtrribute();

			for (auto i = atrributes->begin(); i != atrributes->end(); i++) {
				const float gravity = 1.2f;
				i->_isAlive = false;
			}


			auto j = atrributes->begin();

			while (j != atrributes->end()) {
				if (j->_isAlive == false) {
					j = atrributes->erase(j);
				}
				else {
					j++;
				}
			}
		}

	}


	m_bRenderOn = false;
}

void CParticleMgr::Free()
{
	
	for (auto particle : particles) {
		if (particle) {
			particle->Free();
			Safe_Release(particle);
		}
	
	}
	particles.clear();

	for (auto particle : postparticles) {
		if (particle) {
			particle->Free();
			Safe_Release(particle);
		}

	}
	postparticles.clear();
}
