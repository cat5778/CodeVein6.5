
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("���� ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (cs.hMenu)
	{
		::DestroyMenu(cs.hMenu);
		cs.hMenu = nullptr;
	}
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;


	cs.x = 0;
	cs.y = 100;
	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_MainSplitter.CreateStatic(this, 1, 2);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, 900), pContext);
	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(WINCX*0.4, 900), pContext);
	m_MainSplitter.SetColumnInfo(1, 500, 900);

	return TRUE;
}
