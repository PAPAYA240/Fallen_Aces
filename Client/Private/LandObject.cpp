#include "stdafx.h"
#include "..\Public\LandObject.h"

#include "Collider_Box.h"

CLandObject::CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CLandObject::CLandObject(const CLandObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLandObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::Initialize(void * pArg)
{
	LANDOBJECT_DESC*	pLandObjectDesc = (LANDOBJECT_DESC*)pArg;

	m_pTerrainTransform = pLandObjectDesc->pTerrainTransform;
	m_pTerrainVIBuffer = pLandObjectDesc->pTerrainVIBuffer;

	Safe_AddRef(m_pTerrainTransform);
	Safe_AddRef(m_pTerrainVIBuffer);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CLandObject::Tick(_float fTimeDelta)
{

}

void CLandObject::Late_Tick(_float fTimeDelta)
{
}

HRESULT CLandObject::Render()
{
	if (FAILED(m_pColliderCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLandObject::SetUp_OnTerrain(_float fRevision)
{
	/* 현재 지형을 타야하는객체의 월드 위치르 ㄹ얻어오낟. */
	_float3		vTargetPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	/* 월드위치르 짖형버퍼의 로컬로 수정하낟. */
	D3DXVec3TransformCoord(&vTargetPos, &vTargetPos, m_pTerrainTransform->Get_WorldMatrix_Inverse());

	/* 지형을 타야하는개체의 로컬위치와 지형 정점의 로컬위치르 ㄹ비교하여 지형ㅇ르 태우는 높이를 계산해서 높이로 저장하낟. */
	vTargetPos.y = m_pTerrainVIBuffer->Compute_Height(vTargetPos) + fRevision;

	/* 지형위로 고정한 로컬 위치를 다ㅣㅅ ㅣ 월드로 치환하낟. */
	D3DXVec3TransformCoord(&vTargetPos, &vTargetPos, m_pTerrainTransform->Get_WorldMatrix());

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargetPos);		

	return S_OK;
}

void CLandObject::Free()
{
	__super::Free();

	Safe_Release(m_pTerrainTransform);
	Safe_Release(m_pTerrainVIBuffer);

	for (auto iter : m_Collisions)
	{
		Safe_Release(iter.pCollisionObject);
	}

	Safe_Release(m_pColliderCom);
}
