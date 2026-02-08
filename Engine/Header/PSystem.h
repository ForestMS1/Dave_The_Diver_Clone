#pragma once
#include "CComponent.h"
#include <DirectXCollision.h >
BEGIN(Engine)

class ENGINE_DLL PSystem :
	public CBase
{
public:


public:
	explicit PSystem();
	virtual ~PSystem();

	virtual	HRESULT	 Ready_Buffer();
	virtual void reset(_vec3 position, D3DXCOLOR color);
	virtual void resetParticle(Attribute* attribute, D3DXCOLOR color) = 0;
	virtual void addParticle(_vec3 position, D3DXCOLOR color);

	virtual void update(float timeDelta) = 0;
	virtual void preRender();
	virtual void render();
	virtual void postRender();
	bool isEmpty();
	bool isDead();
	DWORD FtoDw(float f);
	float GetRandomFloat(float lowBound, float highBound);
	void GetRandomVector(
		D3DXVECTOR3* out,
		D3DXVECTOR3* min,
		D3DXVECTOR3* max);

public:
	virtual HRESULT	Ready_Texture(TEXTUREID eID, const _tchar* pPath, const _uint& iCnt);
	virtual void		Set_Texture(const _uint& iIndex = 0);
	virtual void Free();

protected:
	virtual void removeDeadParticles();
public:

	IDirect3DDevice9*					_device;
	D3DXVECTOR3							_origin;
	TmpBoundingBox							_boundingBox;
	IDirect3DVertexBuffer9*				 _vb;
	list<Attribute>						_particles;
	DWORD								_vbSize;
	DWORD								_vbOffset;
	DWORD								_vbBatchSize;
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	float								_emitRate;
	_vec3								_size;
	int									_matParticles;
	int									numOfParticles;
	bool								empty;
};

END