#include "stdafx.h"
#include "Loading.h"
#include <fstream>
#include "Export_Function.h"
#include "DirectoryMgr.h"
#include <iostream>
#include "ColliderManager.h"
CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading(void)
{

}

_bool CLoading::Get_Finish(void)
{
	_uint uiCheckLoad=0;
	for (int i = 0; i < OPTION_END; i++)
	{
		if (m_uiLoadingCheck[(THREAD_OPTION)i] == 1)
			uiCheckLoad++;
	}
	if (uiCheckLoad == OPTION_END)
		return true;
	return false;
}

_uint	CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;
	_uint iFlag = 0;

	//EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}
	
	//LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

_uint CLoading::Thread_Dynamic(void * pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;
	_uint iFlag = 1;

	iFlag = pLoading->Loading_Dynamic();

	return iFlag;
}

_uint CLoading::Thread_Basic(void * pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;
	_uint iFlag = 2;
	
	iFlag = pLoading->Loading_Basic();

	_endthreadex(2);

	return iFlag;
}

_uint CLoading::Thread_Collider(void * pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;
	_uint iFlag = 3;

	iFlag = pLoading->Loading_Collider();

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread[BASIC] = (HANDLE)_beginthreadex(NULL, 0, Thread_Basic, this, 0, NULL);
	m_hThread[STATIC] = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);
	m_hThread[DYNAMIC] = (HANDLE)_beginthreadex(NULL, 0, Thread_Dynamic, this, 0, NULL);
	m_hThread[COLLIDER] = (HANDLE)_beginthreadex(NULL, 0, Thread_Collider, this, 0, NULL);
	m_eLoading = eLoading;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{

	lstrcpy(m_szLoading[STATIC], L"Buffer Loading.............................");

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
												RESOURCE_STATIC,
												L"Buffer_CubeTex",
												Engine::BUFFER_CUBETEX),
												E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
													RESOURCE_STAGE,
													L"Mesh_Navi",
													Engine::TYPE_NAVI,
													NULL,
													NULL),
													E_FAIL);
	
	lstrcpy(m_szLoading[STATIC], L"Mesh Loading.............................");

	switch ((LOADMODE)LOAD_MODE)
	{
	case LOAD_NOMAL:
		Loading_Mesh();

		break;
	case LOAD_NOMAL2:
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Map3",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Snow/Map3/Map3/",
			L"Map3.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"SM_NormalGreatSwordA_ba01",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Base/Weapon/SM_NormalGreatSwordA_ba01/",
			L"SM_NormalGreatSwordA_ba01.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHatShield",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Public/Weapon/RussianHatShield/",
			L"RussianHatShield.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"SK_NormalHalberdB",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Public/Weapon/SK_NormalHalberdB/",
			L"SK_NormalHalberdB.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"SM_Sky_ba01",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Snow/SM_Sky_ba01/SM_Sky_ba01/",
			L"SM_Sky_ba01.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"SM_SkySphere_st09a1",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Snow/SM_SkySphere_st09a1/SM_SkySphere_st09a1/",
			L"SM_SkySphere_st09a1.X"),
			E_FAIL);
		break;
	case LOAD_NOMAL3:
		Loading_Mesh();
		break;
	case LOAD_PLAYER:
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"SM_NormalGreatSwordA_ba01",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Base/Weapon/SM_NormalGreatSwordA_ba01/",
			L"SM_NormalGreatSwordA_ba01.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHat",
			Engine::TYPE_DYNAMIC,
			L"../../Resource/Mesh/DynamicMesh/RussianHat/",
			L"RussianHat.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHatShield",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Public/Weapon/RussianHatShield/",
			L"RussianHatShield.X"),
			E_FAIL);
		break;
	case LOAD_BATTLE:
	{
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianKetsugi",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Public/Weapon/RussianKetsugi/",
			L"RussianKetsugi.X"),
			E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianSword",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Public/Weapon/RussianSword/",
			L"RussianSword.X"),
			E_FAIL);



		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"SM_NormalGreatSwordA_ba01",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Base/Weapon/SM_NormalGreatSwordA_ba01/",
			L"SM_NormalGreatSwordA_ba01.X"),
			E_FAIL);



		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHatShield",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Public/Weapon/RussianHatShield/",
			L"RussianHatShield.X"),
			E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"SK_NormalHalberdB",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Public/Weapon/SK_NormalHalberdB/",
			L"SK_NormalHalberdB.X"),
			E_FAIL);

	}
		break;
	case LOAD_SLIDE:
		break;
	case LOAD_END:
		break;
	default:
		break;
	}

	//Loading_Collider();

	lstrcpy(m_szLoading[STATIC], L"Loading Complete!!!");
	m_uiLoadingCheck[STATIC] = 1;


	return 0;
}

