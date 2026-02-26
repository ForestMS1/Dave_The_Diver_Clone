#include "PSystem.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetCubeTexture.h"




PSystem::PSystem() 
{
	_device = CGraphicDev::GetInstance()->Get_GraphicDev();
	_device->AddRef();
	_origin = { 0,0,0 };
	//_particles;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 512;
	_emitRate = 0.f;
	_size = { 0.f,0.f,0.f };
	_matParticles = 0;
	numOfParticles = 0;
	empty = false;
}


PSystem::~PSystem()
{
}

HRESULT	 PSystem::Ready_Buffer()
{
	if (FAILED(_device
		->CreateVertexBuffer(_vbSize * sizeof(Particle)
			, D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY
			, FVF, D3DPOOL_DEFAULT, &_vb, 0)))
		return E_FAIL;

	return S_OK;
}
 
void PSystem::reset(_vec3 position, _vec3 center, _vec3 extents, D3DXCOLOR color)
{
	list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++) {
		i->_position = position;
		resetParticle(&(*i), color);
	}
}


void PSystem::addParticle(_vec3 position, D3DXCOLOR color)
{
	Attribute attribute;
	_hitPosition = position;
	resetParticle(&attribute, color);
	_particles.push_back(attribute);
	
}

void PSystem::preRender()
{
	_device->SetRenderState(D3DRS_LIGHTING, false);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSIZE, FtoDw(_size.x));
	_device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	//거리에 따른 파티클 크기 제어
	_device->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	//텍스처을 알파를 이용한다

	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


}

void PSystem::render()
{

	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, _origin.x, _origin.y, _origin.z);
	_device->SetTransform(D3DTS_WORLD, &matWorld);
	if (!_particles.empty()) {
		preRender();
		_device->SetFVF(FVF);
		_device->SetStreamSource(0, _vb, 0, sizeof(Particle));

		//버택스 버퍼를 벗어날 경우 처음부터 시작
		if (_vbOffset >= _vbSize) {
			_vbOffset = 0;
		}
		Particle* v = 0;
		_vb->Lock(_vbOffset * sizeof(Particle), _vbBatchSize * sizeof(Particle)
			, (void**)&v, _vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;
		list<Attribute>::iterator i;

		for (i = _particles.begin(); i != _particles.end(); i++) {
			if (i->_isAlive) {
				v->_poisition = i->_position;
				v->_color = (D3DCOLOR)i->_color;
				v++;
				numParticlesInBatch++;

				if (numParticlesInBatch == _vbBatchSize) {
					_vb->Unlock();
					_device->DrawPrimitive(D3DPT_POINTLIST, _vbOffset, _vbBatchSize);
					_vbOffset += _vbBatchSize;
					// 버택스 버퍼의 경계를 넘는 메모리로 오프셋을 설정하지 않는다. 
					// 경계를 념을 경우 처음부터 시작한다. 
					if (_vbOffset >= _vbSize) {
						_vbOffset = 0;
					}
					_vb->Lock(_vbOffset * sizeof(Particle), _vbBatchSize * sizeof(Particle)
						, (void**)&v, _vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
					numParticlesInBatch = 0;
				}
			}
		}
		_vb->Unlock();

		if (numParticlesInBatch) {
			_device->DrawPrimitive(
				D3DPT_POINTLIST,
				_vbOffset,
				numParticlesInBatch);
		}
		_vbOffset += _vbBatchSize;
		postRender();
	}
}

void PSystem::postRender()
{
	//_device->SetRenderState(D3DRS_LIGHTING, true);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

bool PSystem::isEmpty()
{
	if (_particles.empty())
		return true;
	else
		return false;
}

bool PSystem::isDead()
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++)
	{
		if (i->_isAlive)
			return false;
	}
	return true;
}

DWORD PSystem::FtoDw(float f)
{
	return *((DWORD*)&f);
}

float PSystem::GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) {
		return lowBound;
	}
	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void PSystem::GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}



//HRESULT PSystem::Ready_Texture(TEXTUREID eID, const _tchar* pPath, const _uint& iCnt)
//{
//	m_vecTexture.reserve(iCnt);
//
//	IDirect3DBaseTexture9* pTexture = nullptr;
//
//	for (_uint i = 0; i < iCnt; ++i)
//	{
//		TCHAR szFileName[128] = L"";
//
//		wsprintf(szFileName, pPath, i);
//
//		switch (eID)
//		{
//		case TEX_NORMAL:
//
//			if (FAILED(D3DXCreateTextureFromFile(_device, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
//				return E_FAIL;
//
//			break;
//
//		case TEX_CUBE:
//
//			if (FAILED(D3DXCreateCubeTextureFromFile(_device, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
//				return E_FAIL;
//
//			break;
//		}
//
//		m_vecTexture.push_back(pTexture);
//	}
//	return S_OK;
//}

HRESULT PSystem::Ready_Texture(std::wstring_view svLayerTag, std::wstring_view svPath, const _uint& iCnt)
{

	for (int i = 0; i < iCnt; ++i)
	{
		wstring s = std::wstring(svPath) + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(svLayerTag, CAssetTexture::Create(s.c_str()));
	}
	CAssetMgr::GetInstance()->LoadAsset();

	return S_OK;
}

void PSystem::Set_Texture(const _uint& iIndex)
{
	if (m_vecTexture.size() <= iIndex)
		return;

	_device->SetTexture(0, m_vecTexture[iIndex]);
}


void PSystem::Set_Texture(std::wstring_view svLayerTag,const _uint& iIndex)
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto pAssTex = dynamic_cast<CAssetTexture*>((*CAssetMgr::GetInstance()->Get_Asset(svLayerTag))[iIndex]))
	{
		pGraphicDev->SetTexture(0, pAssTex->Get_Texture());
	}
	else if (auto pAssTex = dynamic_cast<CAssetCubeTexture*>((*CAssetMgr::GetInstance()->Get_Asset(svLayerTag))[iIndex]))
	{
		pGraphicDev->SetTexture(0, pAssTex->Get_CubeTexture());
	}
	
}

void PSystem::Free()
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), Safe_Release<IDirect3DBaseTexture9*>);
	m_vecTexture.clear();
	list<Attribute>::iterator iter = _particles.begin();
	for (iter; iter != _particles.end();) {
		iter = _particles.erase(iter);
	}
	Safe_Release(_vb);
	Safe_Release(_device);
}

void PSystem::removeDeadParticles()
{
	list<Attribute>::iterator i;

	i = _particles.begin();

	while (i != _particles.end()) {
		if (i->_isAlive == false) {
			i = _particles.erase(i);
		}
		else {
			i++;
		}
	}
	if (_particles.size() == 0) {
		empty = true;
	}
}
