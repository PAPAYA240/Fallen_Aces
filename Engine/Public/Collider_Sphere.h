#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_Sphere final : public CCollider
{
private:
	CCollider_Sphere(LPDIRECT3DDEVICE9	pGraphic_Device);
	CCollider_Sphere(const CCollider_Sphere& rhs);
	virtual ~CCollider_Sphere() = default;

public:
	HRESULT Initialize(void* pArg) override;
	HRESULT Initialize_Prototype() override;

	virtual HRESULT Render() override;

public:
	_bool Check_Sphere(const class CTransform* pTargetTransform, _Out_ _float3* pDirection = nullptr);
	_bool Check_Box(const class CTransform* pTargetTransform, _Out_ _float3* pDirection = nullptr);

public:
	virtual HRESULT Update_ColliderTransform() override;

private:
	LPD3DXMESH			m_pSphereMesh = { nullptr };

public:
	static CCollider_Sphere* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END