_bool CLoading::Ready_Mesh(MESH_PATH * pPathInfo)
{
	if (Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_STAGE, pPathInfo->wstrObjectType.c_str(), Engine::TYPE_STATIC, pPathInfo->wstrRelative.c_str(), pPathInfo->wstrName.c_str()) < 0)
		return false;


	return 	true;
}

_bool CLoading::Loading_Basic()
{

	lstrcpy(m_szLoading[STATIC], L"Texture Loading.............................");

	// �ؽ���

	//FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Texture_Cube",
	//	Engine::TEX_CUBE,
	//	L"../../Resource/Texture/SkyBox/burger%d.dds",
	//	4),
	//	E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
											RESOURCE_STATIC,
											L"Buffer_InRcTex",
											Engine::BUFFER_INRCTEX),
											E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
											RESOURCE_STAGE,
											L"Texture_HPGauge",
											Engine::TEX_NORMAL,
											L"../../Resource/Texture/Gauge/T_BossHpGaugeTexture_UI.tga"),
											E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
											RESOURCE_STAGE,
											L"Texture_HPGaugeBar",
											Engine::TEX_NORMAL,
											L"../../Resource/Texture/Gauge/T_BossHpGaugeBarTexture_UI.tga"),
											E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
											RESOURCE_STAGE,
											L"Texture_StaminaGauge",
											Engine::TEX_NORMAL,
											L"../../Resource/Texture/Gauge/StaminaGauge.tga"),
											E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"LockOnSite",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/LockOn/LockOnSite.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_StaminaGaugeBar",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Gauge/StaminaGaugeBar.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"BossHP",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Gauge/BossHP.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"BossHPBar",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Gauge/BossHPBar.tga"),
		E_FAIL);


	//UI
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"EmptyFrame",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UIFrame/EmptyFrame.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"EmptyFrame2",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UIFrame/EmptyFrame2.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"ShopUI2",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UIFrame/ShopUI2.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"ShopUI3",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UIFrame/ShopUI3.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"InvenFrame",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UIFrame/InvenFrame.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"PortalUI",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UIFrame/Portal.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"ShopUI",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UIFrame/ShopUI.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Select",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UIFrame/Select.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Select2",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/WeaponIcon/T_Radar_WidgetFrame.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"LargeSword_H_Icon",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/WeaponIcon/T_WeaponIcon_LargeSword_H.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"LargeSword_I_Icon",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/WeaponIcon/T_WeaponIcon_LargeSword_I.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Halberd_B_Icon",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/WeaponIcon/T_WeaponIcon_WeaponHalberd_B.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"PortalEmpty",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/WayPointImage/PortalEmpty.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Waypoint",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/WayPointImage/Waypoint.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Number",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Number/%d.tga",11),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Fire",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Texture/T_FX_TC5Fire01.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"FireMask",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Texture/TC_Alpha/T_FX_GEZInk01.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Fire2",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/T_FX_ExternalFlamesBurst/T_FX_ExternalFlamesBurst%d.png",32),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"FireMask2",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/T_FX_ExternalFlamesBurst_Alpha/T_FX_ExternalFlamesBurst_Alpha%d.png", 32),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"FireAlpha",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Texture/TC_Alpha/T_FX_TC5CloudFull09.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"T_FX_ExternalRGBNoise01",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Texture/T_FX_ExternalRGBNoise01.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"T_FX_POLY",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Texture/T_FX_POLY.tga"),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"SM_Sky_ba01",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Snow/SM_Sky_ba01/SM_Sky_ba01/",
		L"SM_Sky_ba01.X"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"SM_SkySphere_st09a1",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Snow/SM_SkySphere_st09a1/SM_SkySphere_st09a1/",
		L"SM_SkySphere_st09a1.X"),
		E_FAIL);

	//Loading_Mesh2()


	lstrcpy(m_szLoading[BASIC], L"Texture Complete > ,< ");
	m_uiLoadingCheck[BASIC] = 1;

	return true;
}

