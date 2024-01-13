#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
private:
	CPicking(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	void Update();
	void Transform_PickingToLocalSpace(const class CTransform * pTransform, _Out_ _float3 * pRayDir, _Out_ _float3 * pRayPos);

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = { nullptr };
	HWND					m_hWnd;
	_uint					m_iWinSizeX, m_iWinSizeY;

	_float3					m_vRayDir, m_vRayPos;	

public:
	static CPicking* Create(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	virtual void Free() override;
};

END