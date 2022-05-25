#include "pch.h"
#include "Simulation.h"
#include <string>
#include <algorithm>

#pragma warning (disable:4996)

double Simulation::Density(double h)
{
	double m = 28.97e-3;
	double g = 9.80943;		// ��� ��� ������
	double R = 8.31446;
	double density_on_land = 1.225;	// ��/�^3
	double temperature = 253;	// -20 �������� �������
	return density_on_land * exp(-m * g * h / (R * temperature));
}

double Simulation::ForceResistance(double c_d, double density, double S, double Vx, double Vy)
{
	return c_d * density * S * (Vx * Vx + Vy * Vy) / 2;
}


double Simulation::FuncX(double vx, double vy, double y)
{
	double S = 3.141592 * D * D / 4.;
	return -c_d * Density(y) * S * sqrt(vx * vx + vy * vy) * vx / 2. / mass;
}

double Simulation::FuncY(double vx, double vy, double y)
{
	double g = 9.81;
	double S = 3.141592 * D * D / 4.;
	return -c_d * Density(y) * S * sqrt(vx * vx + vy * vy) * vy / 2. / mass - g;
}

void Simulation::DifferentialSolve()
{
	auto a = projectiles.end() - 1;

	double vx = a->vx;
	double vy = a->vy;
	double x = a->x;
	double y = a->y;

	/*� ��������� � ����� �������� �� ����������� t, x*/
	double k1x = FuncX(vx, vy, y) * dt;
	double k1y = FuncY(vx, vy, y) * dt;

	double k2x = FuncX(vx + k1x / 2., vy + k1y / 2., y + vy * dt / 2.) * dt;
	double k2y = FuncY(vx + k1x / 2., vy + k1y / 2., y + vy * dt / 2.) * dt;

	double k3x = FuncX(vx + k2x / 2., vy + k2y / 2., y + vy * dt / 2. + k1y / 4. * dt) * dt;
	double k3y = FuncY(vx + k2x / 2., vy + k2y / 2., y + vy * dt / 2. + k1y / 4. * dt) * dt;

	double k4x = FuncX(vx + k3x, vy + k3y, y + vy * dt / 2. + k2y * dt / 2.) * dt;
	double k4y = FuncY(vx + k3x, vy + k3y, y + vy * dt / 2. + k2y * dt / 2.) * dt;


	double x_ = x + vx * dt + 1. / 6. * (k1x + k2x + k3x) * dt;
	double y_ = y + vy * dt + 1. / 6. * (k1y + k2y + k3y) * dt;

	double vx_ = vx + 1. / 6. * (k1x + 2*k2x + 2*k3x + k4x);
	double vy_ = vy + 1. / 6. * (k1y + 2*k2y + 2*k3y + k4y);

	projectiles.push_back({ x_,y_,vx_,vy_ });
}


void Simulation::StartSimulation()
{
	projectiles.clear();

	/*���������� ������*/
	LOGFONT lf = {};
	lf.lfHeight = 12;
	lf.lfWidth = 6;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	wcscpy(lf.lfFaceName, L"Arial");

	CFont font; font.CreateFontIndirectW(&lf);
	SelectObject(*mainDC, font);

	/*��������� �����*/
	projectiles.push_back({ 0., 0., V0 * cos(angle), V0 * sin(angle) });

	/*������� ����������������� ���������*/
	int k = 0;
	do
	{
		DifferentialSolve();
		k++;
	} while ((projectiles.end() - 1)->y > 0. && k < 200000);


	/*��������� ���������� �������*/

	if (TorX)
		MainGraph(mainDC, mainRect);
	else
	{
		MainGraphT(mainDC, mainRect);
	}
	/*��������� ������������ �������*/
	KineticGraph(mainDC, kineticRect);
	/*��������� ������������� �������*/
	PotentialGraph(mainDC, potentialRect);
	/*��������� ������ �������*/
	FullEnergyGraph(mainDC, fullEnergyRect);

	/*��������� Vy*/
	VyGraph(mainDC, vyGraphRect);
	/*��������� Vx*/
	VxGraph(mainDC, vxGraphRect);


}

