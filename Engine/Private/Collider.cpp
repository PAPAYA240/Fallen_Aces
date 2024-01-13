#include "Collider.h"
#include "Transform.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
{
}

HRESULT CCollider::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CCollider::COLLIDER_DESC* pColliderDesc = (COLLIDER_DESC*)pArg;

	m_pObjectTransform = pColliderDesc->pObjectTransform;

	Safe_AddRef(m_pObjectTransform);

	m_pColliderTransform = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pColliderTransform)
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetTexture(0, nullptr);

	return S_OK;
}

HRESULT CCollider::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}


CCollider::MIN_MAX_POS CCollider::Compute_MinPos_MaxPos(const CTransform* pTransform) const
{
	if (nullptr == pTransform)
		return CCollider::MIN_MAX_POS{};

	_float3 arrVerticesPos[COLLIDER_BOX_VERTICES];

	const _float4x4* pWorldMatrix = pTransform->Get_WorldMatrix();

	arrVerticesPos[0] = _float3(-0.5f, 0.5f, -0.5f);
	arrVerticesPos[1] = _float3(0.5f, 0.5f, -0.5f);
	arrVerticesPos[2] = _float3(0.5f, -0.5f, -0.5f);
	arrVerticesPos[3] = _float3(-0.5f, -0.5f, -0.5f);
	arrVerticesPos[4] = _float3(-0.5f, 0.5f, 0.5f);
	arrVerticesPos[5] = _float3(0.5f, 0.5f, 0.5f);
	arrVerticesPos[6] = _float3(0.5f, -0.5f, 0.5f);
	arrVerticesPos[7] = _float3(-0.5f, -0.5f, 0.5f);

	//	D3DXVec3TransformCoordArray(arrVerticesPos, sizeof(_float3), arrVerticesPos, sizeof(_float3), pWorldMatrix, m_iNumVertices);

	for (size_t i = 0; i < COLLIDER_BOX_VERTICES; ++i)
	{
		D3DXVec3TransformCoord(&arrVerticesPos[i], &arrVerticesPos[i], pWorldMatrix);
	}

	MIN_MAX_POS tMinMaxPos = {};

	tMinMaxPos.vMinPos = arrVerticesPos[0];
	tMinMaxPos.vMaxPos = arrVerticesPos[0];

	for (auto vVertex : arrVerticesPos)
	{
		//	각 축에 대한 최외각 정점을 기준으로해야함
		if (tMinMaxPos.vMinPos.x > vVertex.x)
			tMinMaxPos.vMinPos.x = vVertex.x;

		if (tMinMaxPos.vMaxPos.x < vVertex.x)
			tMinMaxPos.vMaxPos.x = vVertex.x;

		if (tMinMaxPos.vMinPos.y > vVertex.y)
			tMinMaxPos.vMinPos.y = vVertex.y;

		if (tMinMaxPos.vMaxPos.y < vVertex.y)
			tMinMaxPos.vMaxPos.y = vVertex.y;

		if (tMinMaxPos.vMinPos.z > vVertex.z)
			tMinMaxPos.vMinPos.z = vVertex.z;

		if (tMinMaxPos.vMaxPos.z < vVertex.z)
			tMinMaxPos.vMaxPos.z = vVertex.z;
	}

	return tMinMaxPos;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Release(m_pObjectTransform);
	Safe_Release(m_pColliderTransform);

	/*if (true == m_isCloned)
		Safe_Delete_Array(m_pWorldVerticesPos);*/
}
