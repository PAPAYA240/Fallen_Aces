#pragma once

#include "Component.h"

/* 1.객체의 월드공간에서의 상태를 표현한다(월드변환행렬) */
/* 2. 월드공간에서의 움직임에 대한 처리 기능 */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}TRANSFORM_DESC;

private:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTransform() = default;

public:
	_float3 Get_State(STATE eState) const {
		return *(_float3*)&m_WorldMatrix.m[eState][0];
	}

	_float3 Get_Scaled() const {
		return _float3(
			D3DXVec3Length(&Get_State(STATE_RIGHT)), 
			D3DXVec3Length(&Get_State(STATE_UP)),
			D3DXVec3Length(&Get_State(STATE_LOOK)));
	}

	const _float4x4* Get_WorldMatrix() const {
		return &m_WorldMatrix;
	}

	_float4x4* Get_WorldMatrix_Inverse() const {
		_float4x4	InverseMatrix;
		return D3DXMatrixInverse(&InverseMatrix, nullptr, &m_WorldMatrix);
	}

	void Set_State(STATE eState, const _float3& vState) {
		memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof(_float3));
	}

	void Set_Scaled(_float fX, _float fY, _float fZ);

	void NormalizeScale(const POINT& _ptSize);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	/* 월드스페이스 변환행려을 장치에 바인딩한다. */
	/* 바인딩한 행렬은 추후 그리는 정점에 알아서 곱해지게 될 것이다. */
	HRESULT Bind_WorldMatrix();

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Turn(const _float3& vAxis, _float fTimeDelta);
	void LookAt(const _float3& vAtPoint);
	void LookAt_ForLandObject(const _float3& vAtPoint);
	_bool MoveToTarget(const CTransform* pTargetTransform, _float fTimeDelta, _float fMinDistance = 0.f);
	_bool MoveToTarget(const _float3& vTargetPoint, _float fTimeDelta, _float fMinDistance = 0.f);

public:
	void Refresh_ToLook(const _float3& _vLookVec) { m_vToLookAt = _vLookVec; }
	void Refresh_ToLook();

	_float3 Get_ToLook() const { return m_vToLookAt; }

private:
	_float4x4			m_WorldMatrix;
	_float3				m_vScale = { 1.f, 1.f, 1.f };

	_float3				m_vToLookAt = { 0.f, 0.f, 0.f };
	_float				m_fRotationPerSec = { 0.0f };
	_float				m_fSpeedPerSec = { 0.0f };

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END