wstring Simulation::SendOutput()
{
	//wstring str;
	CString str;
	str.Append(L"����� ����������:\r\n");

	int k = 0;
	for (auto& i : projectiles)
	{
		str.AppendFormat(L"x: %.3f y: %.3f vx: %.3f vy: %.3f t: %.3f ���������: %.3f \r\n", i.x, i.y, i.vx, i.vy, k*dt, Density(i.y));
		//str += L"x: " + to_wstring(i.x) + L" y: " + to_wstring(i.y);
		/*str += L" vx: " + to_wstring(i.vx) + L" vy: " + to_wstring(i.vy);
		str += L" t: " + to_wstring(k * dt) + L" density: " + to_wstring(Density(i.y));
		str += L"\r\n";*/
		k++;
	}

	return str;
}

wstring Simulation::Result()
{
	vector<double> values;
	for (auto& i : projectiles)
	{
		values.push_back(i.y);
	}
	auto ymax = max_element(values.begin(), values.end());
	CString output; output.Format(L" ��������� ������: %.1f �. \r\n ������ ������: %.1f �. \r\n ����� ������: %.1f �.", (projectiles.end() - 1)->x, *ymax,(projectiles.size()-1) * dt);
	return output;
}

void Simulation::StopSimulation()
{
}

void Simulation::RestartSimulation()
{

}

