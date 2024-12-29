
// glim_apply_appDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "glim_apply_app.h"
#include "glim_apply_appDlg.h"
#include "afxdialogex.h"



#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib,"../opencv/opencv_world4100d")
#else
#pragma comment(lib,"../opencv/opencv_world4100")
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CglimapplyappDlg 대화 상자



CglimapplyappDlg::CglimapplyappDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIM_APPLY_APP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CglimapplyappDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CglimapplyappDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_draw, &CglimapplyappDlg::OnBnClickedButtondraw)
	ON_BN_CLICKED(IDC_BUTTON_action, &CglimapplyappDlg::OnBnClickedButtonaction)
	ON_BN_CLICKED(IDC_BUTTON_open, &CglimapplyappDlg::OnBnClickedButtonopen)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CglimapplyappDlg::GetModulePath()
{
	char szPath[MAX_PATH] = { 0, };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	m_module_path = szPath;

	int pos = m_module_path.ReverseFind('\\');

	m_module_path = m_module_path.Left(pos + 1);
}
// CglimapplyappDlg 메시지 처리기

BOOL CglimapplyappDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	
	
	GetModulePath();



	SetWindowPos(&wndTopMost, 0, 0, 700, 680, SWP_HIDEWINDOW);

	srand(time(NULL));
	CString t_str = "";
	t_str.Format("%d", 100 + rand() % 100);
	SetDlgItemText(IDC_EDIT_begin_x, t_str);
	SetDlgItemText(IDC_EDIT_begin_y, t_str);

	t_str.Format("%d", 300 + rand() % 100);
	SetDlgItemText(IDC_EDIT_end_x, t_str);
	SetDlgItemText(IDC_EDIT_end_y, t_str);

	m_rect_draw.SetRect(10, 130, 10 + DRAW_RECT_WIDTH, 130 + DRAW_RECT_HEIGHT);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CClientDC dc(this);
	m_memDC.CreateCompatibleDC(&dc);
	m_memCB.CreateCompatibleBitmap(&dc, DRAW_RECT_WIDTH, DRAW_RECT_HEIGHT);
	m_memDC.SelectObject(&m_memCB);
	m_memCB.DeleteObject();
	m_memDC.FillSolidRect(0, 0, m_rect_draw.Width(), m_rect_draw.Height(), COLORREF_DRAW_BG);


	m_pen_white.CreatePen(PS_SOLID, 1, RGB(255,255,255));
	m_pen_black.CreatePen(PS_SOLID, 1, RGB(0,0,1));
	m_brush_white.CreateSolidBrush(RGB(255, 255, 255));
	m_font_120.CreatePointFont(120, _T("Tahoma"));


	m_circle_cx = 50; // draw 안하고 action 눌렀을때 초기값
	m_circle_cy = 50; // draw 안하고 action 눌렀을때 초기값


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CglimapplyappDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CglimapplyappDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);

		dc.BitBlt(m_rect_draw.left, m_rect_draw.top, m_rect_draw.Width(), m_rect_draw.Height(),
			&m_memDC, 0, 0, SRCCOPY);
		CDialog::OnPaint();

		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CglimapplyappDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CglimapplyappDlg::detect_circle(char* szFilePath)
{
	char t_szFilePath[MAX_PATH] = { 0 };
	strcpy(t_szFilePath, szFilePath);


	cv::Mat src = cv::imread(t_szFilePath, cv::IMREAD_COLOR);
	if (src.empty())
	{
		return -1;
	}

	cv::Mat src_gray;
	cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

	cv::Mat blurred;
	cv::blur(src_gray, blurred, cv::Size(7, 7));


	vector<cv::Vec3f> circles;

	cv::HoughCircles(blurred, circles, cv::HOUGH_GRADIENT, 1, 30, 50, 60);

	if (circles.size() < 1)
	{
		AfxMessageBox("검출된 원이 없습니다.");
		return -1;
	}
	
	for (int i = 0;i< circles.size();i++)
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);

		
		printf("\n==============================\n");
		printf("[opencv] x,y=%d,%d, radius=%d", center.x, center.y, radius);
		printf("\n==============================\n");

	//	cv::circle(src, center, 2, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
	//	cv::circle(src, center, radius, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);

		m_pOldPen = m_memDC.SelectObject(&m_pen_black);
		m_memDC.MoveTo(center.x - 5, center.y - 5);
		m_memDC.LineTo(center.x + 5, center.y + 5);

		m_memDC.MoveTo(center.x + 5, center.y - 5);
		m_memDC.LineTo(center.x - 5, center.y + 5);
		m_memDC.SelectObject(&m_pOldPen);
		

		char t_szPoint[256] = { 0 };
		sprintf(t_szPoint, "(%d,%d)", center.x, center.y);
		m_pOldFont = (CFont*)m_memDC.SelectObject(&m_font_120);
		m_memDC.SetBkMode(TRANSPARENT);
		m_memDC.SetTextAlign(TA_CENTER);
		m_memDC.SetTextColor(RGB(0, 0, 0));
		m_memDC.TextOut(center.x, center.y, t_szPoint);
		m_memDC.SelectObject(&m_pOldFont);
	}

	InvalidateRect(m_rect_draw,false);
	
	return 1;
	
}
void CglimapplyappDlg::load_image(char* szFilePath)
{
	CImage t_image;
	t_image.Load(szFilePath);

	t_image.BitBlt(m_memDC.m_hDC, 0, 0);

	t_image.Destroy();

	InvalidateRect(m_rect_draw,false);
}
void CglimapplyappDlg::save_image(char* szFilePath)
{
	CImage t_image;
	t_image.Create(DRAW_RECT_WIDTH, DRAW_RECT_HEIGHT, 24);
	CDC* pDC = CDC::FromHandle(t_image.GetDC());

	pDC->BitBlt(0, 0, DRAW_RECT_WIDTH, DRAW_RECT_HEIGHT, &m_memDC, 0, 0, SRCCOPY);

	ReleaseDC(pDC);
	t_image.ReleaseDC();

	

	SYSTEMTIME st;
	GetLocalTime(&st);

	CString str;
	str.Format("%s\\%04d%02d%02d_%02d%02d%02d.bmp", m_module_path, st.wYear, st.wMonth, st.wDay,st.wHour, st.wMinute, st.wSecond);

	t_image.Save(str);
	t_image.Destroy();

}

