#include "Collider_Box.h"
#include "Transform.h"

CCollider_Box::CCollider_Box(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCollider(pGraphic_Device)
{
}

CCollider_Box::CCollider_Box(const CCollider_Box& rhs)
	: CCollider(rhs)
	, m_pIB(rhs.m_pIB)
	, m_iVertexStride(rhs.m_iVertexStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwNumPrimitive(rhs.m_dwNumPrimitive)
	, m_iIndexStride(rhs.m_iIndexStride)
	, m_iNumIndices(rhs.m_iNumIndices)
	, m_eIndexFormat(rhs.m_eIndexFormat)
{
	Safe_AddRef(m_pIB);
}

HRESULT CCollider_Box::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//	For.VertexBuffer

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	/* 할당된 공간의 가장 앞 주소를 얻어온다. */
	VTXCOLLIDER* pVertices = { nullptr };


	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CCollider_Box::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_iNumVertices = 8;
	m_iVertexStride = sizeof(VTXCOLLIDER);
	m_dwFVF = D3DFVF_XYZ;
	m_dwNumPrimitive = 12;

	m_iNumIndices = 24;
	m_iIndexStride = m_iNumVertices >= 65535 ? sizeof(_uint) : sizeof(_ushort);
	m_eIndexFormat = sizeof(_ushort) == m_iIndexStride ? D3DFMT_INDEX16 : D3DFMT_INDEX32;

	m_iNumVertices = COLLIDER_BOX_VERTICES;

	//	For.IndexBuffer

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0] = 0;	pIndices[1] = 1;
	pIndices[2] = 1;	pIndices[3] = 2;
	pIndices[4] = 2;	pIndices[5] = 3;
	pIndices[6] = 3;	pIndices[7] = 0;

	pIndices[8] = 4;	pIndices[9] = 5;
	pIndices[10] = 5;	pIndices[11] = 6;
	pIndices[12] = 6;	pIndices[13] = 7;
	pIndices[14] = 7;	pIndices[15] = 4;

	pIndices[16] = 0;	pIndices[17] = 4;
	pIndices[18] = 1;	pIndices[19] = 5;

	pIndices[20] = 2;	pIndices[21] = 6;
	pIndices[22] = 3;	pIndices[23] = 7;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CCollider_Box::Render()
{
	///* 항등행렬을 이용하는 이유 */
	///* 이미 정점들의 위치가 월드 좌표 기준으로 설정되어있음 */
	//_float4x4 IdentityMatrix;
	//D3DXMatrixIdentity(&IdentityMatrix);
	//m_pGraphic_Device->SetTransform(D3DTS_WORLD, &IdentityMatrix);

	//	Update.새로운 ColliderTransform과 ObjectTransform을 별도로 분류하였음
	if (FAILED(Update_ColliderTransform()))
		return E_FAIL;

	if (FAILED(m_pColliderTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetIndices(m_pIB)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetFVF(m_dwFVF)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_iNumVertices, 0, m_dwNumPrimitive)))
		return E_FAIL;

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}


_bool CCollider_Box::Check_Box(const CTransform* pTargetTransform, _Out_ _float3* pOverlap)
{
	Update_ColliderTransform();

	_bool isCollision = { false };

	MIN_MAX_POS tTargetMinMaxPos = Compute_MinPos_MaxPos(pTargetTransform);

	_float3 vTargetMinPos = tTargetMinMaxPos.vMinPos;
	_float3 vTargetMaxPos = tTargetMinMaxPos.vMaxPos;

	if (m_tMinMaxPos.vMinPos.x <= vTargetMaxPos.x && m_tMinMaxPos.vMaxPos.x >= vTargetMinPos.x &&
		m_tMinMaxPos.vMinPos.y <= vTargetMaxPos.y && m_tMinMaxPos.vMaxPos.y >= vTargetMinPos.y &&
		m_tMinMaxPos.vMinPos.z <= vTargetMaxPos.z && m_tMinMaxPos.vMaxPos.z >= vTargetMinPos.z)
	{
		isCollision = true;
	}

	if (nullptr != pOverlap)
	{
		if (true == isCollision)
		{
			pOverlap->x = min(vTargetMaxPos.x, m_tMinMaxPos.vMaxPos.x) - max(vTargetMinPos.x, m_tMinMaxPos.vMinPos.x);
			pOverlap->y = min(vTargetMaxPos.y, m_tMinMaxPos.vMaxPos.y) - max(vTargetMinPos.y, m_tMinMaxPos.vMinPos.y);
			pOverlap->z = min(vTargetMaxPos.z, m_tMinMaxPos.vMaxPos.z) - max(vTargetMinPos.z, m_tMinMaxPos.vMinPos.z);
		}
		else
		{
			*pOverlap = {};
		}
	}

	return isCollision;
}

