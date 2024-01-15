#include "stdafx.h"
#include "UI_Select.h"

CUI_Select::CUI_Select(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_Select::CUI_Select(CUI_Select& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Select::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Select::Initialize(void* pArg)
{
	UI_SELECT_DESC* Desc = (UI_SELECT_DESC*)pArg;
	for (size_t i = 0; i < 3; i++)
	{
		m_SlotPosArr[i] = Desc->SlotPosArr[i];
	}
	m_pSelectNum = Desc->pSelectNum;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	Initialize_UI_Setting(g_iWinSizeX, g_iWinSizeY);

	return S_OK;
}

void CUI_Select::Tick(_float fTimeDelta)
{
	if (m_iCurSelect != *m_pSelectNum)
	{
		m_iCurSelect = *m_pSelectNum;
		m_vPos = { m_SlotPosArr[*m_pSelectNum].x, m_SlotPosArr[*m_pSelectNum].y , m_vPos.z };
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);

		Initialize_UI_Setting(g_iWinSizeX, g_iWinSizeY);
	}
}

void CUI_Select::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Select::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_Texture(0, 2)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Select::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Select::Set_RenderState()
{
	/* 알파테스트를 한다. (그려지는 픽셀들의 알파를 검사하여 그릴지 안그릴지를 판단한다.) */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	/* 내가 설정한 알파는 0이다. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);

	/* 그려지는 픽세르이 알파가 내가 설정한 알파보다 클때만 그려라. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CUI_Select::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CUI_Select* CUI_Select::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Select* pInstance = new CUI_Select(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : UI_Select Class"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Select::Clone(void* pArg)
{
	CUI_Select* pInstance = new CUI_Select(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : UI_Select Class"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Select::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