void CglimapplyappDlg::OnBnClickedButtondraw()
{
	
	int t_begin_x = GetDlgItemInt(IDC_EDIT_begin_x, 0);
	int t_begin_y = GetDlgItemInt(IDC_EDIT_begin_y, 0);
	printf("begin x,y=(%d,%d)\n", t_begin_x, t_begin_y);


	CString t_str = "";
	t_str.Format("[오류] x값은 0~%d, y값은 0~%d 로 해주세요!", DRAW_RECT_WIDTH, DRAW_RECT_HEIGHT);

	if (t_begin_x < 0 || t_begin_y < 0)
	{
		AfxMessageBox(t_str, MB_ICONWARNING);
		return;
	}
	else if (t_begin_x > DRAW_RECT_WIDTH || t_begin_y > DRAW_RECT_HEIGHT )
	{
		AfxMessageBox(t_str, MB_ICONWARNING);
		return;
	}


	m_circle_cx = 50 + rand() % 50;
	m_circle_cy = 50 + rand() % 50;

	m_circle_cy = m_circle_cx;

	printf("rand radius=%d\n", m_circle_cx/2);


	draw_circle(t_begin_x, t_begin_y, m_circle_cx, m_circle_cy);
}
void CglimapplyappDlg::draw_circle(int x, int y, int cx, int cy)
{
	CClientDC dc(this);

	int t_radius_x = m_circle_cx / 2;
	int t_radius_y = m_circle_cy / 2;

	int t_x1 = x - t_radius_x;
	int t_y1 = y - t_radius_y;

	int t_x2 = x + t_radius_x;
	int t_y2 = y + t_radius_y;


	m_memDC.FillSolidRect(0, 0, m_rect_draw.Width(), m_rect_draw.Height(), COLORREF_DRAW_BG);

	m_pOldBrush = m_memDC.SelectObject(&m_brush_white);
//	m_pOldPen = m_memDC.SelectObject(&m_pen_white);
	m_memDC.Ellipse(t_x1,t_y1,t_x2,t_y2);
	m_memDC.SelectObject(&m_pOldBrush);
//	m_memDC.SelectObject(&m_pOldPen);

	InvalidateRect(m_rect_draw);
}

