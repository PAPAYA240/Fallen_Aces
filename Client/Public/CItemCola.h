#pragma once

#include "CHealItem.h"

class CItemCola final : public CHealItem
{
private:
	CItemCola(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CItemCola(const CItemCola& _rhs);
	virtual ~CItemCola() = default;

public:
	virtual HRESULT			Initialize_Prototype()			override;
	virtual HRESULT			Initialize(void* pArg)			override;
	virtual void			Tick(_float fTimeDelta)			override;
	virtual void			Late_Tick(_float fTimeDelta)	override;
	virtual HRESULT			Render()						override;

public:
	static	CItemCola* Create(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)	override;
	virtual void			Free()				override;

public:
	HRESULT					Add_MyComponents();
	HRESULT					Set_RenderState();
	HRESULT					Reset_RenderState();

private:
	CGameObject* Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection = nullptr);

private:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIbufferCom = { nullptr };
};

