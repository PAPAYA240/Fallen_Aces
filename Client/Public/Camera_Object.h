#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Object final : public CCamera
{
public:
	typedef struct CAMERA_OBJECT_DESCRIPTION : public CCamera::CAMERA_DESC
	{
		_float		fMouseSensor = { 1.f };
		wstring		strTargetTag = TEXT("");

	}CAMERA_OBJECT_DESC;

private:
	CCamera_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera_Object(const CCamera_Object& rhs);
	virtual ~CCamera_Object() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

private:
	_float					m_fMouseSensor = { 1.0f };
	POINT					m_ptMouse = {};
	CTransform*				m_TargetTrans = {nullptr};

public:
	static CCamera_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
