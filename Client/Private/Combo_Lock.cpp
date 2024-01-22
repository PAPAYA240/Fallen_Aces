#include "stdafx.h"
#include "Combo_Lock.h"
#include "Number.h"
#include "Combo_Lock_Obj.h"

CCombo_Lock::CCombo_Lock(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CGameObject(_pGraphic_Device)
{
}

CCombo_Lock::CCombo_Lock(const CCombo_Lock& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CCombo_Lock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCombo_Lock::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC  MyDesc = {};

	if (nullptr != pArg)
		CGameObject::GAMEOBJECT_DESC* pDesc = (CGameObject::GAMEOBJECT_DESC*)pArg;

	if (FAILED(__super::Initialize(&MyDesc)))
		return E_FAIL;

	if (FAILED(Add_MyComponents()))
		return E_FAIL;

	m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Combo_Look"));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(17.f, 1.f, 17.f));
	m_pTransformCom->Set_Scaled(0.5f, 0.2f, 1.f);

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	CCombo_Lock::LOCK_DESC	LockDesc = {};

	LockDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	LockDesc.fDistance = 0.16f; 
	LockDesc.fObj_Distance = -0.12f;
	LockDesc.eNum = THIRED;
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Number"), TEXT("Prototype_GameObject_Number"), &LockDesc)))
		return E_FAIL;

	LockDesc.fObj_Distance = 0.0f;
	LockDesc.fDistance = 0.01f;
	LockDesc.eNum = SECEND;
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Number"), TEXT("Prototype_GameObject_Number"), &LockDesc)))
		return E_FAIL;

	LockDesc.fDistance = -0.14f;
	LockDesc.fObj_Distance = 0.12f;
	LockDesc.eNum = FIRST;
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Number"), TEXT("Prototype_GameObject_Number"), &LockDesc)))
		return E_FAIL;

	return S_OK;
}

void CCombo_Lock::Tick(_float fTimeDelta)
{
	CGameObject* pNumber = m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Number"), 0);
	CGameObject* pObj = m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_LockObj"), 0);

	if(!dynamic_cast<CNumber*>(pNumber)->Get_Correct())
	{
		if (dynamic_cast<CCombo_Lock_Obj*>(pObj)->Get_Access())
			m_bProj = !m_bProj;

		if (m_bProj)
			ProjTrans();
	}
}

void CCombo_Lock::Late_Tick(_float fTimeDelta)
{
	if(m_bProj)
		m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CCombo_Lock::Render()
{
	CGameObject* p = m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Number"), 0);

	if(!dynamic_cast<CNumber*>(p)->Get_Correct() && m_bProj)
	{
		if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
			return E_FAIL;

		m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
		m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
		

		if (FAILED(m_pTextureCom->Bind_Texture(0, 0)))
			return E_FAIL;

		if (FAILED(Set_RenderState()))
			return E_FAIL;

		if (FAILED(m_pVIbufferCom->Render()))
			return E_FAIL;

		if (FAILED(Reset_RenderState()))
			return E_FAIL;
	}

	return S_OK;
}


void CCombo_Lock::Origin_Trans()
{
}

HRESULT CCombo_Lock::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCombo_Lock::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CCombo_Lock::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CCombo_Lock::ProjTrans()
{
	D3DXMatrixIdentity(&m_OriginalMatrix);
	m_vOrthoProjectedPoint = _float3(m_fX, m_fY, 0.f);

	D3DXMatrixIdentity(&m_ViewMatrix);

	/* 직교투영행렬을 만들어주는 함수. */
	/* 직교투영을 수행하여 투영스페이로 옮겨주기위한 영역(뷰볼륨whnf)을 설정해달라. */
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, -100., 100.0f);

	m_fX = g_iWinSizeX * 0.01f;
	m_fY = g_iWinSizeY * 0.05f;

	m_pTransformCom->Set_Scaled(850.f, 600.f, 100.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX ,m_fY,0.f));
}


CCombo_Lock* CCombo_Lock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCombo_Lock* pInstance = new CCombo_Lock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCombo_Lock"));

		Safe_Release<CCombo_Lock*>(pInstance);
	}

	return pInstance;
}

CGameObject* CCombo_Lock::Clone(void* pArg)
{
	CCombo_Lock* pInstance = new CCombo_Lock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCombo_Lock Clone "));

		Safe_Release<CCombo_Lock*>(pInstance);
	}
	return pInstance;
}

CGameObject* CCombo_Lock::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;
}

void CCombo_Lock::Free()
{
	Safe_Release<CTexture*>(m_pTextureCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIbufferCom);

	__super::Free();
}
