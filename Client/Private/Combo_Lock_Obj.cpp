#include "stdafx.h"
#include "Combo_Lock_Obj.h"
#include "Combo_Lock.h"

CCombo_Lock_Obj::CCombo_Lock_Obj(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CGameObject(_pGraphic_Device)
{
}

CCombo_Lock_Obj::CCombo_Lock_Obj(const CCombo_Lock_Obj& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CCombo_Lock_Obj::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCombo_Lock_Obj::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC  MyDesc = {};

	if (nullptr != pArg)
		CGameObject::GAMEOBJECT_DESC* pDesc = (CGameObject::GAMEOBJECT_DESC*)pArg;

	if (FAILED(__super::Initialize(&MyDesc)))
		return E_FAIL;

	if (FAILED(Add_MyComponents()))
		return E_FAIL;

	m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Combo_Look"));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(9.f, 1.f, 9.f));
	m_pTransformCom->Set_Scaled(0.5f, 0.2f, 1.f);
	
	return S_OK;
}

void CCombo_Lock_Obj::Tick(_float fTimeDelta)
{
	Access_LockObj();
}

void CCombo_Lock_Obj::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CCombo_Lock_Obj::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0, 0)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	if (FAILED(m_pVIbufferCom->Render()))
		return E_FAIL;

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}


HRESULT CCombo_Lock_Obj::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCombo_Lock_Obj::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CCombo_Lock_Obj::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CCombo_Lock_Obj::Access_LockObj()
{
	CGameObject* pObj = m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_PadLock"), 0);

	const CTransform* pPlayerTransform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDiff = vMyPos - vPlayerPos;

	_float fDistance = D3DXVec3Length(&vDiff);

	if (abs(fDistance) <= 2.0f)
	{
		if (DOWN == m_pGameInstance->Get_KeyState('Q'))
			m_bAccess = true;
		else
			m_bAccess = false;

	}

}


CCombo_Lock_Obj* CCombo_Lock_Obj::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCombo_Lock_Obj* pInstance = new CCombo_Lock_Obj(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCombo_Lock_Obj"));

		Safe_Release<CCombo_Lock_Obj*>(pInstance);
	}

	return pInstance;
}

CGameObject* CCombo_Lock_Obj::Clone(void* pArg)
{
	CCombo_Lock_Obj* pInstance = new CCombo_Lock_Obj(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCombo_Lock_Obj Clone "));

		Safe_Release<CCombo_Lock_Obj*>(pInstance);
	}
	return pInstance;
}

CGameObject* CCombo_Lock_Obj::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;
}

void CCombo_Lock_Obj::Free()
{
	Safe_Release<CTexture*>(m_pTextureCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIbufferCom);

	__super::Free();
}
