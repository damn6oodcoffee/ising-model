#include "Ising.hpp"
#include <algorithm>
#include <random>

int Ising::p(int i) {
	if (i < 0) return i + cellLinSize;
	else if (i >= cellLinSize) return i - cellLinSize;
	else return i;
}

Ising::Ising() {

}

void Ising::Set(int size, double J, double posFrac, int n0, int n) {

	positiveSpinsFraction = posFrac;
	this->J = J;
	this->n0 = n0;
	this->n = n;
	tCritTheor = 2.269 * abs(J) / k;
	MCS = size * size;
	cellLinSize = size;

}
void Ising::Reset() {



	if (cellLinSize != theCell.size()) {
		theCell.clear();
		theCell.resize(cellLinSize);
		for (auto& arr : theCell) arr.resize(cellLinSize);
	}

	for (auto& arr : theCell) for (auto& el : arr) el = 0;

	int posSpinsCount = cellLinSize * cellLinSize * positiveSpinsFraction;
	int negSpinsCount = cellLinSize * cellLinSize - posSpinsCount;
	if (posSpinsCount >= negSpinsCount) {
		while (posSpinsCount > 0) {
			int i = (cellLinSize) * ((double)rand()) / RAND_MAX;
			int j = (cellLinSize) * ((double)rand()) / RAND_MAX;
			i = i == cellLinSize ? i - 1 : i;
			j = j == cellLinSize ? j - 1 : j;
			if (theCell[i][j] == 0) {
				theCell[i][j] = 1;
				posSpinsCount--;
			}
		}
		for (auto& arr : theCell) for (auto& el : arr) el = (el == 0 ? -1 : 1);
	}
	else {
		while (negSpinsCount > 0) {
			int i = (cellLinSize) * ((double)rand()) / RAND_MAX;
			int j = (cellLinSize) * ((double)rand()) / RAND_MAX;
			i = i == cellLinSize ? i - 1 : i;
			j = j == cellLinSize ? j - 1 : j;
			if (theCell[i][j] == 0) {
				theCell[i][j] = -1;
				negSpinsCount--;
			}
		}
		for (auto& arr : theCell) for (auto& el : arr) el = (el == 0 ? 1 : -1);
	}

}

