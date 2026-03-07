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
	attribute->_position = _hitPosition;

	float Random = GetRandomFloat(-1.f, 1.f);
	attribute->_position.x += Random;


	Random = GetRandomFloat(-1.f, 1.f);
	attribute->_position.y += Random;


	attribute->_isAlive = true;

	_vec3 min{};
	_vec3 max{};
	if (_hitPosition.x >= attribute->_position.x && _hitPosition.y < attribute->_position.y) {
		min = { 0.f, -1.f,0.f };
		max = { 1.f, 0.f,0.f };
	}
	if (_hitPosition.x < attribute->_position.x && _hitPosition.y < attribute->_position.y) {
		min = { -1.f, -1.f,0.f };
		max = { 0.f, 0.f,0.f };
	}

	if (_hitPosition.x >= attribute->_position.x && _hitPosition.y >= attribute->_position.y) {
		min = { 0.f, 0.f,0.f };
		max = { 1.f, 1.f,0.f };
	}
	if (_hitPosition.x < attribute->_position.x && _hitPosition.y >= attribute->_position.y) {
		min = { -1.f, 0.f,0.f };
		max = { 0.f, 1.f,0.f };
	}


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

	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // 또는 ONE
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
	for (auto& p : _particles)
	{
		if (!p._isAlive)
			continue;

		// 시간 누적
		p._age += fTimeDelta;

		// 수명 끝
		if (p._age >= p._lifeTime)
		{
			p._isAlive = false;
			continue;
		}

		float t = p._age / p._lifeTime;


		p.velocity *= 0.98f;

		// 위로 아주 약하게 상승
		p.velocity.y += 0.15f * fTimeDelta;

		float sway1 = sinf(p._age * 1.7f + p._position.y * 0.05f);
		float sway2 = cosf(p._age * 2.3f + p._position.x * 0.03f);

		p.velocity.x += sway1 * 0.25f * fTimeDelta;


		p.velocity.y += sinf(p._age * 0.8f) * 0.03f * fTimeDelta;

		p._position += p.velocity * fTimeDelta;


		float flicker = 0.65f
			+ 0.35f * sinf(p._age * 6.0f + p._position.x * 0.2f);

		float fadeIn = min(t * 3.0f, 1.0f);
		float fadeOut = min((1.0f - t) * 2.0f, 1.0f);

		//p._color = flicker * fadeIn * fadeOut;


	}

	removeDeadParticles();
}
void CBloomBubble::Free()
{
	PSystem::Free();
}
