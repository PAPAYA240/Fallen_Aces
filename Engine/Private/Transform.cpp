#include "..\Public\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

void CTransform::Set_Scaled(_float fX, _float fY, _float fZ)
{
	_float3		vRight = Get_State(STATE_RIGHT);
	_float3		vUp = Get_State(STATE_UP);
	_float3		vLook = Get_State(STATE_LOOK);

	Set_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * fX);
	Set_State(STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * fY);
	Set_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * fZ);
}

void CTransform::NormalizeScale(const POINT& _ptSize)
{
	_float fMax = (_float)_ptSize.x < _ptSize.y ? _ptSize.y : _ptSize.x;

	if (512 > fMax)
	{
		m_vScale.x = (_ptSize.x / 512.f);
		m_vScale.y = (_ptSize.y / 512.f);
	}
	else
	{
		m_vScale.x = (_ptSize.x / 1024.f);
		m_vScale.y = (_ptSize.y / 1024.f);
	}

	D3DXMatrixIdentity(&m_TestMatrix);
	
	m_TestMatrix._11 *= m_vScale.x;
	m_TestMatrix._22 *= m_vScale.y;

	m_TestMatrix *= m_WorldMatrix;
}

HRESULT CTransform::Initialize_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	m_vToLookAt = *(_float3*)&m_WorldMatrix.m[2][0];

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return S_OK;

	TRANSFORM_DESC*		pTransformDesc = (TRANSFORM_DESC*)pArg;

	m_fSpeedPerSec = pTransformDesc->fSpeedPerSec;
	m_fRotationPerSec = pTransformDesc->fRotationPerSec;

	LookAt_ForLandObject(_float3(0.f, 0.f, 1.f));
	m_vToLookAt = Get_State(CTransform::STATE_LOOK);

	return S_OK;
}

void CTransform::Refresh_ToLook()
{
	m_vToLookAt = *(_float3*)&m_WorldMatrix.m[2][0];
}

HRESULT CTransform::Bind_WorldMatrix()
{
	return m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vDir = Get_State(STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vDir = Get_State(STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vDir = Get_State(STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vDir = Get_State(STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Turn(const _float3 & vAxis, _float fTimeDelta)
{
	_float4x4		RotatonMatrix;
	D3DXMatrixRotationAxis(&RotatonMatrix, &vAxis, m_fRotationPerSec * fTimeDelta);

	_float3		vRight = Get_State(STATE_RIGHT);
	_float3		vUp = Get_State(STATE_UP);
	_float3		vLook = Get_State(STATE_LOOK);

	Set_State(STATE_RIGHT, *D3DXVec3TransformNormal(&vRight, &vRight, &RotatonMatrix));
	Set_State(STATE_UP, *D3DXVec3TransformNormal(&vUp, &vUp, &RotatonMatrix));
	Set_State(STATE_LOOK, *D3DXVec3TransformNormal(&vLook, &vLook, &RotatonMatrix));
}

void CTransform::LookAt(const _float3 & vAtPoint)
{
	_float3		vScaled = Get_Scaled();

	_float3		vPosition = Get_State(STATE_POSITION);

	_float3		vLook = vAtPoint - vPosition;
	_float3		vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
	_float3		vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);

	Set_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x);
	Set_State(STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScaled.y);
	Set_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScaled.z);
}

void CTransform::LookAt_ForLandObject(const _float3 & vAtPoint)
{
	_float3		vScaled = Get_Scaled();

	_float3		vPosition = Get_State(STATE_POSITION);

	_float3		vLook = vAtPoint - vPosition;
	_float3		vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);

	vLook = *D3DXVec3Cross(&vLook, &vRight, &_float3(0.f, 1.f, 0.f));

	Set_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x);
	Set_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScaled.z);
}

_bool CTransform::MoveToTarget(const CTransform* pTargetTransform, _float fTimeDelta, _float fMinDistance)
{
	_float3		vPosition = Get_State(STATE_POSITION);

	/* vDir : 내위치에서 타겟 위치까지를 잇는 방향벡터가 구해진다. */
	/* 방향 : 방향벡터는 내가 타겟위치ㅐ를 바라보는 방향. */
	/* 크기 : 내 위치에서 상대방까지의 거리. */
	_float3		vDir = pTargetTransform->Get_State(STATE_POSITION) - vPosition;

	_float		fDistance = D3DXVec3Length(&vDir);

	if (fDistance > fMinDistance)
	{
		vPosition += *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeedPerSec * fTimeDelta;
		Set_State(STATE_POSITION, vPosition);

		return false;
	}	

	return true;
}

_bool CTransform::MoveToTarget(const _float3 & vTargetPoint, _float fTimeDelta, _float fMinDistance)
{
	_float3		vPosition = Get_State(STATE_POSITION);

	_float3		vDir = vTargetPoint - vPosition;


	_float		fDistance = D3DXVec3Length(&vDir);

	if (fDistance > fMinDistance)
	{
		vPosition += *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeedPerSec * fTimeDelta;
		Set_State(STATE_POSITION, vPosition);

		return false;
	}

	return true;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*		pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTransform"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	return nullptr;
}

void CTransform::Free()
{
	__super::Free();

}
