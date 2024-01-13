#include "Collider_Sphere.h"
#include "Transform.h"

CCollider_Sphere::CCollider_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCollider(pGraphic_Device)
{
}

CCollider_Sphere::CCollider_Sphere(const CCollider_Sphere& rhs)
	: CCollider(rhs)
	, m_pSphereMesh(rhs.m_pSphereMesh)
{
	Safe_AddRef(m_pSphereMesh);
}

HRESULT CCollider_Sphere::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider_Sphere::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	D3DXCreateSphere(m_pGraphic_Device, 0.5f, 20, 20, &m_pSphereMesh, NULL);

	return S_OK;
}

HRESULT CCollider_Sphere::Render()
{
	if (FAILED(Set_RenderState()))
		return E_FAIL;

	_float3 vScale = m_pObjectTransform->Get_Scaled();
	_float fDiameter = D3DXVec3Length(&vScale);

	_float3 vPos = m_pObjectTransform->Get_State(CTransform::STATE_POSITION);
	m_pColliderTransform->Set_State(CTransform::STATE_POSITION, vPos);
	m_pColliderTransform->Set_Scaled(fDiameter, fDiameter, fDiameter);

	m_pColliderTransform->Bind_WorldMatrix();

	m_pSphereMesh->DrawSubset(0);

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

_bool CCollider_Sphere::Check_Sphere(const CTransform* pTargetTransform, _Out_ _float3* pOverlap)
{
	_bool isCollision = { false };

	_float3 vDir = pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pObjectTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vNormalizeDir = *D3DXVec3Normalize(&vNormalizeDir, &vDir);

	_float fDistance = D3DXVec3Length(&vDir);

	_float3 vSrcScale = m_pObjectTransform->Get_Scaled();
	_float3 vDstScale = pTargetTransform->Get_Scaled();

	//	반지름을 외곽 꼭지점과의 거리로 설정 (외접 구)
	_float fTotalRadius = D3DXVec3Length(&vDstScale) * 0.5f + D3DXVec3Length(&vDstScale) * 0.5f;

	if (fTotalRadius >= fDistance)
	{
		isCollision = true;

		if (nullptr != pOverlap)
		{
			*pOverlap = vNormalizeDir * (fTotalRadius - fDistance);
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

_bool CCollider_Sphere::Check_Box(const CTransform* pTargetTransform, _Out_ _float3* pOverlap)
{
	MIN_MAX_POS tTargetMinMaxPos = Compute_MinPos_MaxPos(pTargetTransform);

	_bool isCollision = { false };

	_float3 vSrcScale = m_pColliderTransform->Get_Scaled();
	_float fRadius = D3DXVec3Length(&vSrcScale) * 0.5f;
	_float3 vSrcPos = m_pColliderTransform->Get_State(CTransform::STATE_POSITION);

	if ((vSrcPos.x + fRadius) >= tTargetMinMaxPos.vMinPos.x && (vSrcPos.x - fRadius) <= tTargetMinMaxPos.vMaxPos.x &&
		(vSrcPos.y + fRadius) >= tTargetMinMaxPos.vMinPos.y && (vSrcPos.y - fRadius) <= tTargetMinMaxPos.vMaxPos.y &&
		(vSrcPos.z + fRadius) >= tTargetMinMaxPos.vMinPos.z && (vSrcPos.z - fRadius) <= tTargetMinMaxPos.vMaxPos.z)
	{
		isCollision = true;

		if (nullptr != pOverlap)
		{

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

HRESULT CCollider_Sphere::Update_ColliderTransform()
{
	return S_OK;
}

CCollider_Sphere* CCollider_Sphere::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_Sphere* pInstance = new CCollider_Sphere(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCollider_Sphere"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider_Sphere::Clone(void* pArg)
{
	CCollider_Sphere* pInstance = new CCollider_Sphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CCollider_Sphere"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Sphere::Free()
{
	__super::Free();

	Safe_Release(m_pSphereMesh);
}