_bool CCollider_Box::Check_Sphere(const CTransform* pTargetTransform, _Out_ _float3* pOverlap)
{
	Update_ColliderTransform();

	_bool isCollision = { false };

	_float3 vDstScale = pTargetTransform->Get_Scaled();
	_float fRadiusSphere = D3DXVec3Length(&vDstScale) * 0.5f;
	_float3 vDstPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);

	_float fNearX = clamp(vDstPos.x, m_tMinMaxPos.vMinPos.x, m_tMinMaxPos.vMaxPos.x);
	_float fNearY = clamp(vDstPos.y, m_tMinMaxPos.vMinPos.y, m_tMinMaxPos.vMaxPos.y);
	_float fNearZ = clamp(vDstPos.z, m_tMinMaxPos.vMinPos.z, m_tMinMaxPos.vMaxPos.z);

	_float3 vNearPos = { fNearX, fNearY, fNearZ };
	_float3 vNearDir = vNearPos - m_pColliderTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = vDstPos - m_pColliderTransform->Get_State(CTransform::STATE_POSITION);

	if (D3DXVec3Length(&vDir) <= fRadiusSphere + D3DXVec3Length(&vNearDir))
	{
		isCollision = true;

		if (nullptr != pOverlap)
		{
			_float3 vNormalizeDir = *D3DXVec3Normalize(&vNormalizeDir, &vDir);
			_float fSize = fRadiusSphere + D3DXVec3Length(&vNearDir) - D3DXVec3Length(&vDir);

			*pOverlap = vNormalizeDir * fSize;
		}
	}

	else
	{
		if (nullptr != pOverlap)
		{
			*pOverlap = { };
		}
	}

	return isCollision;
}

HRESULT CCollider_Box::Update_ColliderTransform()
{
	m_tMinMaxPos = Compute_MinPos_MaxPos(m_pObjectTransform);

	_float fScaleX = m_tMinMaxPos.vMaxPos.x - m_tMinMaxPos.vMinPos.x;
	_float fScaleY = m_tMinMaxPos.vMaxPos.y - m_tMinMaxPos.vMinPos.y;
	_float fScaleZ = m_tMinMaxPos.vMaxPos.z - m_tMinMaxPos.vMinPos.z;

	_float3 vSacle = { fScaleX, fScaleY, fScaleZ };

	m_pColliderTransform->Set_Scaled(fScaleX, fScaleY, fScaleZ);

	m_pColliderTransform->Set_State(CTransform::STATE_POSITION, m_pObjectTransform->Get_State(CTransform::STATE_POSITION));

	return S_OK;
}

HRESULT CCollider_Box::Create_VertexBuffer()
{
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider_Box::Create_IndexBuffer()
{
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, 0)))
		return E_FAIL;

	return S_OK;
}

CCollider_Box* CCollider_Box::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_Box* pInstance = new CCollider_Box(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCollider_Box"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider_Box::Clone(void* pArg)
{
	CCollider_Box* pInstance = new CCollider_Box(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CCollider_Box"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Box::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
