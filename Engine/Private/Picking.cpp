#include "..\Public\Picking.h"
#include "Transform.h"

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CPicking::Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
	m_hWnd = hWnd;

	m_iWinSizeX = iWinSizeX;
	m_iWinSizeY = iWinSizeY;

	return S_OK;
}

void CPicking::Update()
{
	/* 매번 다른 폴리곤과 충돌으 ㄹ할때마다 마우스좌표를 다시 구해서 역으로 로컬까지 내려가느 ㄴ과정이 너무 길기때문에. 
	모든 폴리곤모델과 충돌할때 공통적으로 거쳐가는 공간(뷰포트->월드)까지는 미리 구해놓ㅈ나. */
	/* 모든 모델들이 공통으로 존재하는 월드 스페이스 상의 마우스레이, 마우스 레이의 시작점을 구해놓자. */
	POINT			ptMouse;
	GetCursorPos(&ptMouse);

	/* 뷰포트 상의 마우스 위치를 구하자. */
	ScreenToClient(m_hWnd, &ptMouse);


	/* 0, 0 -> -1, 1 */
	/* wincx * 0.5f, wincy * 0.5f -> 0, 0 */
	/* wincx, wincy -> 1, -1 */

	_float3		vMousePos;

	/* 투영스페이스 상의 마우스 위치를 구하자. */
	vMousePos.x = ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(m_iWinSizeY * 0.5f) + 1.f;
	vMousePos.z = 0.f;	

	_float4x4	ProjMatrixInv;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrixInv);
	D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrixInv);

	/* 뷰스페이스 상의 마우스 위치를 구했다. */
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &ProjMatrixInv);

	/* 뷰스페이스 상의 마우스 레이, 마우스 레이의 시작점을 구하자. */
	m_vRayDir = vMousePos - _float3(0.f, 0.f, 0.f);
	m_vRayPos = _float3(0.f, 0.f, 0.f);

	_float4x4	ViewMatrixInv;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrixInv);
	D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);

	/* 월드스페이스 상의 마우스 위치를 구했다. */
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &ViewMatrixInv);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &ViewMatrixInv);
}

void CPicking::Transform_PickingToLocalSpace(const CTransform * pTransform, _Out_ _float3* pRayDir, _Out_ _float3* pRayPos)
{	
	_float4x4	WorldMatrixInv = *pTransform->Get_WorldMatrix_Inverse();

	D3DXVec3TransformNormal(pRayDir, &m_vRayDir, &WorldMatrixInv);
	D3DXVec3TransformCoord(pRayPos, &m_vRayPos, &WorldMatrixInv);
}

CPicking * CPicking::Create(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
	CPicking*		pInstance = new CPicking(pGraphic_Device);

	if (FAILED(pInstance->Initialize(hWnd, iWinSizeX, iWinSizeY)))
	{
		MSG_BOX(TEXT("Failed To Created : CPicking"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicking::Free()
{
	Safe_Release(m_pGraphic_Device);
}

