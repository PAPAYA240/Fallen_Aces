#pragma once

#include "Base.h"

BEGIN(Engine)

class CTexLayer final : public CBase
{
private:
	CTexLayer();
	virtual ~CTexLayer() = default;

public:
	HRESULT		Initialize();
	HRESULT		Add_Texture(const wstring& _wstrStateTag, LPDIRECT3DBASETEXTURE9 _pTexture);
	void		Add_Ref();

	vector<LPDIRECT3DBASETEXTURE9>* Find_State(const wstring& _wstrStateTag);

public:
	static CTexLayer*	Create();
	virtual void		Free();

private:
	map<wstring, vector<LPDIRECT3DBASETEXTURE9>>		m_TexContainer;
};

END