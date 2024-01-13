#include "..\Public\VIBuffer_Terrain.h"

#include "Transform.h"
#include "GameInstance.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	
{

}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;

	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_iNumPrimitive = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;

	m_iIndexStride = m_iNumVertices >= 65535 ? 4 : 2;
	m_iNumIndices = m_iNumPrimitive * 3;
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

	for (size_t i = 0; i < iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < iNumVerticesX; j++)
		{
			_uint		iIndex = _uint(i * iNumVerticesX + j);

			pVertices[iIndex].vPosition = _float3((_float)j, 0.0f, (_float)i);
			pVertices[iIndex].vTexcoord = _float2(j / (iNumVerticesX - 1.f) * 50.f, i / (iNumVerticesZ - 1.f) * 50.f);
		}
	}
	m_pVB->Unlock();

#pragma endregion


#pragma region INDEX_BUFFER

	HRESULT	hr = m_iIndexStride == 2 ? Ready_Index16_Buffer() : Ready_Index32_Buffer();
	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const wstring & strHeightMapFilePath)
{
	BITMAPFILEHEADER	fh;
	BITMAPINFOHEADER	ih;
	_ulong*				pPixels = { nullptr };

	_ulong				dwByte = { 0 };
	HANDLE				hFile = CreateFile(strHeightMapFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	pPixels = new _ulong[m_iNumVerticesX * m_iNumVerticesZ];

	ReadFile(hFile, pPixels, sizeof(_ulong) * m_iNumVerticesX * m_iNumVerticesZ, &dwByte, nullptr);

	CloseHandle(hFile);

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_iVertexStride = sizeof(VTXPOSTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	m_iIndexStride = m_iNumVertices >= 65535 ? 4 : 2;
	m_iNumIndices = m_iNumPrimitive * 3;
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

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint		iIndex = _uint(i * m_iNumVerticesX + j);

			// pPixels[iIndex] : 11111111 01100100 01100100 01100100
			//               &   00000000 00000000 00000000 11111111

			//                   00000000 00000000 00000000 01100100



			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3((_float)j, (pPixels[iIndex] & 0x000000ff) / 10.f, (_float)i);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f) * 50.f, i / (m_iNumVerticesZ - 1.f) * 50.f);
		}
	}
	m_pVB->Unlock();

#pragma endregion


#pragma region INDEX_BUFFER

	HRESULT	hr = m_iIndexStride == 2 ? Ready_Index16_Buffer() : Ready_Index32_Buffer();
	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

	Safe_Delete_Array(pPixels);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Terrain::Ready_Index16_Buffer()
{
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort*		pIndices = { nullptr };

	_uint		iNumIndices = { 0 };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = _uint(i * m_iNumVerticesX + j);

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];		
		}
	}
	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Ready_Index32_Buffer()
{
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_uint*		pIndices = { nullptr };

	_uint		iNumIndices = { 0 };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = _uint(i * m_iNumVerticesX + j);

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];
		}
	}
	m_pIB->Unlock();

	return S_OK;
}

_float CVIBuffer_Terrain::Compute_Height(const _float3 & vTargetPos)
{
	_uint		iIndex = _uint(vTargetPos.z) * m_iNumVerticesX + _uint(vTargetPos.x);

	_uint		iIndices[4] = {
		iIndex + m_iNumVerticesX, 
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1, 
		iIndex
	};

	_float		fWidth = vTargetPos.x - m_pVerticesPos[iIndices[0]].x;
	_float		fDepth = m_pVerticesPos[iIndices[0]].z - vTargetPos.z;

	D3DXPLANE	Plane = {};

	/* d오른쪽 위 삼각형안에 있냐. */
	if (fWidth > fDepth)	
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]]);

	/* 왼쪽 아래 삼각형안에 있냐 */
	else	
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[2]], &m_pVerticesPos[iIndices[3]]);
	
	/* ax + by + cz + d = 0 */
	/* y = (-ax - cz - d) / b*/
	return (-Plane.a * vTargetPos.x - Plane.c * vTargetPos.z - Plane.d) / Plane.b;	
}

_float3 CVIBuffer_Terrain::Compute_Picking(const CTransform * pTransform) const
{
	_float3		vRayDir, vRayPos;

	m_pGameInstance->Transform_PickingToLocalSpace(pTransform, &vRayDir, &vRayPos);

	_float3	vOut(0.f, 0.f, 0.f);

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = _uint(i * m_iNumVerticesX + j);

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

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
		}
	}

exit:
	D3DXVec3TransformCoord(&vOut, &vOut, pTransform->Get_WorldMatrix());

	return vOut;	
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Terrain"));

		Safe_Release(pInstance);
	}

	return pInstance;
}
CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const wstring & strHeightMapFilePath)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(strHeightMapFilePath)))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Terrain"));

		Safe_Release(pInstance);
	}

	return pInstance;
}
/* 사본객체를 생성하기위한 함수에요. */
CComponent * CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Terrain"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

	

}
