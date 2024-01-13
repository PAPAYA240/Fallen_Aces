#include "stdafx.h"
#include "UI_Equip.h"

CUI_Equip::CUI_Equip(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUI(pGraphic_Device)
{
}

CUI_Equip::CUI_Equip(CUI_Equip& rhs)
    :CUI(rhs)
{
}

HRESULT CUI_Equip::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Equip::Initialize(void* pArg)
{
    UI_EQUIP_DESC* Desc = (UI_EQUIP_DESC*)pArg;
    m_pEquipItem = Desc->pEquipItem;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    Initialize_UI_Setting(g_iWinSizeX, g_iWinSizeY);

    m_pTextureCom->Change_Container(TEXT("UI"), TEXT("Inven_Icons"));

    return S_OK;
}

void CUI_Equip::Tick(_float fTimeDelta)
{
}

void CUI_Equip::Late_Tick(_float fTimeDelta)
{
    if (nullptr != *m_pEquipItem)
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this);
    }
}

HRESULT CUI_Equip::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

    if (FAILED(m_pTextureCom->Bind_Texture(0, 0)))
        return E_FAIL;

    if (FAILED(Set_RenderState()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    if (FAILED(Reset_RenderState()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Equip::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Equip::Set_RenderState()
{
    /* 알파테스트를 한다. (그려지는 픽셀들의 알파를 검사하여 그릴지 안그릴지를 판단한다.) */
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    /* 내가 설정한 알파는 0이다. */
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);

    /* 그려지는 픽세르이 알파가 내가 설정한 알파보다 클때만 그려라. */
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CUI_Equip::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

CUI_Equip* CUI_Equip::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUI_Equip* pInstance = new CUI_Equip(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed To Created : UI_Equip Class"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Equip::Clone(void* pArg)
{
    CUI_Equip* pInstance = new CUI_Equip(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed To Cloned : UI_Equip Class"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Equip::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
}
