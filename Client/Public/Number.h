#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Combo_Lock.h"

BEGIN(Client)
class CNumber final : public CGameObject
{
private:
	CNumber(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CNumber(const CNumber& rhs);
	virtual ~CNumber() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_MyComponents();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

	void	ProjTrans();
	void	Access_Number();
public :
	_bool Get_Correct()			{ return m_bCorrect; }
	_bool Get_LockUn()			{ return m_LockUn; }

private:
	_bool						m_bProj		= { false };
	_bool						m_bCorrect	= { false };
	_bool						m_bAllStop = { false };
	_bool						m_LockUn	= false;

	_float4x4					m_ViewMatrix, m_ProjMatrix;
	_float						m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float						m_fCorrect_Time = { 0.0f };
	_float						m_fTime = { 0.0f };

	_float3						m_vPos;
	_float						m_fDistance;
	_uint						m_iCorrect_Cnt;
	CCombo_Lock::NUMBER			m_eNumber;
	CCombo_Lock::NUMBER			m_eCurrentNumber;

private:
	CTexture*		m_pTextureCom	= { nullptr };
	CVIBuffer_Rect* m_pVIbufferCom = { nullptr };

	_float			m_fFrame		= { 0.0f };


public:
	static CNumber* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override;

	CGameObject* Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection);

	virtual void Free() override;
};
END

