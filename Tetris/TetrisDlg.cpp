// TetrisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tetris.h"
#include "TetrisDlg.h"
#include "conio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTetrisDlg dialog




CTetrisDlg::CTetrisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTetrisDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAIN);
}

void CTetrisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BOOL CTetrisDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN)
		OnKeyDown((UINT)pMsg->wParam);
	return false;
}

BEGIN_MESSAGE_MAP(CTetrisDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_START, &CTetrisDlg::OnBnClickedStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, &CTetrisDlg::OnBnClickedStop)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTetrisDlg message handlers

BOOL CTetrisDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, TRUE);		// Set small icon



	return TRUE;  
}


void CTetrisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		drawGame();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTetrisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CTetrisDlg::drawGame(){
	drawBoard();
	drawNext();
	
}
void CTetrisDlg::drawBoard(){

	CWnd *hwnd = GetDlgItem(IDC_BROAD);
	CDC *pDC = hwnd->GetDC();
	CRect rect;
	hwnd->GetClientRect(&rect);
	int nWidth = rect.Width();
	int nHeight = rect.Height();
	CDC   MemDC;  
	CBitmap   MemBitmap;
	MemDC.CreateCompatibleDC(NULL); 
	MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight); 

	
	CBitmap   *pOldBit=MemDC.SelectObject(&MemBitmap); 

	
	MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));

	CPen pen(PS_SOLID, 1, RGB(148, 0, 211));
	
	CPen* pOldPen=MemDC.SelectObject(&pen);
	
	
	float gap = rect.Height() * 1.0 / (Game::ROW);
	for(int i = 0;i<Game::ROW;++i){
		MemDC.MoveTo(0,i * gap);
		MemDC.LineTo(rect.Width(),i * gap);
	}
	gap = rect.Width() * 1.0 / (Game::COL);
	for(int i = 0;i<Game::COL;++i){
		MemDC.MoveTo(i * gap,0);
		MemDC.LineTo(i * gap,rect.Height());
	}
	MemDC.SelectObject(pOldPen);

	/*the sicked block*/
	
	for(int i = 0;i<Game::ROW;++i){
		for(int j = 0;j<Game::COL;++j){
			int hc = game.hasChess(i,j);
			if(hc != -1){
				CBrush brush;
				//AllocConsole();
				//cprintf("%d %d %d\n",i,j, hc);
				brush.CreateSolidBrush(Game::colors[hc]);
				CBrush *oldBrush = MemDC.SelectObject(&brush);
				MemDC.Rectangle(j * rect.Width()/Game::COL,
					i * rect.Height()/Game::ROW,
					(j + 1) * rect.Width()/Game::COL,
					(i + 1) * rect.Height()/Game::ROW);
				MemDC.SelectObject(oldBrush);
				brush.DeleteObject();
			}
		}
	}
	

	/*current block*/
	int c = game.getCurrentColor();
	CBrush brush;
	brush.CreateSolidBrush(Game::colors[c]);
	CBrush *oldBrush = MemDC.SelectObject(&brush);
	for(int i = 0;i<4;++i){
		for(int j = 0;j<4;++j){
			int pos = game.getChessPos(i,j);
			
			if(pos != -1){
				int x = pos >> 4;
				int y = pos & 0x0f;
				MemDC.Rectangle(y * rect.Width()/Game::COL,
					x * rect.Height()/Game::ROW,
					(y + 1) * rect.Width()/Game::COL,
					(x + 1) * rect.Height()/Game::ROW);
			}
		}
	}
	MemDC.SelectObject(oldBrush);
	brush.DeleteObject();

	pDC-> BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY); 

	MemBitmap.DeleteObject(); 
	MemDC.DeleteDC(); 
	
	
	

	/*set the score*/
	CString str;
	str.Format("%d",game.getScore());
	GetDlgItem(IDC_SCORE)->SetWindowText(str);

	
	
	
}
/*draw the next block*/
void CTetrisDlg::drawNext(){
	CWnd *next_hwnd = GetDlgItem(IDC_NEXT);
	CDC *pDC = next_hwnd->GetDC();
	CRect next_rect;
	next_hwnd->GetClientRect(&next_rect);
	int nWidth = next_rect.Width();
	int nHeight = next_rect.Height();
	CDC  MemDC;  
	CBitmap  MemBitmap;
	MemDC.CreateCompatibleDC(NULL); 
	MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight); 


	CBitmap   *pOldBit=MemDC.SelectObject(&MemBitmap); 


	MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));
	for(int i = 0;i<4;++i){
		for(int j = 0;j<4;++j){
			if(game.hasChessInHere(i,j)){
				MemDC.Rectangle(j * next_rect.Width()/4,
					i * next_rect.Height()/4,
					(j + 1) * next_rect.Width()/4,
					(i + 1) * next_rect.Height()/4);
			}
		}
	}
	pDC-> BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY); 

	MemBitmap.DeleteObject(); 
	MemDC.DeleteDC(); 
}

void CTetrisDlg::OnBnClickedStart()
{
	game.startGame();
	SetTimer(1,500,NULL);
}

void CTetrisDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(!game.moveDown()){
		if(game.lose()){
			KillTimer(1);
			CString str;
			if(game.getScore() >= 200)
				str.Format("hdhd, you get %ld point",game.getScore());
			else{
				str.Format("Game over, keep fighting!");
			}
			MessageBox(str);
		}else{
			game.getNextChess();
			
		}
	}
	Invalidate(true);
	
	CDialog::OnTimer(nIDEvent);
}

void CTetrisDlg::OnBnClickedStop()
{
	game.stopGame();
	KillTimer(1);
	
}

void CTetrisDlg::OnKeyDown(UINT nChar)
{
	game.move(nChar);
	//drawGame();
	Invalidate(true);
}

BOOL CTetrisDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}
