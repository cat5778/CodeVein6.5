#include "stdafx.h"
#include "Logo.h"
#include "Stage.h"
#include "MainApp.h"
#include "Image.h"
#include "Export_Function.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CLogo::~CLogo(void)
{

}

HRESULT CLogo::Ready_Scene(void)
{



	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev, RESOURCE_END), E_FAIL);

	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	m_ppPathlist;

	if (true == m_pLoading->Get_Finish())
	{
		*m_ppPathlist = m_pLoading->Get_PathList();

		if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
		{
			Engine::CScene*		pScene = CStage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, E_FAIL);

			FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), E_FAIL);

			return iExit;
		}		
	}



	return iExit;
}

void CLogo::Render_Scene(void)
{
	// DEBUG 용
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_LoadString(CLoading::BASIC),	&_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_LoadString(CLoading::STATIC),	&_vec2(10.f, 30.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_LoadString(CLoading::DYNAMIC), &_vec2(10.f, 50.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_LoadString(CLoading::COLLIDER),&_vec2(10.f, 70.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free(void)
{
	Engine::Safe_Release(m_pLoading);
	Engine::CScene::Free();
}

HRESULT CLogo::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*		pGameObject = nullptr;

	// BackGround
	//pGameObject = CBackGround::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);
	/*
	TitleWallpaper_UI  0
	TitleLogo_BG_Br_UI 
	CloudPattern_B2_UI 0
	CloudPattern_3_UI  0
	CloudPattern_4_UI
	*/

	pGameObject = CImage::Create(m_pGraphicDev, L"TitleWallpaper_UI", _vec3(0.f, 0.f, 0.6f), PIVOT_LT,0.f, _vec3(0.0f, 0.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"00.TitleWallpaper_UI", pGameObject), E_FAIL);

	pGameObject = CImage::Create(m_pGraphicDev,L"CloudPattern_B2_UI",_vec3(0.f,0.f,0.5f),PIVOT_LT, 2.f ,_vec3(0.1f,0.1f,0.f),100.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"01.CloudPattern_B2_UI", pGameObject), E_FAIL);

	pGameObject = CImage::Create(m_pGraphicDev, L"CloudPattern_3_UI", _vec3(160.f, 0.f, 0.4f), PIVOT_LT, 2.9f, _vec3(0.0f, 0.f, 0.f),100.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CImage*>(pGameObject)->Set_MaxAlpha(0.15f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"02.CloudPattern_3_UI", pGameObject), E_FAIL);

	pGameObject = CImage::Create(m_pGraphicDev, L"CloudPattern_4_UI", _vec3(160.f, 0.f, 0.3f), PIVOT_LT, 2.9f, _vec3(0.0f, 0.f, 0.f),100.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CImage*>(pGameObject)->Set_MaxAlpha(0.15f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"03.CloudPattern_4_UI", pGameObject), E_FAIL);

	pGameObject = CImage::Create(m_pGraphicDev, L"TitleLogo_BG_Br_UI", _vec3(0.f, 0.f, 0.2f), PIVOT_LT, 1.5f, _vec3(0.0f, 0.f, 0.f), 100.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CImage*>(pGameObject)->Set_MaxAlpha(0.25f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"04.CloudPattern_3_UI", pGameObject), E_FAIL);

	//pGameObject = CImage::Create(m_pGraphicDev, L"T_TitleLogo_Br_UI", _vec3(0.f, 0.f, 0.003f), PIVOT_LT,2.9f, _vec3(0.0f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	////dynamic_cast<CImage*>(pGameObject)->Set_MaxAlpha(0.25f);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"05.T_TitleLogo_Br_UI", pGameObject), E_FAIL);


	m_pLayerMap.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax)
{
	// 메모리 예약
	Engine::Reserve_ContainerSize(eMax);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_TriCol",
		Engine::BUFFER_TRICOL),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcCol",
		Engine::BUFFER_RCCOL),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcTex",
		Engine::BUFFER_RCTEX),
		E_FAIL);

	//// 텍스쳐
	//FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
	//										RESOURCE_LOGO,
	//										L"Texture_Logo",
	//										Engine::TEX_NORMAL,
	//										L"../../Resource/Texture/LogoBack/LogoBack_%d.png", 38),
	//										E_FAIL); 

	//Title
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"TitleWallpaper_UI",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Title/Convert/T_TitleWallpaper_UI.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"TitleLogo_BG_Br_UI",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Title/Convert/T_TitleLogo_BG_Br_UI.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"T_TitleLogo_Br_UI",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Title/Convert/T_TitleLogo_Br_UI.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"CloudPattern_B2_UI",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Title/Convert/T_CloudPattern_B2_UI.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"CloudPattern_4_UI",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Title/Convert/T_CloudPattern_4_UI.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"CloudPattern_3_UI",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Title/Convert/T_CloudPattern_3_UI.tga"),
		E_FAIL);

	return S_OK;
}
