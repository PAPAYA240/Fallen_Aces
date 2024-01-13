#include "..\Public\Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	CAMERA_DESC*		pCameraDesc = (CAMERA_DESC*)pArg;

	m_fFovy = pCameraDesc->fFovy;
	m_fAspect = pCameraDesc->fAspect;
	m_fNear = pCameraDesc->fNear;
	m_fFar = pCameraDesc->fFar;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pCameraDesc->vEye);
	m_pTransformCom->LookAt(pCameraDesc->vAt);

	return S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
}

void CCamera::Late_Tick(_float fTimeDelta)
{
}


HRESULT CCamera::Bind_PipeLines()
{	
	_float4x4		ViewMatrix = *m_pTransformCom->Get_WorldMatrix_Inverse();
	/* SetTransform함수는 한번 바인딩 되면 재 바인딩될때 까지 계속 값을 유지한다. */
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);

	_float4x4		ProjMatrix;
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_fFovy, m_fAspect, m_fNear, m_fFar));

	return S_OK;
}

CAMERA_DIR CCamera::Get_CameraDot(const CTransform* _pOtherTrans)
{
	_float3 vOtherPos = _pOtherTrans->Get_State(CTransform::STATE_POSITION);
	_float3 vCameraPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vOtherLook = _pOtherTrans->Get_ToLook();
	_float3 vCameraDir = vOtherPos - vCameraPos;

	_float4x4 matViewInv = *m_pTransformCom->Get_WorldMatrix_Inverse();

	CAMERA_DIR	eDir = { CAMERA_DIR::_END };

	D3DXVec3Normalize(&vOtherLook, &vOtherLook);
	D3DXVec3Normalize(&vCameraDir, &vCameraDir);

	D3DXVec3TransformNormal(&vOtherLook, &vOtherLook, &matViewInv);
	D3DXVec3TransformNormal(&vCameraDir, &vCameraDir, &matViewInv);

	_float fDot = D3DXVec3Dot(&vOtherLook, &vCameraDir);

	_float3 vOtherRight = *D3DXVec3Cross(&vOtherRight, &_float3(0.f, 1.f, 0.f), &vOtherLook);

	_float fRightDot = D3DXVec3Dot(&vOtherRight, &vCameraDir);

	if ((0.2f < fDot && fDot <= 0.85f) && 0 > fRightDot)
		eDir = CAMERA_DIR::SOUTH_EAST;
	else if ((0.2f < fDot && fDot <= 0.85f) && 0 < fRightDot)
		eDir = CAMERA_DIR::SOUTH_WEST;
	else if ((0.85f < fDot && fDot <= 1.f))
		eDir = CAMERA_DIR::SOUTH;
	else if ((-0.2f < fDot && fDot <= 0.2f) && 0 < fRightDot)
		eDir = CAMERA_DIR::WEST;
	else if ((-0.2f < fDot && fDot <= 0.2f) && 0 > fRightDot)
		eDir = CAMERA_DIR::EAST;
	else if ((-0.85f < fDot && fDot <= -0.2f) && 0 > fRightDot)
		eDir = CAMERA_DIR::NORTH_EAST;
	else if ((-0.85f < fDot && fDot <= -0.2f) && 0 < fRightDot)
		eDir = CAMERA_DIR::NORTH_WEST;
	else if ((-1.f < fDot && fDot <= -0.85f))
		eDir = CAMERA_DIR::NORTH;

	return eDir;
}

void CCamera::Free()
{
	__super::Free();
}
