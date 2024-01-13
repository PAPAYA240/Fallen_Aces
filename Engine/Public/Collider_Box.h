#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_Box final : public CCollider
{
private:
	CCollider_Box(LPDIRECT3DDEVICE9	pGraphic_Device);
	CCollider_Box(const CCollider_Box& rhs);
	virtual ~CCollider_Box() = default;

public:
	HRESULT Initialize(void* pArg) override;
	HRESULT Initialize_Prototype() override;

	HRESULT Render() override;

private:
	MIN_MAX_POS			m_tMinMaxPos = {};

public:
	//	bool로 리턴하고 출력매개변수 활용하기 
	_bool Check_Box(const class CTransform* pTargetTransform, _Out_ _float3* pDirection = nullptr);
	_bool Check_Sphere(const class CTransform* pTargetTransform, _Out_ _float3* pDirection = nullptr);

public:
	virtual HRESULT Update_ColliderTransform() override;

private:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

private:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = { nullptr };
	LPDIRECT3DINDEXBUFFER9			m_pIB = { nullptr };

	///* 정점의 위치를 보관하기위한 동적 배열 */
	///* 해당 콜라이더를 지닌 객체의 VIBuffer와 별도로 객체의 트랜스폼을 이용하여 가상의 충돌체를 설정 */
	//_float3*						m_pWorldVerticesPos = { nullptr };

	_uint							m_iVertexStride = { 0 };	//	정점하나의 크기 (Byte) 
	_uint							m_iNumVertices = { 0 };		//	정점의 갯수 
	_ulong							m_dwFVF = { 0 };			//	정점은 어떤 멤버들을 가지고 있는지
	_ulong							m_dwNumPrimitive = { 0 };	//	Primitive 원시적인, 근본적인

	_uint							m_iIndexStride = { 0 };
	_uint							m_iNumIndices = { 0 };
	D3DFORMAT						m_eIndexFormat = { };


public:
	static CCollider_Box* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END