void Simulation::MainGraph(CDC* dc, CRect rect)
{
	//CDC* dc = new CDC; dc->CreateCompatibleDC(dc_);
	HBITMAP bitmap = CreateCompatibleBitmap(*dc, 1920, 1080);
	dc->SelectObject(bitmap);
	/*������������ �������*/
	
	CBrush brush(RGB(240, 240, 240));
	dc->FillRect(rect, &brush);

	/*	----	��������� �������	----	*/
	double xmin = 0, xmax = (projectiles.end() - 1)->x * 1.1;
	double ymin = 0, ymax = 5;
	for (auto& i : projectiles)
		if (i.y * 1.1 > ymax) ymax = i.y * 1.1;

	graphRect = CRect{ rect.left + 8 * 5, rect.top + 16, rect.right, rect.bottom - 12 };
	double scalex = graphRect.Width() / (xmax - xmin);
	double scaley = graphRect.Height() / (ymax - ymin);


	CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
	HGDIOBJ prevobject = GetCurrentObject(*dc, OBJ_PEN);
	SelectObject(*dc, pen);
	dc->SetBkColor(RGB(240, 240, 240));
	/*��������� �����*/
	int imax = 10;
	for (int i = 0; i < imax; i++)
	{
		double x = (xmax - xmin) * i / imax * scalex;
		double y = (ymax - ymin) * i / imax * scaley;

		MoveToEx(*dc, graphRect.left, graphRect.bottom - y, NULL);
		LineTo(*dc, graphRect.right, graphRect.bottom - y);

		MoveToEx(*dc, graphRect.left + x, graphRect.bottom, NULL);
		LineTo(*dc, graphRect.left + x, graphRect.top);


		CString strx; strx.Format(L"%.1f", x / scalex);
		CString stry; stry.Format(L"%.1f", y / scaley);

		TextOut(*dc, graphRect.left + x - 4, graphRect.bottom, strx, strx.GetLength());
		TextOut(*dc, graphRect.left - 8 * 5, graphRect.bottom - y - 8, stry, stry.GetLength());
	}SelectObject(*dc, prevobject);

	TextOut(*dc, rect.left, rect.top, L"���������� �������", 19);
	TextOut(*dc, rect.left, rect.top + 16 - 3, L"Y, �", 5);
	TextOut(*dc, rect.right - 16, rect.bottom - 8, L"X, �", 5);

	/*��� X*/
	MoveToEx(*dc, graphRect.left, graphRect.bottom, NULL);
	LineTo(*dc, graphRect.right, graphRect.bottom);

	/*��� Y*/
	MoveToEx(*dc, graphRect.left, graphRect.bottom, NULL);
	LineTo(*dc, graphRect.left, graphRect.top);


	/*��������� ����������*/
	auto a = projectiles.begin();
	for (int l = 0; l < projectiles.size() - 1; l++)
	{
		int x = (*(a + l)).x * scalex + graphRect.left;
		int y = -(*(a + l)).y * scaley + graphRect.bottom;

		int x_ = (*(a + l + 1)).x * scalex + graphRect.left;
		int y_ = -(*(a + l + 1)).y * scaley + graphRect.bottom;

		if (graphRect.PtInRect(CPoint(x_, y_)))
		{
			MoveToEx(*dc, x, y, NULL);
			LineTo(*dc, x_, y_);
		}
	}

	CImage image; image.Attach(bitmap);
	image.Save(L"MainGraphRender.png");

	//DeleteObject(bitmap);
}
void Simulation::MainGraphT(CDC* dc, CRect rect)
{
	//CDC* dc = new CDC; dc->CreateCompatibleDC(dc_);
	HBITMAP bitmap = CreateCompatibleBitmap(*dc, 1920, 1080);
	dc->SelectObject(bitmap);
	/*������������ �������*/

	CBrush brush(RGB(240, 240, 240));
	dc->FillRect(rect, &brush);

	/*	----	��������� �������	----	*/
	double xmin = 0, xmax = 1.1 * projectiles.size() * dt; /*(projectiles.end() - 1)->x * 1.1*/;
	double ymin = 0, ymax = 5;
	for (auto& i : projectiles)
		if (i.y * 1.1 > ymax) ymax = i.y * 1.1;

	graphRect = CRect{ rect.left + 8 * 5, rect.top + 16, rect.right, rect.bottom - 12 };
	double scalex = graphRect.Width() / (xmax - xmin);
	double scaley = graphRect.Height() / (ymax - ymin);


	CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
	HGDIOBJ prevobject = GetCurrentObject(*dc, OBJ_PEN);
	SelectObject(*dc, pen);
	dc->SetBkColor(RGB(240, 240, 240));
	/*��������� �����*/
	int imax = 10;
	for (int i = 0; i < imax; i++)
	{
		double x = (xmax - xmin) * i / imax * scalex;
		double y = (ymax - ymin) * i / imax * scaley;

		MoveToEx(*dc, graphRect.left, graphRect.bottom - y, NULL);
		LineTo(*dc, graphRect.right, graphRect.bottom - y);

		MoveToEx(*dc, graphRect.left + x, graphRect.bottom, NULL);
		LineTo(*dc, graphRect.left + x, graphRect.top);


		CString strx; strx.Format(L"%.1f", x / scalex);
		CString stry; stry.Format(L"%.1f", y / scaley);

		TextOut(*dc, graphRect.left + x - 4, graphRect.bottom, strx, strx.GetLength());
		TextOut(*dc, graphRect.left - 8 * 5, graphRect.bottom - y - 8, stry, stry.GetLength());
	}SelectObject(*dc, prevobject);

	TextOut(*dc, rect.left, rect.top, L"����������� ���������� �� �������: ", 36);
	TextOut(*dc, rect.left, rect.top + 16 - 3, L"Y, �", 5);
	TextOut(*dc, rect.right - 16, rect.bottom - 8, L"t, �", 5);

	/*��� X*/
	MoveToEx(*dc, graphRect.left, graphRect.bottom, NULL);
	LineTo(*dc, graphRect.right, graphRect.bottom);

	/*��� Y*/
	MoveToEx(*dc, graphRect.left, graphRect.bottom, NULL);
	LineTo(*dc, graphRect.left, graphRect.top);


	/*��������� ����������*/
	auto a = projectiles.begin();
	for (int l = 0; l < projectiles.size() - 1; l++)
	{
		//int x = (*(a + l)).x * scalex + graphRect.left;
		int x = l * dt * scalex + graphRect.left;
		int y = -(*(a + l)).y * scaley + graphRect.bottom;

		//int x_ = (*(a + l + 1)).x * scalex + graphRect.left;
		int x_ = (l + 1) * dt * scalex + graphRect.left;
		int y_ = -(*(a + l + 1)).y * scaley + graphRect.bottom;

		if (graphRect.PtInRect(CPoint(x_, y_)))
		{
			MoveToEx(*dc, x, y, NULL);
			LineTo(*dc, x_, y_);
		}
	}

	CImage image; image.Attach(bitmap);
	image.Save(L"MainGraphRender.png");

	//DeleteObject(bitmap);

}
void Simulation::KineticGraph(CDC* dc, CRect rect)
{
	CBrush brush(RGB(240, 240, 240));
	dc->FillRect(rect, &brush);

	vector<double> values;
	for (auto& i : projectiles)
	{
		values.push_back(mass * (i.vx * i.vx + i.vy * i.vy) / 2.);
	}

	double xmin = 0, xmax = values.size() * dt;
	double ymin = 0, ymax = *max_element(values.begin(),values.end());

	CRect graphRect_ = CRect{ rect.left + 32, rect.top+16, rect.right, rect.bottom - 12 };
	double scalex = graphRect_.Width() / (xmax - xmin);
	double scaley = graphRect_.Height() / (ymax - ymin);

	CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
	HGDIOBJ prevobject = GetCurrentObject(*dc, OBJ_PEN);

	SelectObject(*dc, pen);
	/*��������� �����*/
	int imax = 5;
	for (int i = 0; i < imax; i++)
	{
		double x = (xmax - xmin) * i / imax * scalex;
		double y = (ymax - ymin) * i / imax * scaley;

		MoveToEx(*dc, graphRect_.left, graphRect_.bottom - y, NULL);
		LineTo(*dc, graphRect_.right, graphRect_.bottom - y);

		MoveToEx(*dc, graphRect_.left + x, graphRect_.bottom, NULL);
		LineTo(*dc, graphRect_.left + x, graphRect_.top);

		CString strx; strx.Format(L"%.1f", x / scalex);
		CString stry; stry.Format(L"%.0e", y / scaley);

		TextOut(*dc, graphRect_.left + x - 4, graphRect_.bottom, strx, strx.GetLength());
		TextOut(*dc, graphRect_.left - 32, graphRect_.bottom - y - 8, stry, stry.GetLength());
	}SelectObject(*dc, prevobject);

	wstring title = L"������������ �������";
	TextOut(*dc, rect.left, rect.top, title.c_str(), title.size());
	TextOut(*dc, rect.left-8, rect.top + 16 - 3, L"Ek, ��", 7);
	TextOut(*dc, rect.right - 16, rect.bottom - 12, L"t, �", 5);

	/*��� X*/
	MoveToEx(*dc, graphRect_.left, graphRect_.bottom, NULL);
	LineTo(*dc, graphRect_.right, graphRect_.bottom);

	/*��� Y*/
	MoveToEx(*dc, graphRect_.left, graphRect_.bottom, NULL);
	LineTo(*dc, graphRect_.left, graphRect_.top);


	/*��������� �������*/
	for (int l = 0; l < values.size() - 1; l++)
	{
		int x = l*dt * scalex + graphRect_.left;
		int y = -values[l] * scaley + graphRect_.bottom;

		int x_ = (l + 1)*dt * scalex + graphRect_.left;
		int y_ = -values[l+1] * scaley + graphRect_.bottom;

		if (graphRect_.PtInRect(CPoint(x_, y_)))
		{
			MoveToEx(*dc, x, y, NULL);
			LineTo(*dc, x_, y_);
		}
	}


}
void Simulation::PotentialGraph(CDC* dc, CRect rect)
{
	CBrush brush(RGB(240, 240, 240));
	dc->FillRect(rect, &brush);

	vector<double> values;
	for (auto& i : projectiles)
	{
		values.push_back(mass * 9.81* i.y);
	}

	double xmin = 0, xmax = values.size() * dt;
	double ymin = 0, ymax = *max_element(values.begin(), values.end());

	CRect graphRect_ = CRect{ rect.left + 32, rect.top + 16, rect.right, rect.bottom - 12 };
	double scalex = graphRect_.Width() / (xmax - xmin);
	double scaley = graphRect_.Height() / (ymax - ymin);

	CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
	HGDIOBJ prevobject = GetCurrentObject(*dc, OBJ_PEN);

	SelectObject(*dc, pen);
	/*��������� �����*/
	int imax = 5;
	for (int i = 0; i < imax; i++)
	{
		double x = (xmax - xmin) * i / imax * scalex;
		double y = (ymax - ymin) * i / imax * scaley;

		MoveToEx(*dc, graphRect_.left, graphRect_.bottom - y, NULL);
		LineTo(*dc, graphRect_.right, graphRect_.bottom - y);

		MoveToEx(*dc, graphRect_.left + x, graphRect_.bottom, NULL);
		LineTo(*dc, graphRect_.left + x, graphRect_.top);

		CString strx; strx.Format(L"%.1f", x / scalex);
		CString stry; stry.Format(L"%.0e", y / scaley);

		TextOut(*dc, graphRect_.left + x - 4, graphRect_.bottom, strx, strx.GetLength());
		TextOut(*dc, graphRect_.left - 32, graphRect_.bottom - y - 8, stry, stry.GetLength());
	}SelectObject(*dc, prevobject);

	wstring title = L"������������� �������";
	TextOut(*dc, rect.left, rect.top, title.c_str(), title.size());
	TextOut(*dc, rect.left - 8, rect.top + 16 - 3, L"Ep, ��", 7);
	TextOut(*dc, rect.right - 16, rect.bottom - 12, L"t, �", 5);

	/*��� X*/
	MoveToEx(*dc, graphRect_.left, graphRect_.bottom, NULL);
	LineTo(*dc, graphRect_.right, graphRect_.bottom);

	/*��� Y*/
	MoveToEx(*dc, graphRect_.left, graphRect_.bottom, NULL);
	LineTo(*dc, graphRect_.left, graphRect_.top);


	/*��������� �������*/
	for (int l = 0; l < projectiles.size() - 1; l++)
	{
		int x = l * dt * scalex + graphRect_.left;
		int y = -values[l] * scaley + graphRect_.bottom;

		int x_ = (l + 1) * dt * scalex + graphRect_.left;
		int y_ = -values[l + 1] * scaley + graphRect_.bottom;

		;
		if (graphRect_.PtInRect(CPoint(x_, y_)))
		{
			MoveToEx(*dc, x, y, NULL);
			LineTo(*dc, x_, y_);
		}
	}


}
void Simulation::FullEnergyGraph(CDC* dc, CRect rect)
{
	CBrush brush(RGB(240, 240, 240));
	dc->FillRect(rect, &brush);

	vector<double> values;
	for (auto& i : projectiles)
	{
		values.push_back(mass * 9.81 * i.y + mass * (i.vx * i.vx + i.vy * i.vy) / 2.);
	}

	double xmin = 0, xmax = values.size() * dt;
	double ymin = 0, ymax = *max_element(values.begin(), values.end());

	CRect graphRect_ = CRect{ rect.left + 32, rect.top + 16, rect.right, rect.bottom - 12 };
	double scalex = graphRect_.Width() / (xmax - xmin);
	double scaley = graphRect_.Height() / (ymax - ymin);

	CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
	HGDIOBJ prevobject = GetCurrentObject(*dc, OBJ_PEN);

	SelectObject(*dc, pen);
	/*��������� �����*/
	int imax = 5;
	for (int i = 0; i < imax; i++)
	{
		double x = (xmax - xmin) * i / imax * scalex;
		double y = (ymax - ymin) * i / imax * scaley;

		MoveToEx(*dc, graphRect_.left, graphRect_.bottom - y, NULL);
		LineTo(*dc, graphRect_.right, graphRect_.bottom - y);

		MoveToEx(*dc, graphRect_.left + x, graphRect_.bottom, NULL);
		LineTo(*dc, graphRect_.left + x, graphRect_.top);

		CString strx; strx.Format(L"%.1f", x / scalex);
		CString stry; stry.Format(L"%.0e", y / scaley);

		TextOut(*dc, graphRect_.left + x - 4, graphRect_.bottom, strx, strx.GetLength());
		TextOut(*dc, graphRect_.left - 32, graphRect_.bottom - y - 8, stry, stry.GetLength());
	}SelectObject(*dc, prevobject);

	wstring title = L"������ �������";
	TextOut(*dc, rect.left, rect.top, title.c_str(), title.size());
	TextOut(*dc, rect.left, rect.top + 16 - 3, L"E, ��", 6);
	TextOut(*dc, rect.right - 16, rect.bottom - 12, L"t, �", 5);

	/*��� X*/
	MoveToEx(*dc, graphRect_.left, graphRect_.bottom, NULL);
	LineTo(*dc, graphRect_.right, graphRect_.bottom);

	/*��� Y*/
	MoveToEx(*dc, graphRect_.left, graphRect_.bottom, NULL);
	LineTo(*dc, graphRect_.left, graphRect_.top);


	/*��������� �������*/
	for (int l = 0; l < values.size() - 1; l++)
	{
		int x = l*dt * scalex + graphRect_.left;
		int y = -values[l] * scaley + graphRect_.bottom;

		int x_ = (l + 1)*dt * scalex + graphRect_.left;
		int y_ = -values[l + 1] * scaley + graphRect_.bottom;

		if (graphRect_.PtInRect(CPoint(x_, y_)))
		{
			MoveToEx(*dc, x, y, NULL);
			LineTo(*dc, x_, y_);
		}
	}
}
void Simulation::VxGraph(CDC* dc, CRect rect)
{
	/*������������ �������*/
	CBrush brush(RGB(240, 240, 240));
	mainDC->FillRect(rect, &brush);

	/*	----	��������� �������	----	*/
	double xmin = 0, xmax = projectiles.size() * dt;
	double ymin = 0, ymax = 5;
	for (auto& i : projectiles)
		if (i.vx * 1.1 > ymax) ymax = i.vx * 1.1;

	graphRect = CRect{ rect.left + 8 * 5, rect.top +16, rect.right, rect.bottom - 12 };
	double scalex = graphRect.Width() / (xmax - xmin);
	double scaley = graphRect.Height() / (ymax - ymin);


	CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
	HGDIOBJ prevobject = GetCurrentObject(*dc, OBJ_PEN);

	SelectObject(*dc, pen);
	/*��������� �����*/
	int imax = 5;
	for (int i = 0; i < imax; i++)
	{
		double x = (xmax - xmin) * i / imax * scalex;
		double y = (ymax - ymin) * i / imax * scaley;

		MoveToEx(*mainDC, graphRect.left, graphRect.bottom - y, NULL);
		LineTo(*mainDC, graphRect.right, graphRect.bottom - y);

		MoveToEx(*mainDC, graphRect.left + x, graphRect.bottom, NULL);
		LineTo(*mainDC, graphRect.left + x, graphRect.top);

		CString strx; strx.Format(L"%.1f", x / scalex);
		CString stry; stry.Format(L"%.1f", y / scaley);

		TextOut(*mainDC, graphRect.left + x - 4, graphRect.bottom, strx, strx.GetLength());
		TextOut(*mainDC, graphRect.left - 8 * 5, graphRect.bottom - y - 8, stry, stry.GetLength());
	}SelectObject(*dc, prevobject);

	TextOut(*mainDC, rect.left, rect.top, L"�������� �������� �� ��� X", 27);
	TextOut(*dc, rect.left, rect.top + 16 - 3, L"Vx, �/�", 8);
	TextOut(*dc, rect.right - 16, rect.bottom - 12, L"t, �", 5);

	/*��� X*/
	MoveToEx(*mainDC, graphRect.left, graphRect.bottom, NULL);
	LineTo(*mainDC, graphRect.right, graphRect.bottom);

	/*��� Y*/
	MoveToEx(*mainDC, graphRect.left, graphRect.bottom, NULL);
	LineTo(*mainDC, graphRect.left, graphRect.top);


	auto a = projectiles.begin();
	for (int l = 0; l < projectiles.size() - 1; l++)
	{
		int x = l * graphRect.Width() / projectiles.size() + graphRect.left;
		int y = -(*(a + l)).vx * scaley + graphRect.bottom;

		int x_ = (l+1) * graphRect.Width() / projectiles.size() + graphRect.left;
		int y_ = -(*(a + l + 1)).vx * scaley + graphRect.bottom;

		if (graphRect.PtInRect(CPoint(x_, y_)))
		{
			MoveToEx(*mainDC, x, y, NULL);
			LineTo(*mainDC, x_, y_);
		}
	}
}
void Simulation::VyGraph(CDC* dc, CRect rect)
{
	/*������������ �������*/
	CBrush brush(RGB(240, 240, 240));
	mainDC->FillRect(rect, &brush);

	/*	----	��������� �������	----	*/

	double xmin = 0, xmax = projectiles.size() * dt;
	double ymin = -5, ymax = 5;
	for (auto& i : projectiles)
	{
		if (i.vy * 1.1 > ymax) ymax = i.vy * 1.1;
		if (i.vy * 1.1 < ymin) ymin = i.vy * 1.1;
	}

	graphRect = CRect{ rect.left + 8 * 5, rect.top+16, rect.right, rect.bottom - 16};
	double scalex = graphRect.Width() / (xmax - xmin);
	double scaley = graphRect.Height() / (ymax - ymin);

	CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
	HGDIOBJ prevobject = GetCurrentObject(*dc, OBJ_PEN);

	SelectObject(*dc, pen);
	/*��������� �����*/
	int imax = 5;
	for (int i = 0; i < imax; i++)
	{
		double x = (xmin + (xmax - xmin) * i / imax) * scalex;
		double y = (ymin + (ymax - ymin) * i / imax) * scaley;

		MoveToEx(*mainDC, graphRect.left, graphRect.bottom - (y - ymin * scaley), NULL);
		LineTo(*mainDC, graphRect.right, graphRect.bottom - (y - ymin * scaley));

		MoveToEx(*mainDC, graphRect.left + x, graphRect.bottom, NULL);
		LineTo(*mainDC, graphRect.left + x, graphRect.top);

		CString strx; strx.Format(L"%.1f", x / scalex);
		CString stry; stry.Format(L"%.1f", y / scaley);

		TextOut(*mainDC, graphRect.left + x - 4, graphRect.bottom, strx, strx.GetLength());
		TextOut(*mainDC, graphRect.left - 8 * 5, graphRect.bottom - (y - ymin * scaley) - 8, stry, stry.GetLength());
	}	SelectObject(*dc, prevobject);

	TextOut(*mainDC, rect.left, rect.top, L"�������� �������� �� ��� Y", 27);
	TextOut(*dc, rect.left, rect.top + 16 - 3, L"Vy, �/�", 8);
	TextOut(*dc, rect.right - 16, rect.bottom - 12, L"t, �", 5);

	/*��� X*/
	MoveToEx(*mainDC, graphRect.left, graphRect.bottom + ymin*scaley, NULL);
	LineTo(*mainDC, graphRect.right, graphRect.bottom + ymin*scaley);
	

	/*������ ���*/
	//MoveToEx(*mainDC, graphRect.left, graphRect.bottom, NULL);
	//LineTo(*mainDC, graphRect.right, graphRect.bottom);

	/*��� Y*/
	MoveToEx(*mainDC, graphRect.left, graphRect.bottom, NULL);
	LineTo(*mainDC, graphRect.left, graphRect.top);



	auto a = projectiles.begin();
	for (int l = 0; l < projectiles.size() - 1; l++)
	{
		int x = l * graphRect.Width() / projectiles.size() + graphRect.left;
		int y = (-(*(a + l)).vy + ymin) * scaley + graphRect.bottom;

		int x_ = (l + 1) * graphRect.Width() / projectiles.size() + graphRect.left;
		int y_ = (-(*(a + l + 1)).vy + ymin)* scaley + graphRect.bottom;

		if (graphRect.PtInRect(CPoint(x_, y_)))
		{
			MoveToEx(*mainDC, x, y, NULL);
			LineTo(*mainDC, x_, y_);
		}
	}

}

void Simulation::UpdateParams(double V0, double angle, double mass, double c_d, double D, double dt, CDC* dc,CRect rect, CRect rect2, CRect rect3, CRect rect4, CRect rect5, CRect rect6,bool T_or_X)
{
	this->V0 = V0;
	this->angle = angle * 3.14159265358979323 / 180.;
	this->mass = mass;
	this->c_d = c_d;
	this->D = D;
	this->dt = dt;
	mainDC = dc;
	mainRect = rect;
	kineticRect = rect2;
	potentialRect = rect3;
	fullEnergyRect = rect4;
	vxGraphRect = rect5;
	vyGraphRect = rect6;
	TorX = T_or_X;
}
