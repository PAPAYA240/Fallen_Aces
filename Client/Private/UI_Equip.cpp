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
    return E_NOTIMPL;
}

HRESULT CUI_Equip::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CUI_Equip::Tick(_float fTimeDelta)
{
}

void CUI_Equip::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUI_Equip::Render()
{
    return E_NOTIMPL;
}

HRESULT CUI_Equip::Add_Components()
{
    return E_NOTIMPL;
}

HRESULT CUI_Equip::Set_RenderState()
{
    return E_NOTIMPL;
}

HRESULT CUI_Equip::Reset_RenderState()
{
    return E_NOTIMPL;
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
