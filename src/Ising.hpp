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
	int64_t n0; // ��� �� ����������.
	int64_t n; // ��� ��� ����������.
	int64_t MCS; // ��� = �����_�����^2.
	bool bSwapNeighbours = true;
	bool bTSingle = true; // ����� ��� ����� ����������� (�� ����� ������� ������� ����� ����������)
	void Metropolis(double T);
	//void MetropolisSingleStep(double T, int64_t& iter);
	void Reset();
	void Set(int size, double J, double posFrac, int n0, int n);
	int p(int i);	// ������������ ���������� ������ i ��� ��������� ������ � ��������� ������.

	std::vector<double> energyVals; // ������ ��� ���������� �������. (��� ���������� �������)
	std::vector<double> energyDuringEql; // ������ ��� �������� ������� �� ������ ��� �� ���������� ����������. (��� ������ ����������� ������� �� ����� ��������)
	std::vector<double> energy2Vals; // ������ ��� ���������� �������� �������. (��� ���������� �������� �������)

	double GetFullEnergy();
};

