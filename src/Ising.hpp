#pragma once
#include <vector>

class Ising
{
public:
	Ising();
	std::vector<std::vector<int>> theCell;
	int cellLinSize;
	double positiveSpinsFraction;
	double J;
	double tCritTheor;
	double k = 1.380649 * 1e-23;
	int64_t n0; // МКШ до равновесия.
	int64_t n; // МКШ для усреднения.
	int64_t MCS; // МКШ = длина_сетки^2.
	bool bSwapNeighbours = true;
	bool bTSingle = true; // Режим для одной температуры (не нужно считать средние после равновесия)
	void Metropolis(double T);
	//void MetropolisSingleStep(double T, int64_t& iter);
	void Reset();
	void Set(int size, double J, double posFrac, int n0, int n);
	int p(int i);	// Периодически возвращает индекс i вне расчетной ячейки в расчетную ячейку.

	std::vector<double> energyVals; // Массив для накопления энергии. (для усреднения энергии)
	std::vector<double> energyDuringEql; // Массив для хранения энергии на каждом МКШ до достижения равновесия. (для вывода зависимости энергии от числа итераций)
	std::vector<double> energy2Vals; // Массив для накопления квадрата энергии. (для усреднения квадрата энергии)

	double GetFullEnergy();
};