_bool CLoading::Loading_Dynamic()
{
	lstrcpy(m_szLoading[DYNAMIC], L"Character Loading.............................");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Player",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Player2/",
		L"Player2.X"),
		E_FAIL);
	if (LOAD_MODE == LOAD_SLIDE)
	{
		m_uiLoadingCheck[DYNAMIC] = 1;
		lstrcpy(m_szLoading[DYNAMIC], L"Character Loading Complete > ,< ");

		return true;
	}

	lstrcpy(m_szLoading[DYNAMIC], L"Monster Loading .............................");
	if (LOAD_MODE== 5)
	{
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"CocoonDevil",
			Engine::TYPE_DYNAMIC,
			L"../../Resource/Mesh/DynamicMesh/CocoonDevil/",
			L"CocoonDevil.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHat",
			Engine::TYPE_DYNAMIC,
			L"../../Resource/Mesh/DynamicMesh/RussianHat/",
			L"RussianHat.X"),
			E_FAIL);
				
	}
	else
	{
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHat",
			Engine::TYPE_DYNAMIC,
			L"../../Resource/Mesh/DynamicMesh/RussianHat/",
			L"RussianHat.X"),
			E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RedDevil",
			Engine::TYPE_DYNAMIC,
			L"../../Resource/Mesh/DynamicMesh/RedDevil/",
			L"RedDevil.X"),
			E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"CocoonDevil",
			Engine::TYPE_DYNAMIC,
			L"../../Resource/Mesh/DynamicMesh/CocoonDevil/",
			L"CocoonDevil.X"),
			E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Davis",
			Engine::TYPE_DYNAMIC,
			L"../../Resource/Mesh/DynamicMesh/Davis/",
			L"Davis.X"),
			E_FAIL);

	}
	lstrcpy(m_szLoading[DYNAMIC], L"Character Loading Complete > ,< ");
	m_uiLoadingCheck[DYNAMIC] = 1;

	return true;
}

_bool CLoading::Loading_Mesh() //�ؽ�Ʈ �а�ͼ� �޽� �ε�
{
	TCHAR szFileName[MAX_STR] = L"../../Resource/Data/PathInfo.txt";

	ifstream fin;

	fin.open(szFileName,ios::in);

	if (fin.fail())
		return E_FAIL;
	
	char cTemp[MIN_STR];
	while (!fin.eof())
	{
		
		MESH_PATH* pPathInfo = new MESH_PATH;
		
		fin.getline(cTemp, MIN_STR);
		wchar_t* ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrGroup = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrMap = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrMeshType= ppwchar;
		delete ppwchar;
		
		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrName= ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrObjectType= ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrRelative= ppwchar;
		delete ppwchar;

		if (pPathInfo->wstrName.empty())
		{
			delete pPathInfo;
			pPathInfo = nullptr;
			continue;
		}
		else
			m_pPathList.push_back(pPathInfo);
	}

	for (auto pPathInfo : m_pPathList)
	{
		if (pPathInfo->wstrMeshType.compare(L"StaticMesh") == 0)
		{
			//if (pPathInfo->wstrObjectType.compare(L"SM_Ceiling_a_ba01"))
			//{

			//}
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_STAGE, pPathInfo->wstrObjectType.c_str(), Engine::TYPE_STATIC, pPathInfo->wstrRelative.c_str(), pPathInfo->wstrName.c_str()) < 0);
		}
		else
			continue;
	}

	


	return false;
}

_bool CLoading::Loading_Mesh2()
{
	TCHAR szFileName[MAX_STR] = L"../../Resource/Data/PathInfo4.txt";

	ifstream fin;

	fin.open(szFileName, ios::in);

	if (fin.fail())
		return E_FAIL;

	char cTemp[MIN_STR];
	while (!fin.eof())
	{

		MESH_PATH* pPathInfo = new MESH_PATH;

		fin.getline(cTemp, MIN_STR);
		wchar_t* ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrGroup = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrMap = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrMeshType = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrName = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrObjectType = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrRelative = ppwchar;
		delete ppwchar;

		if (pPathInfo->wstrName.empty())
		{
			delete pPathInfo;
			pPathInfo = nullptr;
			continue;
		}
		else
			m_pPathList.push_back(pPathInfo);
	}

	for (auto pPathInfo : m_pPathList)
	{
		if (pPathInfo->wstrMeshType.compare(L"StaticMesh") == 0)
		{
			if (pPathInfo->wstrObjectType.compare(L"SM_Ceiling_a_ba01"))
			{
				int a = 3;
			}
			if (Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_STAGE, pPathInfo->wstrObjectType.c_str(), Engine::TYPE_STATIC, pPathInfo->wstrRelative.c_str(), pPathInfo->wstrName.c_str()) < 0)
			{
				wstring wstr = L"Loading.cpp ReadyMesh Failed!!!  Object =" + pPathInfo->wstrObjectType;
				DE_COUT(wstr.c_str())
					continue;
			}

		}
		else
			continue;
	}




	return false;

}

