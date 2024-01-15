#include "stdafx.h"
#include "Camera_Object.h"

CCamera_Object::CCamera_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CCamera(pGraphic_Device)
{
}

CCamera_Object::CCamera_Object(const CCamera_Object& rhs)
    :CCamera(rhs)
{
}

HRESULT CCamera_Object::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Object::Initialize(void* pArg)
{
	wstring strTargetTag;

	if (nullptr != pArg)
	{
		CAMERA_OBJECT_DESC* pCameraDesc = (CAMERA_OBJECT_DESC*)pArg;
		m_fMouseSensor = pCameraDesc->fMouseSensor;
		strTargetTag = pCameraDesc->strTargetTag;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	GetCursorPos(&m_ptMouse);

	m_TargetTrans = (CTransform*)(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, strTargetTag, g_strTransformTag));

	return S_OK;
}

void CCamera_Object::Tick(_float fTimeDelta)
{
	if (!GetFocus())
		return;

	POINT		ptMouse = {};

	/* 스크린 기준의 마우스 위치를 얻어온다. */
	GetCursorPos(&ptMouse);

	/* 왼쪽으로 움직이면 -, 오른쪽으로 움직이면 + */
	_long MouseMoveX = ptMouse.x - m_ptMouse.x;

	if (0 != MouseMoveX)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMoveX * m_fMouseSensor);
		if (m_TargetTrans != nullptr)
			m_TargetTrans->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMoveX * m_fMouseSensor);
	}

	_long MouseMoveY = ptMouse.y - m_ptMouse.y;

	if (0 != MouseMoveY)
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMoveY * m_fMouseSensor);
		if (m_TargetTrans != nullptr)
			m_TargetTrans->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMoveY * m_fMouseSensor);
	}

	if (m_TargetTrans != nullptr)
	{
		_float3 vPos = m_TargetTrans->Get_State(CTransform::STATE_POSITION);
		vPos.y += m_TargetTrans->Get_Scaled().y * 0.25f;
		/*_float3 vLook = m_TargetTrans->Get_State(CTransform::STATE_LOOK);
		vLook.y = 0.f;
		vPos -= vLook * 5.f;*/

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

	if (FAILED(__super::Bind_PipeLines()))
		return;

	/* Mouse Position */
	ptMouse = { _long(g_iWinSizeX * 0.5f), _long(g_iWinSizeY * 0.5f) };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);

	m_ptMouse = ptMouse;
}

void CCamera_Object::Late_Tick(_float fTimeDelta)
{
	
}

CCamera_Object* CCamera_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Object* pInstance = new CCamera_Object(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : Camera_Object Class"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Object::Clone(void* pArg)
{
	CCamera_Object* pInstance = new CCamera_Object(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned :Camera_Object Class"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Object::Free()
{
	__super::Free();
}
