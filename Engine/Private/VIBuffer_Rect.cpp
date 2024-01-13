#include "..\Public\VIBuffer_Rect.h"
#include "GameInstance.h"

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	m_iNumVertices = 4;
	m_pVerticesPos = new _float3[m_iNumVertices];
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	m_iNumPrimitive = 2;

	m_iIndexStride = m_iNumVertices >= 65535 ? 4 : 2;
	m_iNumIndices = 6;
	m_eIndexFormat = m_iIndexStride == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32;


#pragma region VERTEX_BUFFER
	/* 정점 여섯개의 배열 공간을 할당한다. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	/* 할당된 공간의 가장 앞 주소를 얻어온다. */
	VTXPOSTEX*		pVertices = { nullptr };

	/* 할당된 정점 배열공간의 가장 앞 주소를 얻어올 수 있따. */
	/* 이 공간을 걸어 잠근다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_pVerticesPos[0] = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);

	pVertices[1].vPosition = m_pVerticesPos[1] = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.0f, 0.f);

	pVertices[2].vPosition = m_pVerticesPos[2] = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.0f, 1.f);

	pVertices[3].vPosition = m_pVerticesPos[3] = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.0f, 1.f);

	m_pVB->Unlock();

#pragma endregion


#pragma region INDEX_BUFFER

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort*		pIndices = { nullptr };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void * pArg)
{
	return S_OK;
}

_float3 CVIBuffer_Rect::Compute_Picking(const CTransform * pTransform) const
{
	_float3		vRayDir, vRayPos;

	m_pGameInstance->Transform_PickingToLocalSpace(pTransform, &vRayDir, &vRayPos);

	_float3		vOut(0.f, 0.f, 0.f);

	_uint		iIndices[4] = { 0, 1, 2, 3};

	_float	fU, fV, fDist;


			/* 오른쪽 위 삼각형과 충돌인가? */
	if (TRUE == D3DXIntersectTri(&m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]], &vRayPos, D3DXVec3Normalize(&vRayDir, &vRayDir), &fU, &fV, &fDist))
	{
		vOut = vRayPos + vRayDir * fDist;
		goto exit;

	}

	/* 왼쪽 아래 삼각형과 충돌인가? */
	if (TRUE == D3DXIntersectTri(&m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[2]], &m_pVerticesPos[iIndices[3]], &vRayPos, D3DXVec3Normalize(&vRayDir, &vRayDir), &fU, &fV, &fDist))
	{
		vOut = vRayPos + vRayDir * fDist;
		goto exit;
	}

exit:
	D3DXVec3TransformCoord(&vOut, &vOut, pTransform->Get_WorldMatrix());

	return vOut;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect*		pInstance = new CVIBuffer_Rect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Rect"));

		Safe_Release(pInstance);
	}

	return pInstance;
}
/* 사본객체를 생성하기위한 함수에요. */
CComponent * CVIBuffer_Rect::Clone(void* pArg)
{
	CVIBuffer_Rect*		pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Rect"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();

}
