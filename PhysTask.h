
// PhysTask.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CPhysTaskApp:
// Сведения о реализации этого класса: PhysTask.cpp
//

class CPhysTaskApp : public CWinApp
{
public:
	CPhysTaskApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CPhysTaskApp theApp;
