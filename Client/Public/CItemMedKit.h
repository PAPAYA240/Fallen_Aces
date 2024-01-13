#pragma once

#include "CHealItem.h"

class CItemMedKit final : public CHealItem
{
private:
	CItemMedKit(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CItemMedKit(const CItemMedKit& _rhs);
	virtual ~CItemMedKit() = default;

public:
	virtual HRESULT			Initialize_Prototype()			override;
	virtual HRESULT			Initialize(void* pArg)			override;
	virtual void			Tick(_float fTimeDelta)			override;
	virtual void			Late_Tick(_float fTimeDelta)	override;
	virtual HRESULT			Render()						override;

public:
	static	CItemMedKit*	Create(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg)	override;
	virtual void			Free()				override;

public:
	HRESULT					Add_MyComponents();
	HRESULT					Set_RenderState();
	HRESULT					Reset_RenderState();

private:
	CGameObject*			Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection = nullptr);

private:
	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Rect*			m_pVIbufferCom = { nullptr };
};

