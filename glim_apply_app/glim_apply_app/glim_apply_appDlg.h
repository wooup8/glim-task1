
// glim_apply_appDlg.h: 헤더 파일
//

#pragma once




#include <vector>
using namespace std;

// CglimapplyappDlg 대화 상자
class CglimapplyappDlg : public CDialogEx
{
private:
	CImage m_image;

	CString m_module_path;
	void GetModulePath();
	int detect_circle(char* szFilePath);
	
	CPen m_pen_white;
	CPen m_pen_black;
	CPen* m_pOldPen;

	CBrush m_brush_red;
	CBrush m_brush_white;
	CBrush* m_pOldBrush;

	CFont m_font_120;
	CFont* m_pOldFont;



// 생성입니다.
public:
	CglimapplyappDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIM_APPLY_APP_DIALOG };
#endif

	typedef struct circle_pos
	{
		int x;
		int y;
	}circle_pos;
	vector<circle_pos> m_vCirclePos;
	CRect m_rect_draw;

#define MOVING_PIXEL_UNIT 20  // 이동시키려는 픽셀 개수 단위

#define TIMER_MOVING_CIRCLE 1

#define DRAW_RECT_WIDTH 640
#define DRAW_RECT_HEIGHT 480

#define COLORREF_DRAW_BG RGB(100,100,100)

	int m_circle_cx;
	int m_circle_cy;

	void draw_circle(int x, int y, int cx, int cy);
	int m_current_index; // 현재 이동 단계

	CDC m_memDC;
	CBitmap m_memCB;

	void save_image(char* szFilePath);
	void load_image(char* szFilePath);


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtondraw();
	afx_msg void OnBnClickedButtonaction();
	afx_msg void OnBnClickedButtonopen();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
