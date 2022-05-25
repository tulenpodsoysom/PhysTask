
// PhysTaskDlg.h: файл заголовка
//

#pragma once
#include "Simulation.h"
#include <string>
using namespace std;

// Диалоговое окно CPhysTaskDlg
class CPhysTaskDlg : public CDialogEx
{
// Создание
public:
	CPhysTaskDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHYSTASK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

	CRect mainGraph;
	CRect kineticGraph;
	CRect potentialGraph;
	CRect fullEnergyGraph;
	CRect vxGraphRect;
	CRect vyGraphRect;

	Simulation simulation;

// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonstart();
	void UpdateParams();
	CString s_angle;
	CString s_V0;
	CString s_mass;
	CString s_dt;
	CString s_D;
	CString s_C_d;

	CString s_output;
	afx_msg void OnIdok();
//	afx_msg void OnEnChangeEditoutput();
	afx_msg void OnBnClickedButtonpreset1();
	afx_msg void OnBnClickedStaticfullgraph();
	afx_msg void OnEnChangeEditv0();
	afx_msg void OnEnChangeEditangle();
	CStatic result;
	afx_msg void OnBnClickedRadio1();
	CButton radio_button1;
	CButton radio_button2;
	afx_msg void OnBnClickedRadio2();
};
