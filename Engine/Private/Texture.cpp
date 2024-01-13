#include "..\Public\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_Textures(rhs.m_Textures)
	, m_pTexture(rhs.m_pTexture)
{
	for (auto& pTexLayer : m_Textures)
	{
		Safe_AddRef(pTexLayer.second);

		pTexLayer.second->Add_Ref();
	}
}

HRESULT CTexture::Initialize_Prototype(TYPE eType, const wstring & strTextureFilePath)
{
	/* 파일의 경로를 담아 둠 */
	filesystem::path folderPath = strTextureFilePath.c_str();

	/* 파일 경로가 실제 폴더로 되어있는지, 파일 경로가 실제로 존재 하는 지 */
 	if (filesystem::exists(folderPath) && filesystem::is_directory(folderPath))
	{
		/* 파일 경로부터 하위 폴더를 모두 탐색함 */
		for (const auto& file : filesystem::recursive_directory_iterator(folderPath))
		{
			/* 이미지 파일을 발견한 경우 */
			if (file.path().extension() == TEXT(".png") || file.path().extension() == TEXT(".dds") || 
  				file.path().extension() == TEXT(".bmp") || file.path().extension() == TEXT(".tga") || file.path().extension() == TEXT(".jpg"))
			{
				wstring wstrStateTag = file.path().parent_path().filename().wstring();
				wstring wstrTypeTag = file.path().parent_path().parent_path().filename().wstring();

				auto TypeIter = m_Textures.find(wstrTypeTag);

				HRESULT hr = {};

				if (TypeIter == m_Textures.end())
				{
					CTexLayer* pTexLayer = CTexLayer::Create();

					if (TYPE_TEX2D == eType)
					{
						LPDIRECT3DTEXTURE9 pTexture = { nullptr };
						
						hr = D3DXCreateTextureFromFile(m_pGraphic_Device, file.path().c_str(), &pTexture);

						POINT ptSize = {};

						D3DXIMAGE_INFO ImageInfo = {};

						if (FAILED(D3DXGetImageInfoFromFile(file.path().c_str(), &ImageInfo)))
						{
							MSG_BOX(TEXT("Failed to GetInfoFromFile"));
							Safe_Release<LPDIRECT3DTEXTURE9>(pTexture);
							return E_FAIL;
						}

						ptSize.x = ImageInfo.Width;
						ptSize.y = ImageInfo.Height;

						pTexLayer->Add_Texture(wstrStateTag, pTexture, ptSize);
					}
					else if (TYPE_TEXCUBE == eType)
					{
						LPDIRECT3DCUBETEXTURE9 pTexture = { nullptr };

						hr = D3DXCreateCubeTextureFromFile(m_pGraphic_Device, file.path().c_str(), &pTexture);

						POINT ptSize = {};

						D3DXIMAGE_INFO ImageInfo = {};

						if (FAILED(D3DXGetImageInfoFromFile(file.path().c_str(), &ImageInfo)))
						{
							MSG_BOX(TEXT("Failed to GetInfoFromFile"));
							Safe_Release<LPDIRECT3DCUBETEXTURE9>(pTexture);
							return E_FAIL;
						}

						ptSize.x = ImageInfo.Width;
						ptSize.y = ImageInfo.Height;

						pTexLayer->Add_Texture(wstrStateTag, pTexture, ptSize);
					}

					m_Textures.emplace(wstrTypeTag, pTexLayer);

					/* 이미지를 처음 삽입 했을 때, 주소를 보관 중이지 않으므로 주소를 첫 주소로 연결 시켜 줌 */
					if (nullptr == m_pTexture)
					{
						m_pTexture = pTexLayer->Find_State(wstrStateTag);

						if (nullptr != m_pTexture)
						{
							m_wstrTypeTag = wstrTypeTag;
							m_wstrStateTag = wstrStateTag;
						}
					}
				}
				else
				{
					auto StateIter = TypeIter->second->Find_State(wstrStateTag);

					/* 종류 태그는 존재하고, 상태 태그가 없는 경우 */
					/* 태그는 있지만, 객체 주소가 없는 경우, 벡터만 추가함 (해당 태그의 공간을 만듦)*/
					if (nullptr == StateIter)
					{
						if (FAILED(TypeIter->second->Add_Texture(wstrStateTag, nullptr, POINT())))
							return E_FAIL;

						StateIter = TypeIter->second->Find_State(wstrStateTag);
					}

					if (TYPE_TEX2D == eType)
					{
						LPDIRECT3DTEXTURE9 pTexture = { nullptr };

						hr = D3DXCreateTextureFromFile(m_pGraphic_Device, file.path().c_str(), &pTexture);

						T_IMAGE tImage = {};
						D3DXIMAGE_INFO dxInfo = {};

						if (FAILED(D3DXGetImageInfoFromFile(file.path().c_str(), &dxInfo)))
						{
							MSG_BOX(TEXT("Failed to GetInfoFromFile"));
							Safe_Release<LPDIRECT3DTEXTURE9>(pTexture);
							return E_FAIL;
						}

						tImage.pTexture = pTexture;
						tImage.ptImageSize.x = dxInfo.Width;
						tImage.ptImageSize.y = dxInfo.Height;

						StateIter->push_back(tImage);
					}
					else if (TYPE_TEXCUBE == eType)
					{
						LPDIRECT3DCUBETEXTURE9 pTexture = { nullptr };

						hr = D3DXCreateCubeTextureFromFile(m_pGraphic_Device, file.path().c_str(), &pTexture);

						T_IMAGE tImage = {};
						D3DXIMAGE_INFO dxInfo = {};

						if (FAILED(D3DXGetImageInfoFromFile(file.path().c_str(), &dxInfo)))
						{
							MSG_BOX(TEXT("Failed to GetInfoFromFile"));
							Safe_Release<LPDIRECT3DCUBETEXTURE9>(pTexture);
							return E_FAIL;
						}

						tImage.pTexture = pTexture;
						tImage.ptImageSize.x = dxInfo.Width;
						tImage.ptImageSize.y = dxInfo.Height;

						StateIter->push_back(tImage);
					}
				}

				if (FAILED(hr))
					return E_FAIL;
			}
		}
	}

	if (nullptr != m_pTexture)
		m_iNumTextures = (_uint)m_pTexture->size();

	return S_OK;
}

