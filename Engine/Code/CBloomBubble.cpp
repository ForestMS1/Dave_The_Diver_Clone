#include "CBloomBubble.h"
#include "CParticleMgr.h"

CBloomBubble::CBloomBubble()
{
	_size = { 0.1f,0.1f,0.1f };
	_origin = { 0,0,0 };
}

CBloomBubble::~CBloomBubble()
{
}

HRESULT CBloomBubble::Ready_Buffer()
{


	if (FAILED(PSystem::Ready_Buffer()))
		return E_FAIL;


	return S_OK;
}


CBloomBubble* CBloomBubble::Create()
{
	CBloomBubble* pBloomBubble = new CBloomBubble();

	if (FAILED(pBloomBubble->Ready_Buffer()))
	{
		Safe_Release(pBloomBubble);
		MSG_BOX("pBloomBubble Create Failed");
		return nullptr;
	}
	return pBloomBubble;
}

void CBloomBubble::resetParticle(Attribute* attribute, D3DXCOLOR color)
{

	float Random = GetRandomFloat(-1.f, 1.f);
	attribute->_position = _hitPosition;
	attribute->_position.x += Random;


	Random = GetRandomFloat(-1.f, 1.f);
	attribute->_position.y += Random;
	attribute->_isAlive = true;


	_vec3 min = { -0.1f, 0.1f,0.f };
	_vec3 max = { 0.1f, 0.1f,0.f };

	GetRandomVector(&attribute->velocity, &min, &max);

	attribute->velocity *= 1.f;
	attribute->_color = { 1.f, 1.f, 1.f, 1.f };
	attribute->_age = 0.f;
	attribute->_lifeTime = 50.f;


}

void CBloomBubble::render()
{
	Set_Texture(L"Tex_BloomBubble", 0);
	preRender();

	__super::render();

	postRender();
}
void CBloomBubble::preRender()
{

	__super::preRender();
	

	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // ¶Ç´Â ONE
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void CBloomBubble::postRender()
{
	__super::postRender();
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);


}

void CBloomBubble::update(float fTimeDelta)
{
	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
		const float gravity = 1.2f;
		//i->velocity.y -= gravity * fTimeDelta;
		i->_position += i->velocity * fTimeDelta;
		i->_age += fTimeDelta;
		
	


		if (i->_age > (i->_lifeTime)) {
			i->_isAlive = false;
		}
	}
	removeDeadParticles();

}

void CBloomBubble::Free()
{
	PSystem::Free();
}