void CglimapplyappDlg::OnBnClickedButtonaction()
{

	int t_begin_x = GetDlgItemInt(IDC_EDIT_begin_x, 0);
	int t_begin_y = GetDlgItemInt(IDC_EDIT_begin_y, 0);

	int t_end_x = GetDlgItemInt(IDC_EDIT_end_x, 0);
	int t_end_y = GetDlgItemInt(IDC_EDIT_end_y, 0);

	CString t_str = "";
	t_str.Format("[오류] x값은 0~%d, y값은 0~%d 로 해주세요!", DRAW_RECT_WIDTH, DRAW_RECT_HEIGHT);

	if (t_begin_x < 0 || t_begin_y < 0 ||
		t_end_x < 0 || t_end_y < 0 )
	{
		AfxMessageBox(t_str, MB_ICONWARNING);
		return;
	}
	else if (t_begin_x > DRAW_RECT_WIDTH || t_begin_y > DRAW_RECT_HEIGHT ||
		t_end_x > DRAW_RECT_WIDTH || t_end_y > DRAW_RECT_HEIGHT)
	{
		AfxMessageBox(t_str, MB_ICONWARNING);
		return;
	}
	

	m_vCirclePos.clear();

	circle_pos t_struct_pos;
	t_struct_pos.x = t_begin_x;
	t_struct_pos.y = t_begin_y;


	int t_distance_x = t_end_x - t_begin_x;
	int t_distance_y = t_end_y - t_begin_y;

	int t_step_x = abs(t_distance_x)/ MOVING_PIXEL_UNIT;
	int t_step_y = abs(t_distance_y) / MOVING_PIXEL_UNIT;

	int t_step = min(t_step_x, t_step_y);

	printf("dis x,y=%d,%d\n", t_distance_x, t_distance_y);
	printf("step=%d\n", t_step);
	if (t_step == 0)
	{
		t_struct_pos.x = t_end_x;
		t_struct_pos.y = t_end_y;
		m_vCirclePos.push_back(t_struct_pos);
	}
	else
	{
		int t_unit_x = t_distance_x / t_step;
		int t_unit_y = t_distance_y / t_step;
		for (int i = 0; i < t_step; i++)
		{
			t_struct_pos.x += t_unit_x;
			t_struct_pos.y += t_unit_y;
			m_vCirclePos.push_back(t_struct_pos);

			printf("[%d] x,y=%d,%d\n", i, t_struct_pos.x, t_struct_pos.y);
		}

		if ((t_distance_x % t_step != 0) || (t_distance_y % t_step != 0))
		{
			t_struct_pos.x = t_end_x;
			t_struct_pos.y = t_end_y;
			m_vCirclePos.push_back(t_struct_pos);
		}
	}
	

	m_current_index = 0;
	SetTimer(TIMER_MOVING_CIRCLE, 50, NULL);
}


void CglimapplyappDlg::OnBnClickedButtonopen()
{
	TCHAR szFilter[] = _T("BMP Files(*.bmp)|*.bmp|");
	CFileDialog dlg(TRUE, _T("bmp"), _T("*.bmp"), OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT, szFilter);

	dlg.m_ofn.lpstrInitialDir = m_module_path;
	dlg.m_ofn.lpstrTitle = _T("Load BMP Image...");
	if (dlg.DoModal() == IDOK) 
	{
		CString t_str = dlg.GetPathName();	//파일명을 받아서 저장한다.
		load_image((char *)(const char*)t_str);

		detect_circle((char*)(const char*)t_str);
	}

}


void CglimapplyappDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_MOVING_CIRCLE)
	{
		draw_circle(m_vCirclePos[m_current_index].x, m_vCirclePos[m_current_index].y, m_circle_cx,m_circle_cy);
		m_current_index++;
		if (m_current_index == m_vCirclePos.size())
		{
			KillTimer(TIMER_MOVING_CIRCLE);
			save_image("");
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CglimapplyappDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_RETURN ||
			pMsg->wParam == VK_ESCAPE)
		{
			return 1;
		}
		break;
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
