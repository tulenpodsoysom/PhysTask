#pragma once
#include "afxwin.h"
#include <vector>
#include <string>
using namespace std;

class Simulation
{
	double V0;
	double angle;

	double mass;
	double c_d;
	double D;
	struct Projectile
	{
		double x, y;
		double vx, vy;
	};

	double dt;
	bool TorX;

	vector<Projectile> projectiles;	// параметры снаряда на каждый кадр симуляции

	CDC* mainDC;	// Девайс контекст для основного графического окна
	CRect mainRect;//
	CRect graphRect;//
	CRect kineticRect;
	CRect potentialRect;
	CRect fullEnergyRect;
	CRect vxGraphRect;
	CRect vyGraphRect;

	double Density(double h);
	/*Возращает модуль силы сопротивления*/
	double ForceResistance(double c_d, double density, double S, double Vx,double Vy);

	double FuncX(double vx, double v, double y);
	double FuncY(double vy, double v, double y);
	void DifferentialSolve();

public:
	void StartSimulation();
	void StopSimulation();
	void RestartSimulation();
	
	void MainGraph(CDC* dc, CRect rect);
	void MainGraphT(CDC* dc, CRect rect);
	void KineticGraph(CDC* dc, CRect rect);
	void PotentialGraph(CDC* dc, CRect rect);
	void FullEnergyGraph(CDC* dc, CRect rect);
	void VxGraph(CDC* dc, CRect rect);
	void VyGraph(CDC* dc, CRect rect);
	wstring SendOutput();
	wstring Result();

	void UpdateParams(double V0,double angle,double mass,double c_d,double D,double dt, CDC* dc, CRect rect, CRect rect2, CRect rect3, CRect rect4, CRect rect5, CRect rect6, bool T_or_X);
};

