#include "stdafx.h"
#include "..\Public\Effect.h"

#include "GameInstance.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{

}

CEffect::CEffect(const CEffect & rhs)
	: CBlendObject(rhs)
{

}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTextureCom->Change_Container(TEXT("Effect"), TEXT("Blood_DropImpact"));

	m_pTransformCom->Set_Scaled(3.f, 1.f, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(_float(rand() % 60), 10.f, _float(rand() % 60)));

	return S_OK;
}

void CEffect::Tick(_float fTimeDelta)
{
	_uint iMaxSize = m_pTextureCom->Container_MaxSize();

	m_fFrame += iMaxSize * fTimeDelta;

	if (m_fFrame >= iMaxSize)
		m_fFrame = 0.f;

	Compute_ViewZ();
}

void CEffect::Late_Tick(_float fTimeDelta)
{	
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
}

HRESULT CEffect::Render()
{	
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;		

	if (FAILED(m_pTextureCom->Bind_Texture(0, (_uint)m_fFrame)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::Add_Components()
{	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CEffect::Set_RenderState()
{	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	return S_OK;
}

HRESULT CEffect::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	

	return S_OK;
}

/* 원형객첼르 생성하기위한 함수에요. */
CEffect * CEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect*		pInstance = new CEffect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CEffect"));

		Safe_Release(pInstance);
	}

	return pInstance;
}
/* 사본객체를 생성하기위한 함수에요. */
CGameObject * CEffect::Clone(void* pArg)
{	
	CEffect*		pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CEffect"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}
