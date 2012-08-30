#ifndef PRIMARY_SYSTEMOGENESIS_H_INCLUDED
#define PRIMARY_SYSTEMOGENESIS_H_INCLUDED

// ���������� ������ (��� ������) ��������� ��������� ���� �� �������� (��������� �����)
void BuildPrimaryNetwork(TNeuralNetwork* PrimaryNetwork, TPoolNetwork* PoolNetwork);

// ������� ������ �������������� ��������
void NeuronsSelection(TNeuralNetwork* PrimaryNetwork, double* InitialSummaryPotential, int ActivationNeuronsQuantity);

// ������� ������ �������� �������� ��������
void SynapsesSelection(TNeuralNetwork* PrimaryNetwork, double* SummaryPotential, double SynapsesActivityTreshold);

// ������� ������ ������������ ������
void PredConnectionSelection(TNeuralNetwork* PrimaryNetwork, double* PredictorSignificance, double SignificanceTreshold);

// ��������� ���������� ���������� ������������� ������
void AgentPrimarySystemogenesis(TNeuralNetwork* PrimaryNetwork, TPoolNetwork* PoolNetwork, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings);

// ��������� ��������� �������������, ������� ����������� ���� �� ����� � ���� �� ��������, ��� ����� ��� ���� � ���� (�������� ��������� �����, � �� �����)
void AgentLinearPrimarySystemogenesis(TNeuralNetwork* PrimaryNetwork, TPoolNetwork* PoolNetwork);

#endif // PRIMARY_SYSTEMOGENESIS_H_INCLUDED
