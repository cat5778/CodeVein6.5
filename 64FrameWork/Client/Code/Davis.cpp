#include "stdafx.h"
#include "Davis.h"
#include "Export_Function.h"
#include "ColliderManager.h"

CDavis::CDavis(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, _vec3 vPos, _uint uiCellIdx)
	:CDynamicObject(pGraphicDev, wstrName, uiIdx, 0)
{
	m_bIsMonster = false;
	m_uiCellIdx = uiCellIdx;
	m_vRespawn = vPos;
}

CDavis::~CDavis()
{
}

HRESULT CDavis::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(DEFAULT_MESH_SCALE);
	m_pTransformCom->Set_Pos(m_vRespawn.x, m_vRespawn.y, m_vRespawn.z);
	m_pNaviCom->Set_Index(m_uiCellIdx);
	m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(180.f));
	return S_OK;
}

HRESULT CDavis::LateReady_GameObject()
{
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

	if (m_ppGameObjectMap == nullptr)
		m_ppGameObjectMap = &Engine::Get_Layer(L"GameLogic")->Get_ObjectMap();

	return S_OK;
}

_int CDavis::Update_GameObject(const _float & fTimeDelta)
{
	StateMachine();
	Collision_Check(fTimeDelta);
	m_eCurState = DV_EventIdle;

	_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pMeshCom->Play_Animation(m_fAnimSpeed*fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return S_OK;
}

void CDavis::Render_GameObject(void)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	SetUp_ConstantTable(pEffect);

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

	m_pColliderGroupCom->Render_Collider();



}

HRESULT CDavis::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Davis"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);// 변경

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pColliderGroupCom = Engine::CColliderGroup::Create(m_pGraphicDev, m_wstrInstName, m_pTransformCom, m_pMeshCom);
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_ColliderGroup", pComponent);
	for (int i = 0; i < Engine::COLOPT_END; i++)
		m_pColliderGroupCom->Set_ColliderEnable((Engine::COLLOPTION)i, true);

	CColliderManager::GetInstance()->Get_ObjCollider(m_pColliderGroupCom, m_ObjName);

	pComponent = m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::CNaviMesh::Create(m_pGraphicDev));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);


	return S_OK;
}

void CDavis::Set_TransformData()
{
	if (m_bIsRespawn)
	{
		m_pTransformCom->m_vScale = m_tTransformInfo.vScale;
		m_pTransformCom->m_vAngle = m_tTransformInfo.vRotation;
		m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_tTransformInfo.vPosition;
	}
	else
	{
		m_pTransformCom->Set_Scale(DEFAULT_MESH_SCALE);
		m_pTransformCom->Set_Pos(INIT_VEC3);
	}
}

void CDavis::StateMachine()
{
	if (m_ePreState != m_eCurState)
	{
		m_ePreState = m_eCurState;
		m_fAnimSpeed = 1.0f;

		switch (m_eCurState)
		{
		case DV_NoneEventIdle:
			m_pMeshCom->Set_AnimationSet(49);
			break;
		case DV_EventIdle:
			m_pMeshCom->Set_AnimationSet(46);
			break;
		case DV_Pogitive_Emotion:
			m_pMeshCom->Set_AnimationSet(48);
			break;
		case DV_Nagative_Emotion:
			m_pMeshCom->Set_AnimationSet(47);
			break;
		case DV_End_State:
			break;
		default:
			break;
		}


	}
	

}


void CDavis::Collision_Check(_float fTimeDelta)
{
	Engine::CColliderGroup* pTargetCollCom = nullptr;
	Engine::CTransform* pTargetTransCom = nullptr;
	for (auto &pObject : *m_ppGameObjectMap)
	{
		if (!pObject.second->IsMonster())
			continue;
		if (m_wstrInstName.compare(pObject.first) == 0)
			continue;

		pTargetTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic",
																pObject.second->Get_InstName().c_str(),
																L"Com_Transform",
																Engine::ID_DYNAMIC));
		_vec3 vPos, vTargetPos , vDir;
		vPos = Get_Pos();
		vTargetPos = *pTargetTransCom->Get_Info(Engine::INFO_POS);
		vDir = vPos - vTargetPos;
		_float fLen = D3DXVec3Length(&vDir);
		
		if (fLen >= 5.f)
			continue;

		pTargetCollCom = dynamic_cast<Engine::CColliderGroup*>(Engine::Get_Component(L"GameLogic",
																pObject.second->Get_InstName().c_str(),
																L"Com_ColliderGroup",
																Engine::ID_DYNAMIC));
		if (pTargetCollCom != nullptr)
		{
			_float fPower = 0.f;
			_bool bIsStepColl = false;
			_vec3 vOutPos;

			bIsStepColl = m_pCalculatorCom->Bounding_Sphere(m_pColliderGroupCom->Get_CollVec(Engine::COLOPT_STEP),
				pTargetCollCom->Get_CollVec(Engine::COLOPT_STEP), &fPower);
	
			if (bIsStepColl)
			{
				pTargetTransCom = dynamic_cast<Engine::CTransform*>(pObject.second->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

				vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
				vTargetPos = *pTargetTransCom->Get_Info(Engine::INFO_POS);

				D3DXVec3Normalize(&vDir, &vDir);
				vDir = vDir*fPower;

				m_pNaviCom->Move_OnNaviMesh(&vPos, &vDir, &vOutPos);

				m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);	//적용
			}

		}


	}
}

CDavis * CDavis::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, _vec3 vPos, _uint uiCellIdx)
{
	CDavis*	pInstance = new CDavis(pGraphicDev, wstrName, uiIdx, vPos, uiCellIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
void CDavis::Free(void)
{
	Engine::CGameObject::Free();
}