_bool CLoading::Loading_Collider()
{
	lstrcpy(m_szLoading[COLLIDER], L"Collider Loading.............................");

	TCHAR szFileName[MAX_STR] = L"../../Resource/Data/Collider/Test2.txt";

	ifstream fin;

	fin.open(szFileName, ios::in);

	if (fin.fail())
		return E_FAIL;

	char cTemp[MIN_STR];
	while (!fin.eof())
	{
		SPHERE_COL_DATA* pColData = new SPHERE_COL_DATA;


		fin.getline(cTemp, MIN_STR);
		wchar_t* ppwchar = CharToWChar(cTemp);
		pColData->wstrObjTag = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		pColData->uiOption = (_uint)atoi(cTemp);

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pColData->wstrBoneTag = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pColData->wstrCollTag = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		pColData->vPos.x = (float)atof(cTemp);

		fin.getline(cTemp, MIN_STR);
		pColData->vPos.y = (float)atof(cTemp);

		fin.getline(cTemp, MIN_STR);
		pColData->vPos.z = (float)atof(cTemp);

		fin.getline(cTemp, MIN_STR);
		pColData->fRadius = (float)atof(cTemp);

		if (pColData->wstrObjTag.empty())
		{
			delete pColData;
			pColData = nullptr;
			continue;
		}
		else
			CColliderManager::GetInstance()->Add_Colldata(pColData);
	}

	lstrcpy(m_szLoading[COLLIDER], L"Collider Complete.............................");
	m_uiLoadingCheck[COLLIDER] = 1;

}

//_bool CLoading::Loading_Collider()
//{
	//TCHAR szFileName[MAX_STR] = L"../../Resource/Data/Collider.txt";

	//ifstream fin;

	//fin.open(szFileName);

	//if (fin.fail())
	//	return E_FAIL;

	//wstring wstrTemp;

	//char cTemp[MIN_STR];
	//_vec3 vPos = { INIT_VEC3 };
	//_float fRadius = 0.f;
	//while (!fin.eof())
	//{
	//	D3DXVECTOR3 vPos;

	//	fin.getline(cTemp, MIN_STR);
	//	wchar_t* ppwchar = CharToWChar(cTemp);
	//	wstrTemp = ppwchar;
	//	delete ppwchar;

	//	if (wstrTemp.compare(L"") == 0)
	//		break;

	//	fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
	//	vPos.x = atof(cTemp);
	//	fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
	//	vPos.y = atof(cTemp);
	//	fin.getline(cTemp, MIN_STR);
	//	vPos.z = atof(cTemp);

	//	fin.getline(cTemp, MIN_STR);
	//	fRadius = atof(cTemp);

	//	COLL_DATA* pCollData = new COLL_DATA;

	//	wstring wstObject, wstrBone;
	//	_uint uiIdx = 0;

	//	DividString(wstrTemp, wstObject, wstrBone, uiIdx);
	//	//wstObject += L"_0";


	//	//Engine::CGameObject* pGameObject = nullptr;
	//	//pGameObject = m_pSphereColl = CSphereCollider::Create(m_pDevice, wstObject, wstrBone);
	//	//m_pSphereColl->Set_Radius(fRadius);
	//	//m_pSphereColl->Set_Position(vPos);

	//	//(*m_ppGameObjectMap).insert(make_pair(wstrTemp, pGameObject));
	//}
	//fin.close();

//}

wchar_t * CLoading::CharToWChar(const char * pstrSrc)
{
	int nLen = strlen(pstrSrc) + 1;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	mbstowcs(pwstr, pstrSrc, nLen);
	return pwstr;

}

void CLoading::DividString(wstring wstrOrigin, wstring & wstrObject, wstring & wstrBone, _uint & uiIdx)
{
	_uint uiObjLine = 0;
	uiObjLine = wstrOrigin.find(L"_");

	_uint uiBoneLine = wstrOrigin.find(L"_", uiObjLine + 1);
	wstrObject = wstrOrigin.substr(0, uiObjLine);

	wstrBone = wstrOrigin.substr(uiObjLine + 1, uiBoneLine - uiObjLine - 1);

}


CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoading)))
		Safe_Release(pInstance);

	return pInstance;

}

void CLoading::Free(void)
{
	for (int i = 0; i < OPTION_END; i++)
	{
		WaitForSingleObject(m_hThread[(THREAD_OPTION)i], INFINITE);
		CloseHandle(m_hThread[(THREAD_OPTION)i]);
	}

	//DeleteCriticalSection(&m_Crt);

	Engine::Safe_Release(m_pGraphicDev);
}

