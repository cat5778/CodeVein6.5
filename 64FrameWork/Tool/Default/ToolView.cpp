
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "MainFrm.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "TestStage.h"




// ��������
HWND g_hWnd;
HINSTANCE g_hInst;                                // ���� �ν��Ͻ��Դϴ�.


// CToolView
IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CToolView::~CToolView()
{
	Engine::Safe_Release(m_pDevice);
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagement);

	Engine::Release_Resources();
	Engine::Release_Utility();
	Engine::Release_System();

}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

HRESULT CToolView::View_Update(const _float & fTimeDelta)
{
	m_fTime += fTimeDelta;
	Engine::Set_InputDev();
	if (m_pManagement != nullptr)
		m_pManagement->Update_Scene(fTimeDelta);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	return S_OK;
}

void CToolView::View_Render()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (m_pManagement != nullptr)
		m_pManagement->Render_Scene();

	//POINT ptMousePos;

	//GetCursorPos(&ptMousePos);
	///*ScreenToClient();*/
	//swprintf_s(m_szMousePos, L"ScrollPos X=: %d  , Y= %d ", ptMousePos.x, ptMousePos.y);

	//++m_dwRenderCnt;

	//if (m_fTime >= 1.0f)
	//{
	//	wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
	//	m_fTime = 0.f;
	//	m_dwRenderCnt = 0;
	//}

	//Engine::Render_Font(L"Font_Second", m_szFPS, &_vec2(10.f, 40.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	//wsprintf(m_szMousePos, L"Mouse Pos X= %d Y= %d ", ptMousePos.x, ptMousePos.y);
	//Engine::Render_Font(L"Font_Second", m_szMousePos, &_vec2(10.f, 60.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));


	m_pDeviceClass->Render_End();

}

Engine::CGraphicDev * CToolView::GetDeviceClass()
{
	if (m_pDeviceClass == nullptr)
		return  nullptr;
	return m_pDeviceClass;
}

LPDIRECT3DDEVICE9 & CToolView::GetDevice()
{
	return m_pDevice;
}

Engine::CScene * CToolView::GetScene()
{
	return m_pScene;
}

HRESULT CToolView::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED(Engine::Ready_GraphicDev(g_hWnd,
		Engine::MODE_WIN,
		WINCX,
		WINCY,
		&m_pDeviceClass),
		E_FAIL);

	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_Device();
	(*ppGraphicDev)->AddRef();

	Engine::Ready_Font(m_pDeviceClass->Get_Device(), L"Font_Second", L"����", 20, 20, FW_THIN);

	// INPUTDEV
	FAILED(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	return S_OK;
}

HRESULT CToolView::Ready_Scene(LPDIRECT3DDEVICE9 & pGraphicDev, Engine::CManagement ** ppManagement)
{
	FAILED(Engine::Create_Management(pGraphicDev,ppManagement));


	(*ppManagement)->AddRef();

	m_pScene = CTestStage::Create(pGraphicDev, this);
	NULL_CHECK_RETURN(m_pScene, E_FAIL);


	FAILED((*ppManagement)->SetUp_Scene(m_pScene), E_FAIL);

	return S_OK;

}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	g_hWnd = m_hWnd;

	g_hInst = AfxGetInstanceHandle();

	// View ũ�⸦ �����ϱ� ���ؼ��� ������ �������� ũ�⸦ �ǵ帰��.
	// ������ �������� ������ ������ ���.
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);
	RECT rcFrame = {};

	// �������� ũ��(�׵θ� + Ŭ���̾�Ʈ ����) ������ �Լ�.(��ũ�� ��ǥ ����)
	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	RECT rcView = {};

	// �������� Ŭ���̾�Ʈ ����(�׵θ��� �������� ����) ũ�� ������ �Լ�. (Ŭ���̾�Ʈ ��ǥ ����)
	GetClientRect(&rcView);

	// ���� ������������� View ������ ������ ���Ѵ�.
	int iGapX = rcFrame.right - rcView.right;
	int iGapY = rcFrame.bottom - rcView.bottom;

	// �������������� ũ�⸦ ���� ����.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER | SWP_NOMOVE);

	FAILED(SetUp_DefaultSetting(&m_pDevice), E_FAIL);
	FAILED(Ready_Scene(m_pDevice, &m_pManagement), E_FAIL);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	
}


void CToolView::OnMButtonDown(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
}
