#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CUI abstract : public CGameObject
{
public:
	typedef struct tagUIDescription : public CGameObject::GAMEOBJECT_DESC
	{
		_float3 vPos;
		_float2 vSize;

	}UI_DESC;

protected:
	CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI(const CUI& rhs);
	~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

protected:
	// 객체 Initialize에서 Add Component 이후에 호출시 필요한 행렬과 정보를 세팅해줌
	void Initialize_UI_Setting(_uint iWinSizeX, _uint iWinSizeY);

protected:
	// 렌더링 때 z sorting을 위한 z값 부여
	_float3						m_vPos;
	_float2						m_vSize;
	_float4x4					m_ViewMatrix, m_ProjMatrix;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END