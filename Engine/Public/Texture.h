#pragma once

#include "Component.h"
#include "CTexLayer.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_TEX2D, TYPE_TEXCUBE, TYPE_END };
private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const wstring& strTextureFilePath);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_Texture(_ulong dwStage, _uint iTextureIndex);
	HRESULT	Change_Container(const wstring& _wstrTypeTag, const wstring& _wstrStateTag);
	const _uint& Container_MaxSize() { return m_iNumTextures; }
	POINT	Get_ImageScale(_uint idx);

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const wstring& strTextureFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:


private:
	//LPDIRECT3DCUBETEXTURE9
	//LPDIRECT3DTEXTURE9
	wstring									m_wstrTypeTag = {};
	wstring									m_wstrStateTag = {};
	_uint									m_iNumTextures = { 0 };

	map<wstring, class CTexLayer*>			m_Textures;
	vector<T_IMAGE>*						m_pTexture = { nullptr };
};

END