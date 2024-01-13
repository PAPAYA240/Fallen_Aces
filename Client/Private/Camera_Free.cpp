#include "stdafx.h"
#include "..\Public\Camera_Free.h"

CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void * pArg)
{
	if (nullptr != pArg)
	{
		CAMERA_FREE_DESC*		pCameraDesc = (CAMERA_FREE_DESC*)pArg;
		m_fMouseSensor = pCameraDesc->fMouseSensor;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	GetCursorPos(&m_ptMouse);

	return S_OK;
}

void CCamera_Free::Tick(_float fTimeDelta)
{
	if (!GetFocus())
		return;

	POINT		ptMouse = {};

	/* 스크린 기준의 마우스 위치를 얻어온다. */
	GetCursorPos(&ptMouse);

	if (GetKeyState('A') &	0x8000)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (GetKeyState('W') & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	/* 왼쪽으로 움직이면 -, 오른쪽으로 움짖ㄱ였다 + */
	_long MouseMoveX = ptMouse.x - m_ptMouse.x;

	if (0 != MouseMoveX)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMoveX * m_fMouseSensor);
	}

	_long MouseMoveY = ptMouse.y - m_ptMouse.y;

	if (0 != MouseMoveY)
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMoveY * m_fMouseSensor);
	}

	if (FAILED(__super::Bind_PipeLines()))
		return;

	/* Mouse Position */
	ptMouse = { _long(g_iWinSizeX * 0.5f), _long(g_iWinSizeY * 0.5f) };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);

	m_ptMouse = ptMouse;
}

void CCamera_Free::Late_Tick(_float fTimeDelta)
{
}


CCamera_Free * CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free*		pInstance = new CCamera_Free(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCamera_Free"));

		Safe_Release(pInstance);
	}

	return pInstance;
}
/* 사본객체를 생성하기위한 함수에요. */
CGameObject * CCamera_Free::Clone(void* pArg)
{
	CCamera_Free*		pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CCamera_Free"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();

}
