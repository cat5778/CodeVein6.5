#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "Export_Function.h"
#include "Player.h"
#include "LockOn.h"
#include "DynamicObject.h"
CThirdPersonCamera::CThirdPersonCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{

}

CThirdPersonCamera::~CThirdPersonCamera(void)
{

}

void CThirdPersonCamera::SetUp_Target(Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

HRESULT CThirdPersonCamera::Ready_GameObject(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Engine::CCamera::Ready_GameObject(), E_FAIL);
	
	return S_OK;
}

HRESULT CThirdPersonCamera::LateReady_GameObject(void)
{
	if (m_ppGameObjectMap == nullptr)
		m_ppGameObjectMap = &Engine::Get_Layer(L"GameLogic")->Get_ObjectMap();
	m_pLockOn = dynamic_cast<CLockOn*>(Engine::Get_GameObject(L"UI", L"LockOnUI"));

	return S_OK;
}

_int CThirdPersonCamera::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pTargetInfo == nullptr)
		m_pTargetInfo = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

	if (CKeyMgr::GetInstance()->KeyDown(KEY_O))
		m_bIsFix ? m_bIsFix = false: m_bIsFix=true;

	Mouse_Fix();
	Target_Renewal(fTimeDelta);
	Mouse_Move(fTimeDelta);
	Key_Input(fTimeDelta);
	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

	return iExit;
}

HRESULT CThirdPersonCamera::Ready_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	m_fVerticalAngle = 170.f;
	return S_OK;
}
//_vec3 Get_CamPos();
//{
//	return m_
//}
void CThirdPersonCamera::Key_Input(const _float& fTimeDelta)
{

}

void CThirdPersonCamera::Target_Renewal(const _float& fTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*	pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Mesh", Engine::ID_STATIC));
		if (pPlayerMeshCom == nullptr)
			return;

		const Engine::D3DXFRAME_DERIVED* pBone = pPlayerMeshCom->Get_FrameByName("Head");
		if (pBone == nullptr)
			return;

		m_pParentBoneMatrix = &pBone->CombinedTransformationMatrix;

		Engine::CTransform*	pPlayerTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
		dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"))->Set_CamTransform(&m_pTransformCom);

		m_pParentWorldMatrix = m_pTargetInfo->Get_WorldMatrixPointer();

		m_matWorld = (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
		memcpy(&m_vHeadPos, &m_matWorld.m[3][0], sizeof(_vec3));
		m_vAt = m_vHeadPos;


		m_vEye = m_pTransformCom->m_vInfo[Engine::INFO_POS] = -(m_vHeadPos + m_pTransformCom->m_vInfo[Engine::INFO_LOOK]) * m_fDistance;
		

	}

	m_matWorld = (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
	memcpy(&m_vHeadPos, &m_matWorld.m[3][0], sizeof(_vec3));
	if (!m_bIsLockOn)
	{
		m_pMonTransform = nullptr;
		m_vAt = m_vHeadPos;
		m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_vHeadPos + m_pTransformCom->m_vInfo[Engine::INFO_LOOK] * m_fDistance;
		_vec3		vRight;
		memcpy(&vRight, &m_pTransformCom->m_matWorld.m[0][0], sizeof(_vec3));

		m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(m_fVerticalAngle));
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(m_fHorizonAngle));
		if (m_pLockOn != nullptr)
			m_pLockOn->Set_TargetPos(nullptr);


	}
	else
	{
		if (m_pMonTransform == nullptr)
		{

			wstring wstrInstName;
			m_LockOnDistance = std::numeric_limits<float>::infinity();

			for (auto &pObject : *m_ppGameObjectMap)
			{
				if (!pObject.second->IsMonster())
					continue;
				else
					if (dynamic_cast<CDynamicObject*>(pObject.second)->CheckDeath())
						continue;
	
				_vec3 vMonPos =*pObject.second->Get_Transform()->Get_Info(Engine::INFO_POS);
				_float fDistance;
				fDistance = D3DXVec3Length(&(*m_pTransformCom->Get_Info(Engine::INFO_POS) - vMonPos));
				
				if (m_LockOnDistance > fDistance)
				{
					m_LockOnDistance = fDistance;
					wstrInstName = pObject.second->Get_InstName();
				}
				//_vec3 vDir ;
				//_vec3 vCamLook = m_pTransformCom->m_vInfo[Engine::INFO_LOOK];
				//vCamLook.y = 0;
				//D3DXVec3Normalize(&vCamLook, &vCamLook);
				//ExtractY_NormalDir(vMonPos, m_vAt, &vDir);
				//Get_Angle(vCamLook, m_vAt);

			
			
			
			}
			m_pMonTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", wstrInstName.c_str(), L"Com_Transform", Engine::ID_DYNAMIC));
			if (m_pLockOn != nullptr)
				m_pLockOn->Set_TargetPos(m_pMonTransform);


		}
		_vec3	vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
		D3DXVec3Normalize(&vLook, &vLook);

		if (m_pMonTransform != nullptr)
		{
			m_vAt = *m_pMonTransform->Get_Info(Engine::INFO_POS);   // TargetPos 
			_vec3 vTempDir = m_pTransformCom->m_vInfo[Engine::INFO_POS] - m_vAt;

			//cout << Get_Angle(vLook, -vTempDir)<< endl;
			//m_pTransformCom-

			D3DXVec3Normalize(&vTempDir, &vTempDir);
			m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_vHeadPos + vTempDir * m_fDistance;
		}
		else
			m_vAt = m_vHeadPos;

		//_vec3 vPos =m_pTransformCom->m_vInfo[Engine::INFO_POS];

	}


	m_vEye = m_pTransformCom->m_vInfo[Engine::INFO_POS];

	//m_pTransformCom->m_vInfo[Engine::INFO_LOOK] = m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_vHeadPos + m_pTransformCom->m_vInfo[Engine::INFO_LOOK] * m_fDistance;
	


}

