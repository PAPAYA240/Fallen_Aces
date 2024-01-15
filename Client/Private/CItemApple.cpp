#include "stdafx.h"
#include "CItemApple.h"

CItemApple::CItemApple(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CHealItem(_pGraphic_Device)
{
}

CItemApple::CItemApple(const CItemApple& _rhs)
	: CHealItem(_rhs)
{
}

HRESULT CItemApple::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItemApple::Initialize(void* pArg)
{

	CHealItem::HEAL_ITEM_DESC MyDesc = {};

	if (nullptr != pArg)
	{
		CHealItem::HEAL_ITEM_DESC* pDesc = (CHealItem::HEAL_ITEM_DESC*)pArg;
		MyDesc.pTerrainTransform = pDesc->pTerrainTransform;
		MyDesc.pTerrainVIBuffer = pDesc->pTerrainVIBuffer;
	}

	m_eBillbordType = XYZ;

	MyDesc.eItemID = { ITEM_ID::APPLE };
	
	MyDesc.fAttackDamage = { 0.f };
	MyDesc.fDurability = { 1.f };

	MyDesc.fRotationPerSec = { D3DXToRadian(90.f) };
	MyDesc.fSpeedPerSec = { 5.f };

	MyDesc.fRecorvery_Buff = { 5.f };
	MyDesc.fRBuff_LimitTime = { 5.f };

	MyDesc.fSpeed_Buff = { 0.f };
	MyDesc.fSBuff_LimitTime = { 0.f };

	if (FAILED(__super::Initialize(&MyDesc)))
		return E_FAIL;

	if (FAILED(Add_MyComponents()))
		return E_FAIL;

	m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Apple"));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(_float(rand() % 100), 5.f, _float(rand() % 100)));
	m_pTransformCom->Set_Scaled(0.3f, 0.3f, 1.f);

	return S_OK;
}

void CItemApple::Tick(_float fTimeDelta)
{
	Check_PlayerRadius();
	Check_Durability();
	Check_PlayerRadius();

	if (FAILED(SetUp_OnTerrain(m_pTransformCom->Get_Scaled().y * 0.5f)))
		return;
}

void CItemApple::Late_Tick(_float fTimeDelta)
{
	BillBoard_Camera();

	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CItemApple::Render()
{
	if (true == m_isTestDead)
		return E_FAIL;

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

CItemApple* CItemApple::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	CItemApple* pInstance = new CItemApple(_pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Apple "));

		Safe_Release<CItemApple*>(pInstance);
	}

	return pInstance;
}

CGameObject* CItemApple::Clone(void* pArg)
{
	CItemApple* pInstance = new CItemApple(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Apple Clone "));

		Safe_Release<CItemApple*>(pInstance);
	}

	return pInstance;
}

void CItemApple::Free()
{
	__super::Free();
}

HRESULT CItemApple::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CItemApple::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CItemApple::Reset_RenderState()
{
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CGameObject* CItemApple::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{


	return nullptr;
}