HRESULT CTexture::Initialize(void * pArg)
{

	return S_OK;
}

HRESULT CTexture::Bind_Texture(_ulong dwStage, _uint iTextureIndex)
{
	if(nullptr == m_pTexture)
		return E_FAIL;

	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return m_pGraphic_Device->SetTexture(dwStage, (*m_pTexture)[iTextureIndex].pTexture); 
#pragma region 공부 주석임

	// SetTexture ( 슬롯, 텍스쳐 )
	/*
	- 슬롯은 텍스쳐를 각각 텍스쳐를 보관해 놓는 보관소 같은 느낌이다.
	- 여러 슬롯을 사용하여 0번 슬롯과 1번 슬롯의 텍스쳐를 합성 한 텍스쳐를 얻을 수 있다.
	- 동작 원리는 어떤 것을 위에 그릴 것인지 순서를 정하여 예를 들면 0번 슬롯은 벽돌, 1번 슬롯은 이끼라고 가정했을 때
		0번 슬롯을 먼저 그리고 그 위에 1번 슬롯을 그리게하여 이끼낀 벽돌을 만들 수 있다.
	*/
#pragma endregion
}

HRESULT CTexture::Change_Container(const wstring& _wstrTypeTag, const wstring& _wstrStateTag)
{
	/* 현재 적용 중인 컨테이너의 주소가 동일한 태그라면 탐색연산을 줄이기 위한 예외처리 */
	if (_wstrTypeTag == m_wstrTypeTag && _wstrStateTag == m_wstrStateTag)
		return E_FAIL;

	auto iter = m_Textures.find(_wstrTypeTag);

	/* 종류 태그가 존재하지 않을 경우 예외 */
	if (iter == m_Textures.end())
		return E_FAIL;

	/* 공간 자체는 있는데 주소가 비어있는 경우 예외 */
	if (nullptr == iter->second)
		return E_FAIL;

	/* 객체가 존재하는 경우 해당 객체에게 내부 컨테이너를 꺼내 옴 */
	CTexLayer* pTexLayer = iter->second;

	auto pTexContainer = pTexLayer->Find_State(_wstrStateTag);

	/* 컨테이너의 주소가 비어있는 경우 예외처리 */
	if (nullptr == pTexContainer)
		return E_FAIL;

	/* 명확하게 주소가 존재한다면 주소 바꿔치기 및 해당 컨테이너의 최대 텍스쳐 개수 갱신 */
	m_pTexture = pTexContainer;
	m_iNumTextures = (_uint)m_pTexture->size();
	m_wstrTypeTag = _wstrTypeTag;
	m_wstrTypeTag = _wstrStateTag;

	return S_OK;
}

POINT CTexture::Get_ImageScale(_uint idx)
{
	return (*m_pTexture)[idx].ptImageSize;
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const wstring & strTextureFilePath)
{
	CTexture*		pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eType, strTextureFilePath)))
	{
 		MSG_BOX(TEXT("Failed To Created : CTexture"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

/* 사본객체를 생성하기위한 함수에요. */
CComponent * CTexture::Clone(void* pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTexture"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto pair : m_Textures)
		Safe_Release(pair.second);

	m_Textures.clear();
}