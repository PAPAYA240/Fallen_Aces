#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_iVertexStride(rhs.m_iVertexStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_dwFVF(rhs.m_dwFVF)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_iIndexStride(rhs.m_iIndexStride)
	, m_iNumIndices(rhs.m_iNumIndices)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_pVerticesPos(rhs.m_pVerticesPos)
{
	Safe_AddRef(m_pVB);	
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	/* 어떤 버텍스 버퍼를 장치에 바인딩할껀데? */
	if (FAILED(m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetIndices(m_pIB)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetFVF(m_dwFVF)))
		return E_FAIL;

	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);	
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	/* 정점 배열을 할당하고 LPDIRECT3DVERTEXBUFFER라는 컴객체를 생성한다. */	
	/* D3DUSAGE_DYNAMIC : 메모리공간을 동적으로 만든다 .*/
	/* 0 : 메모리공간을 정적으로 만든다 .*/
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, 0)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVerticesPos);
}
