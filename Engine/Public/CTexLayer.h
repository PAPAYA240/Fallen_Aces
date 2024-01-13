#pragma once

#include "Base.h"

BEGIN(Engine)

typedef struct tagImage
{
	LPDIRECT3DBASETEXTURE9	pTexture;
	POINT					ptImageSize;
}T_IMAGE;

class CTexLayer final : public CBase
{
private:
	CTexLayer();
	virtual ~CTexLayer() = default;

public:
	HRESULT		Initialize();
	HRESULT		Add_Texture(const wstring& _wstrStateTag, LPDIRECT3DBASETEXTURE9 _pTexture, POINT _ptSize);
	void		Add_Ref();

	vector<T_IMAGE>* Find_State(const wstring& _wstrStateTag);

public:
	static CTexLayer*	Create();
	virtual void		Free();

private:
	map<wstring, vector<T_IMAGE>>		m_TexContainer;
};

END