void Ising::Metropolis(double T) {
	int i1, j1, i2, j2;
	double E2;
	double E1;
	double dE;
	i1 = j1 = i2 = j2 = -1;
	energyVals.clear();
	energyDuringEql.clear();
	energy2Vals.clear();
	double fullEnergy{ GetFullEnergy() };
	int neighbour{};
	std::random_device rd;  // a seed source for the random number engine
	std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distribIndex(0, cellLinSize - 1);
	std::uniform_real_distribution<> distribReal(0, 1);
	std::uniform_int_distribution<> distribNbr(0, 3);

	// ���� ���. ����������� ��� ������������ ����������.
	for (int64_t iter = 0; iter < MCS * n0; iter++) {

		if (bSwapNeighbours) {	// �������� �������� ��� �������.

			while (true) {
				if (iter % MCS == 0) energyDuringEql.push_back(fullEnergy);

				i1 = distribIndex(gen);
				j1 = distribIndex(gen);

				//i1 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//j1 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//i1 = i1 == cellLinSize ? i1 - 1 : i1;
				//j1 = j1 == cellLinSize ? j1 - 1 : j1;

				/*
				int neighbour = 4 * (((double)rand()) / (double)(RAND_MAX));

				switch (neighbour) {
				case 0:
					i2 = i1 + 1;
					j2 = j1;
					break;
				case 1:
					i2 = i1 - 1;
					j2 = j1;
					break;
				case 2:
					i2 = i1;
					j2 = j1 + 1;
					break;
				case 3:
					i2 = i1;
					j2 = j1 - 1;
					break;
				}


				if (theCell[p(i1)][p(j1)] != theCell[p(i2)][p(j2)]) break;
				iter++; // ��������� �������. ����������� ��� ���.
				*/
				bool visited_0{ false };
				bool visited_1{ false };
				bool visited_2{ false };
				bool visited_3{ false };

				while (true) {
					neighbour = distribNbr(gen);
					switch (neighbour) {
					case 0:
						if (visited_0) continue;
						else visited_0 = true;
						i2 = i1 + 1;
						j2 = j1;
						break;
					case 1:
						if (visited_1) continue;
						else visited_1 = true;
						i2 = i1 - 1;
						j2 = j1;
						break;
					case 2:
						if (visited_2) continue;
						else visited_2 = true;
						i2 = i1;
						j2 = j1 + 1;
						break;
					case 3:
						if (visited_3) continue;
						else visited_3 = true;
						i2 = i1;
						j2 = j1 - 1;
						break;
					}

					if (theCell[p(i1)][p(j1)] != theCell[p(i2)][p(j2)]) break;
					if (visited_0 && visited_1 && visited_2 && visited_3) break;
				}
				if (theCell[p(i1)][p(j1)] != theCell[p(i2)][p(j2)]) break;
				iter++;
			}
		}
		else {		// ������������ ����� ������.
			while (true) {
				if (iter % MCS == 0) energyDuringEql.push_back(fullEnergy);

				//i1 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//j1 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//i2 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//j2 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//
				//i1 = i1 == cellLinSize ? i1 - 1 : i1;
				//j1 = j1 == cellLinSize ? j1 - 1 : j1;
				//
				//i2 = i2 == cellLinSize ? i2 - 1 : i2;
				//j2 = j2 == cellLinSize ? j2 - 1 : j2;

				i1 = distribIndex(gen);
				j1 = distribIndex(gen);
				i2 = distribIndex(gen);
				j2 = distribIndex(gen);

				if (theCell[p(i1)][p(j1)] != theCell[p(i2)][p(j2)]) break;
				iter++; // ��������� �������. ����������� ��� ���.
			}
		}

		E1 = -J * (theCell[p(i1)][p(j1)] * theCell[p(i1 + 1)][p(j1)] + theCell[p(i1)][p(j1)] * theCell[p(i1 - 1)][p(j1)]
			+ theCell[p(i1)][p(j1)] * theCell[p(i1)][p(j1 + 1)] + theCell[p(i1)][p(j1)] * theCell[p(i1)][p(j1 - 1)]

			+ theCell[p(i2)][p(j2)] * theCell[p(i2 + 1)][p(j2)] + theCell[p(i2)][p(j2)] * theCell[p(i2 - 1)][p(j2)]
			+ theCell[p(i2)][p(j2)] * theCell[p(i2)][p(j2 + 1)] + theCell[p(i2)][p(j2)] * theCell[p(i2)][p(j2 - 1)]);

		std::swap(theCell[p(i1)][p(j1)], theCell[p(i2)][p(j2)]);

		E2 = -J * (theCell[p(i1)][p(j1)] * theCell[p(i1 + 1)][p(j1)] + theCell[p(i1)][p(j1)] * theCell[p(i1 - 1)][p(j1)]
			+ theCell[p(i1)][p(j1)] * theCell[p(i1)][p(j1 + 1)] + theCell[p(i1)][p(j1)] * theCell[p(i1)][p(j1 - 1)]

			+ theCell[p(i2)][p(j2)] * theCell[p(i2 + 1)][p(j2)] + theCell[p(i2)][p(j2)] * theCell[p(i2 - 1)][p(j2)]
			+ theCell[p(i2)][p(j2)] * theCell[p(i2)][p(j2 + 1)] + theCell[p(i2)][p(j2)] * theCell[p(i2)][p(j2 - 1)]);

		dE = E2 - E1;
		
		if (dE <= 0) {
			fullEnergy += dE;
			continue;
		}
		else {
			double R{ distribReal(gen) };
			
			if (R < exp(-dE / (k * T))) {
				fullEnergy += dE;
				continue;
			}
			else {
				std::swap(theCell[p(i1)][p(j1)], theCell[p(i2)][p(j2)]);
				continue;
			}
		}

	}

	if (bTSingle) return;
	// ���� ���. ����������� ��� ���������� �������.
	for (int64_t iter = 0; iter < MCS * n; iter++) {

		if (bSwapNeighbours) {	// �������� �������� ��� �������.
			while (true) {

				i1 = distribIndex(gen);
				j1 = distribIndex(gen);

				//i1 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//j1 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//i1 = i1 == cellLinSize ? i1 - 1 : i1;
				//j1 = j1 == cellLinSize ? j1 - 1 : j1;


				bool visited_0{ false };
				bool visited_1{ false };
				bool visited_2{ false };
				bool visited_3{ false };

				while (true) {
					neighbour = distribNbr(gen);
					switch (neighbour) {
					case 0:
						if (visited_0) continue;
						else visited_0 = true;
						i2 = i1 + 1;
						j2 = j1;
						break;
					case 1:
						if (visited_1) continue;
						else visited_1 = true;
						i2 = i1 - 1;
						j2 = j1;
						break;
					case 2:
						if (visited_2) continue;
						else visited_2 = true;
						i2 = i1;
						j2 = j1 + 1;
						break;
					case 3:
						if (visited_3) continue;
						else visited_3 = true;
						i2 = i1;
						j2 = j1 - 1;
						break;
					}

					if (theCell[p(i1)][p(j1)] != theCell[p(i2)][p(j2)]) break;
					if (visited_0 && visited_1 && visited_2 && visited_3) break;
				}
				if (theCell[p(i1)][p(j1)] != theCell[p(i2)][p(j2)]) break;
				iter++;
			}
		}
		else {		// ������������ ����� ������.
			while (true) {
				//i1 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//j1 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//i2 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//j2 = (cellLinSize) * ((double)rand()) / RAND_MAX;
				//
				//i1 = i1 == cellLinSize ? i1 - 1 : i1;
				//j1 = j1 == cellLinSize ? j1 - 1 : j1;
				//
				//i2 = i2 == cellLinSize ? i2 - 1 : i2;
				//j2 = j2 == cellLinSize ? j2 - 1 : j2;


				i1 = distribIndex(gen);
				j1 = distribIndex(gen);
				i2 = distribIndex(gen);
				j2 = distribIndex(gen);

				if (theCell[p(i1)][p(j1)] != theCell[p(i2)][p(j2)]) break;
				iter++; // ��������� �������. ����������� ��� ���.
			}
		}

		E1 = -J * (theCell[p(i1)][p(j1)] * theCell[p(i1 + 1)][p(j1)] + theCell[p(i1)][p(j1)] * theCell[p(i1 - 1)][p(j1)]
			+ theCell[p(i1)][p(j1)] * theCell[p(i1)][p(j1 + 1)] + theCell[p(i1)][p(j1)] * theCell[p(i1)][p(j1 - 1)]

			+ theCell[p(i2)][p(j2)] * theCell[p(i2 + 1)][p(j2)] + theCell[p(i2)][p(j2)] * theCell[p(i2 - 1)][p(j2)]
			+ theCell[p(i2)][p(j2)] * theCell[p(i2)][p(j2 + 1)] + theCell[p(i2)][p(j2)] * theCell[p(i2)][p(j2 - 1)]);

		std::swap(theCell[p(i1)][p(j1)], theCell[p(i2)][p(j2)]);
	

		E2 = -J * (theCell[p(i1)][p(j1)] * theCell[p(i1 + 1)][p(j1)] + theCell[p(i1)][p(j1)] * theCell[p(i1 - 1)][p(j1)]
			+ theCell[p(i1)][p(j1)] * theCell[p(i1)][p(j1 + 1)] + theCell[p(i1)][p(j1)] * theCell[p(i1)][p(j1 - 1)]

			+ theCell[p(i2)][p(j2)] * theCell[p(i2 + 1)][p(j2)] + theCell[p(i2)][p(j2)] * theCell[p(i2 - 1)][p(j2)]
			+ theCell[p(i2)][p(j2)] * theCell[p(i2)][p(j2 + 1)] + theCell[p(i2)][p(j2)] * theCell[p(i2)][p(j2 - 1)]);

		dE = E2 - E1;
		
		if (dE <= 0) {
			// ���������� ������� ������ ���������
			fullEnergy += dE;
			energyVals.push_back(fullEnergy);
			energy2Vals.push_back(fullEnergy * fullEnergy);
			continue;
		}
		else {
			double R = (((double)rand()) / RAND_MAX);
		
			if (R < exp(-dE / (k * T))) {
				// ���������� ������� ������ ���������
				fullEnergy += dE;
				energyVals.push_back(fullEnergy);
				energy2Vals.push_back(fullEnergy * fullEnergy);
				continue;
			}
			else {
				std::swap(theCell[p(i1)][p(j1)], theCell[p(i2)][p(j2)]);
			
				continue;
			}
		}

	}
}

double Ising::GetFullEnergy() {
	double E = 0;
	for (int i = 0; i < theCell[0].size(); i++) {
		for (int j = 0; j < theCell.size(); j++) {
			E += theCell[p(j)][p(i)] * theCell[p(j)][p(i - 1)] + theCell[p(j)][p(i)] * theCell[p(j - 1)][p(i)];
		}
	}

	E *= -J;
	return E;
}