#pragma once
#include <SwitchObj.h>

BEGIN(Client)
class CLight_Switch final : public CSwitchObj
{
private :
	CLight_Switch(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CLight_Switch(const CLight_Switch& rhs);
	virtual ~CLight_Switch() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Tick(_float fTimeDelta)			override;
	virtual void			Late_Tick(_float fTimeDelta)	override;
	virtual HRESULT			Render()						override;

	
public:
	HRESULT					Add_MyComponents();
	HRESULT					Set_RenderState();
	HRESULT					Reset_RenderState();

	static	CLight_Switch* Create(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg)	override;
	CGameObject*			Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection);
	virtual void			Free()				override;

};
END

