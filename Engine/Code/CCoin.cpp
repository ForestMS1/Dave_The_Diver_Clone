#include "CCoin.h"
#include "CParticleMgr.h"

CCoin::CCoin()
{
	_size = { 0.3,0.3,0.3 };
	_origin = { 0,0,0 };
}

CCoin::~CCoin()
{
}

HRESULT CCoin::Ready_Buffer()
{


	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;


	return S_OK;
}


CCoin* CCoin::Create()
{
	CCoin* pCoin = new CCoin();

	if (FAILED(pCoin->Ready_Buffer()))
	{
		Safe_Release(pCoin);
		MSG_BOX("pCoin Create Failed");
		return nullptr;
	}
	return pCoin;
}

void CCoin::resetParticle(Attribute* attribute, D3DXCOLOR color)
{

	float Random = GetRandomFloat(-1.f, 1.f);
	attribute->_position = _hitPosition;
	attribute->_position.x += Random;

	Random = GetRandomFloat(0.f, 0.2f);
	attribute->_position.y += Random;
	attribute->_isAlive = true;


	_vec3 min = { -0.5f, 0.f,0.f };
	_vec3 max = { 0.5f, 0.f,0.f };

	GetRandomVector(&attribute->velocity, &min, &max);
	attribute->velocity.y = 0.35f;

	attribute->velocity *= 0.5f;
	attribute->_color = { 1.f, 1.f, 1.f, 1.f };
	attribute->_age = 0.f;
	attribute->_lifeTime = 1.5f;
}

void CCoin::render()
{
	Set_Texture(L"Tex_Coin", 0);
	preRender();

	__super::render();

	postRender();
}
void CCoin::preRender()
{

	__super::preRender();
	// Color = Texture * Diffuse
	_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


}

void CCoin::postRender()
{
	__super::postRender();
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

void CCoin::update(float fTimeDelta)
{
	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
		const float gravity = 1.2f;            
		i->velocity.y -= gravity * fTimeDelta; 
		i->_position += i->velocity * fTimeDelta;
		i->_age += fTimeDelta;
		//_origin = i->_position;
	
		if (i->_age < 1.f) {
			i->_color -= D3DXCOLOR(0.f, 0.f, 0.f, 0.01f);
			i->velocity.y += 0.01f;
		}
	

		
		if (i->_age > (i->_lifeTime)) {
			i->_isAlive = false;
		}
	}
	removeDeadParticles();

}

void CCoin::Free()
{
	PSystem::Free();
}
