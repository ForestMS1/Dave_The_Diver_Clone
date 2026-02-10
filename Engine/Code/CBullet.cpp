#include "CBullet.h"
#include "CParticleMgr.h"
#include "CLog.h"

CBullet::CBullet( CCamera* camera) :  PSystem()
{
	m_pCamera = camera;
	_size = { 0.5f,0.5f,0.5f };
	_origin = { 0,0,0 };


	//AABB µð¹ö±ë¿ë ¸ÅÆ®¸¯½º 
	//matView = m_pCamera->Get_ViewMatrix();
	//matProj = m_pCamera->Get_ProjMatrix();
	//matFinal = matWorld * matView * matProj;
}



CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_Buffer()
{

	if (FAILED(Ready_Texture(L"Tex_Bullet", L"../Bin/Resource/Texture/sprBullet_", 1))) {
		return E_FAIL;
	}

	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;
	//addParticle();
	

	return S_OK;
}



CBullet* CBullet::Create(CCamera* camera)
{
	CBullet* bullet = new CBullet(camera);

	if (FAILED(bullet->Ready_Buffer()))
	{
		Safe_Release(bullet);
		MSG_BOX("CBullet Create Failed");
		return nullptr;
	}
	return bullet;
}


void CBullet::resetParticle(Attribute* attribute, D3DXCOLOR color)
{
	
	attribute->_isAlive = true;
	_vec3 cameraPos = m_pCamera->Get_Pos();
	_vec3 cameraDir =  m_pCamera->Get_At() - cameraPos;
	D3DXVec3Normalize(&cameraDir, &cameraDir);
	attribute->_position = cameraPos + cameraDir* 2.f;

	attribute->velocity = cameraDir * 20.f;
	attribute->_color = color;
	attribute->_age = 0.0f;
	attribute->_lifeTime = 0.5f;
}


void CBullet::render()
{

	
	// ½ÇÁ¦ ÃÑ¾Ë ·»´õ¸µ
	Set_Texture(L"Tex_Bullet", 0);

	PSystem::render();



}

void CBullet::preRender()
{
	PSystem::preRender();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);

}

void CBullet::postRender()
{
	PSystem::postRender();
	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);

}
void CBullet::update(float fTimeDelta)
{
	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
		i->_position += i->velocity * fTimeDelta;
		i->_age += fTimeDelta;
		//_origin = i->_position;
		
		if (i->_age > i->_lifeTime) {
			CParticleMgr::GetInstance()->spwan_Particle(GUNSHOT, i->_position, 5);
			i->_isAlive = false;
		}
	}
	removeDeadParticles();
	
}

void CBullet::Free()
{
	
	PSystem::Free();
}


