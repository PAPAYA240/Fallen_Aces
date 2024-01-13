#pragma once

#include "CHealItem.h"

class CItemBanana final : public CHealItem
{
private:
	CItemBanana(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CItemBanana(const CItemBanana& _rhs);
	virtual ~CItemBanana() = default;

public:
	virtual HRESULT			Initialize_Prototype()			override;
	virtual HRESULT			Initialize(void* pArg)			override;
	virtual void			Tick(_float fTimeDelta)			override;
	virtual void			Late_Tick(_float fTimeDelta)	override;
	virtual HRESULT			Render()						override;

public:
	static	CItemBanana*	Create(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg)	override;
	virtual void			Free()				override;

public:
	HRESULT					Add_MyComponents();
	HRESULT					Set_RenderState();
	HRESULT					Reset_RenderState();

private:
	CGameObject*			Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection = nullptr);
};

