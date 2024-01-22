#pragma once

#include "Client_Defines.h"
#include "BlendObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEffect final : public CBlendObject
{
private:
	CEffect(LPDIRECT3DDEVICE9 pGraphic_Device);	
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public :
	void	Set_Active(_bool _active)	{ m_bActive = _active; }
	_bool	Get_Active()				{ return m_bActive; }
	void    Set_Position(_float3 _pos) { m_pTransformCom->Set_State(CTransform::STATE_POSITION, _pos); }
	

private:		
	CTexture*					m_pTextureCom = { nullptr };
	CVIBuffer_Cube*				m_pVIBufferCom = { nullptr };

	_float						m_fFrame	= { 0.f };
	_bool						m_bActive	= { false };
	_float3						m_fPosition = { };
	_uint						m_iFont_Count;

private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_Components();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};

END