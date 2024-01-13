#pragma once

/* Vertex, Index에 대한 공간을 할당한다. */
/* 할당한 공간을 가지고 있는다. */
/* 이 공간에 있는 정점과 인덱스를 이용하여 화면에 그려낸다. */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = { nullptr };
	LPDIRECT3DINDEXBUFFER9			m_pIB = { nullptr };

	_uint							m_iVertexStride = { 0 }; /* 정점하나의 크기(Byte) */
	_uint							m_iNumVertices = { 0 }; /* 정점의 갯수*/
	_ulong							m_dwFVF = { 0 }; /* 내 정점은 어떤 멤버들을 가지고 있어. */
	_ulong							m_iNumPrimitive = { 0 };

	_uint							m_iIndexStride = { 0 };
	_uint							m_iNumIndices = { 0 };
	D3DFORMAT						m_eIndexFormat = { };

	_float3*						m_pVerticesPos = { nullptr };

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END