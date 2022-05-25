
// PhysTaskDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "PhysTask.h"
#include "PhysTaskDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeEditv0();
	afx_msg void OnBnClickedRadio1();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CAboutDlg::OnBnClickedRadio1)
END_MESSAGE_MAP()


// Диалоговое окно CPhysTaskDlg



CPhysTaskDlg::CPhysTaskDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PHYSTASK_DIALOG, pParent)
	, s_angle(_T(""))
	, s_V0(_T(""))
	, s_mass(_T(""))
	, s_dt(_T(""))
	, s_D(_T(""))
	, s_C_d(_T(""))
	, s_output(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhysTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITANGLE, s_angle);
	DDX_Text(pDX, IDC_EDITV0, s_V0);
	DDX_Text(pDX, IDC_EDITMASS, s_mass);
	DDX_Text(pDX, IDC_EDITDT, s_dt);
	DDX_Text(pDX, IDC_EDITDIAMETER, s_D);
	DDX_Text(pDX, IDC_EDITC_D, s_C_d);
	DDX_Text(pDX, IDC_EDITOUTPUT, s_output);
	DDX_Control(pDX, IDC_STATICOUT, result);
	DDX_Control(pDX, IDC_RADIO1, radio_button1);
	DDX_Control(pDX, IDC_RADIO2, radio_button2);
}

BEGIN_MESSAGE_MAP(CPhysTaskDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONSTART, &CPhysTaskDlg::OnBnClickedButtonstart)
	ON_COMMAND(IDOK, &CPhysTaskDlg::OnIdok)
	
	ON_BN_CLICKED(IDC_BUTTONPRESET1, &CPhysTaskDlg::OnBnClickedButtonpreset1)
	ON_BN_CLICKED(IDC_STATICFULLGRAPH, &CPhysTaskDlg::OnBnClickedStaticfullgraph)
	ON_EN_CHANGE(IDC_EDITV0, &CPhysTaskDlg::OnEnChangeEditv0)
	ON_EN_CHANGE(IDC_EDITANGLE, &CPhysTaskDlg::OnEnChangeEditangle)
	ON_BN_CLICKED(IDC_RADIO1, &CPhysTaskDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CPhysTaskDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// Обработчики сообщений CPhysTaskDlg

BOOL CPhysTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	GetDlgItem(IDC_STATICGRAPH)->GetWindowRect(&mainGraph);
	GetDlgItem(IDC_STATICKINETICGRAPH)->GetWindowRect(&kineticGraph);
	GetDlgItem(IDC_STATICPOTENTIALGRAPH)->GetWindowRect(&potentialGraph);
	GetDlgItem(IDC_STATICFULLGRAPH)->GetWindowRect(&fullEnergyGraph);
	GetDlgItem(IDC_STATICVXGRAPH)->GetWindowRect(&vxGraphRect);
	GetDlgItem(IDC_STATICVYGRAPH)->GetWindowRect(&vyGraphRect);

	ScreenToClient(&mainGraph);
	ScreenToClient(&kineticGraph);
	ScreenToClient(&potentialGraph);
	ScreenToClient(&fullEnergyGraph);
	ScreenToClient(&vxGraphRect);
	ScreenToClient(&vyGraphRect);


	s_V0 = "5";
	s_angle = "45";
	s_mass = "1";
	s_C_d = "1";
	s_D = "1";
	s_dt = "0.1";

	UpdateData(0);


	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CPhysTaskDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CPhysTaskDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CPhysTaskDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPhysTaskDlg::OnBnClickedButtonstart()
{
	// TODO: добавьте свой код обработчика уведомлений

	CDC* dc = GetDC();
	CBrush brush(RGB(0, 0, 0));

	UpdateParams();
	
	simulation.StartSimulation();
	s_output = simulation.SendOutput().c_str();
	result.SetWindowTextW(simulation.Result().c_str());

	UpdateData(0);
}

void CPhysTaskDlg::UpdateParams()
{
	UpdateData();
	double V0 =		wcstod(s_V0,NULL);
	double angle =	wcstod(s_angle, NULL);
	double mass =	wcstod(s_mass,NULL);
	double c_d =	wcstod(s_C_d,NULL);
	double D =		wcstod(s_D,NULL);
	double dt =		wcstod(s_dt, NULL);
	simulation.UpdateParams(
		V0,
		angle,
		mass,
		c_d,
		D,
		dt,
		GetDC(),
		mainGraph,
		kineticGraph,
		potentialGraph,
		fullEnergyGraph,
		vxGraphRect,
		vyGraphRect,
		radio_button1.GetCheck()
	);

	UpdateData(1);
}


void CPhysTaskDlg::OnIdok()
{
	// TODO: добавьте свой код обработчика команд
	OnBnClickedButtonstart();
}


void CPhysTaskDlg::OnBnClickedButtonpreset1()
{
	// TODO: добавьте свой код обработчика уведомлений
	s_V0 = L"1600";
	s_angle = L"55";
	s_mass = L"120";
	s_C_d = L"0.3";
	s_D = L"0.210";
	s_dt = L"0.1";
	UpdateData(0);
}


void CPhysTaskDlg::OnBnClickedStaticfullgraph()
{
	// TODO: добавьте свой код обработчика уведомлений
}


void CPhysTaskDlg::OnEnChangeEditv0()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления

	UpdateData();
	double V0 = wcstod(s_V0, NULL);
	double angle = wcstod(s_angle, NULL);


	//double L = V0 * V0 / 2. * sin(2 * angle / 180. * 3.1415926535);
	double T = 2 * V0 * sin(angle / 180. * 3.1415926535) / 9.81;
	s_dt.Empty();
	s_dt.Format(L"%f", T/1000.);
	UpdateData(0);
}


void CPhysTaskDlg::OnEnChangeEditangle()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления

	UpdateData();
	double V0 = wcstod(s_V0, NULL);
	double angle = wcstod(s_angle, NULL);


	//double L = V0 * V0 / 2. * sin(2 * angle / 180. * 3.1415926535);
	double T = 2 * V0 * sin(angle / 180. * 3.1415926535) / 9.81;
	s_dt.Empty();
	s_dt.Format(L"%f", T / 1000.);
	UpdateData(0);
}


void CAboutDlg::OnBnClickedRadio1()
{
	// TODO: добавьте свой код обработчика уведомлений

}


void CPhysTaskDlg::OnBnClickedRadio1()
{
	if (radio_button2.GetState())
	{
		radio_button2.SetState(0);
	}
}


void CPhysTaskDlg::OnBnClickedRadio2()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (radio_button1.GetState())
	{
		radio_button1.SetState(0);
	}
}
