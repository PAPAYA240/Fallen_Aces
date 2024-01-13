#pragma once

#include "Component.h"

#define COLLIDER_BOX_VERTICES 8

BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
public:
	typedef struct tagMinMaxPos
	{
		_float3		vMinPos;
		_float3		vMaxPos;
	}MIN_MAX_POS;

public:
	typedef struct tagColiderDesc
	{
		class CTransform* pObjectTransform;
	}COLLIDER_DESC;

protected:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() = 0;

public:
	virtual HRESULT Update_ColliderTransform() = 0;

protected:
	virtual HRESULT Set_RenderState();
	virtual HRESULT Reset_RenderState();

protected:
	MIN_MAX_POS Compute_MinPos_MaxPos(const class CTransform* pTransform) const;

protected:
	class CTransform* m_pObjectTransform = { nullptr };
	class CTransform* m_pColliderTransform = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END