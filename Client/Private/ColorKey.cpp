#include "stdafx.h"
#include "ColorKey.h"

CColorKey::CColorKey(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CKeyObject(_pGraphic_Device)
{
}

CColorKey::CColorKey(const CColorKey& rhs)
	: CKeyObject(rhs)
{
}

HRESULT CColorKey::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CColorKey::Initialize(void* pArg)
{
	CKeyObject::KEY_DESC MyDesc = {};

	if (nullptr != pArg)
	{
		CKeyObject::KEY_DESC* pDesc = (CKeyObject::KEY_DESC*)pArg;
		MyDesc.pTerrainTransform = pDesc->pTerrainTransform;
		MyDesc.pTerrainVIBuffer = pDesc->pTerrainVIBuffer;
		m_eColor = MyDesc.eColor = pDesc->eColor;
	}
	
	if (FAILED(__super::Initialize(&MyDesc)))
		return E_FAIL;

	if (FAILED(Add_MyComponents()))
		return E_FAIL;

	m_ePuzzleID = PUZZLE_ID::COLOR_KEY;
	Check_KeyColor();

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(_float(rand() % 100), 1.f, _float(rand() % 100)));
	m_pTransformCom->Set_Scaled(0.6f, 0.3f, 1.f);

	return S_OK;
}

void CColorKey::Tick(_float fTimeDelta)
{
	//Check_Switch();
	Key_Control();
}

void CColorKey::Late_Tick(_float fTimeDelta)
{
	BillBoard_Camera();
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CColorKey::Render()
{
	if (!m_bGetKey)
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
	}

	return S_OK;
}

void CColorKey::Check_KeyColor()
{
	if (COLOR::BULE == m_eColor)
		m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Key_Blue"));

	else if (COLOR::GREEN == m_eColor)
		m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Key_Green"));

	else if (COLOR::RED == m_eColor)
		m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Key_Red"));

	else if (COLOR::YELLOW == m_eColor)
		m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Key_Yellow"));
}

HRESULT CColorKey::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CColorKey::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CColorKey::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}


CColorKey* CColorKey::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CColorKey* pInstance = new CColorKey(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CColorKey "));

		Safe_Release<CColorKey*>(pInstance);
	}

	return pInstance;
}

CGameObject* CColorKey::Clone(void* pArg)
{
	CColorKey* pInstance = new CColorKey(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CButten_Switch Clone "));

		Safe_Release<CColorKey*>(pInstance);
	}
	return pInstance;
}


CGameObject* CColorKey::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;

}

void CColorKey::Free()
{
	__super::Free();
}