void CThirdPersonCamera::Mouse_Move(const _float & fTimeDelta)
{
	_long	dwMouseMove = 0;
	m_fVerticalAngle = 0;
	m_fHorizonAngle = 0;
	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	{
		m_fVerticalAngle += (dwMouseMove*10.f*fTimeDelta);

	}

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
	{
		m_fHorizonAngle += D3DXToRadian(dwMouseMove*300.f)*fTimeDelta;
	}
	if (Engine::Get_DIMouseMove(Engine::DIMS_Z) > 0)
	{
		if (m_fDistance >= 2.0f)
			m_fDistance -= 0.5f;
	}
	if (Engine::Get_DIMouseMove(Engine::DIMS_Z) < 0)
	{
		if (m_fDistance <= 4.5f)
			m_fDistance += 0.5f;
	}
	//Lerp_Cam(fTimeDelta);

}

void CThirdPersonCamera::Lerp_Cam(_float fTimeDelta)
{
	//if (D3DXToDegree(fabsf(m_fHorizonAngle - m_fOldHorizonAngle)) >= 1.0f)
	//{
	//	if (m_fTime[0] <= 1.0f)
	//		m_fTime[0] += fTimeDelta;

	//	m_fLerpHorizonAngle = Lerp(m_fOldHorizonAngle, m_fHorizonAngle, m_fTime[0]);
	//	if (m_fTime[0] >= 1.0f)
	//	{
	//		m_fTime[0] = 0.f;
	//		m_fOldHorizonAngle = m_fHorizonAngle;
	//	}
	//}
	//if (D3DXToDegree(fabsf(m_fVerticalAngle - m_fOldVerticalAngle)) >= 1.0f)
	//{
	//	if (m_fTime[1] <= 1.0f)
	//		m_fTime[1] += fTimeDelta;
	//	m_fLerpVerticalAngle = (Lerp<_float>(m_fOldVerticalAngle, m_fVerticalAngle, m_fTime[1]));
	//	int a = 3;
	//	if (m_fTime[1] >= 1.0f)
	//	{
	//		m_fTime[1] = 0.f;
	//		m_fOldVerticalAngle = m_fVerticalAngle;
	//	}
	//}

}

void CThirdPersonCamera::Mouse_Fix(void)
{
	if (m_bIsFix)
	{
		POINT		ptMouse{ WINCX >> 1, WINCY >> 1 };

		ClientToScreen(g_hWnd, &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);
	}
}

float CThirdPersonCamera::Get_Angle(const D3DXVECTOR3 & a, const D3DXVECTOR3 & b)
{
	float fRadian = acosf(D3DXVec3Dot(&a, &b) / (D3DXVec3Length(&a) * D3DXVec3Length(&b)));

	fRadian = D3DXToDegree(fRadian);
	float fDgree = (a.x * b.z - a.z * b.x > 0.0f) ? fRadian : -fRadian;

	return fDgree;
}

void CThirdPersonCamera::LockOn(_bool bIsLockOn)
{
	m_bIsLockOn = bIsLockOn;

}


CThirdPersonCamera* CThirdPersonCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
														const _vec3* pEye,
														const _vec3* pAt,
														const _vec3* pUp,
														const _float& fFovY,
														const _float& fAspect,
														const _float& fNear,
														const _float& fFar)
{
	CThirdPersonCamera*	pInstance = new CThirdPersonCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Engine::Safe_Release(pInstance);


	return pInstance;
}

void CThirdPersonCamera::Free(void)
{
	Engine::CCamera::Free();
}

