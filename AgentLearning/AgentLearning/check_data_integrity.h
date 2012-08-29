#ifndef CHECK_DATA_INTEGRITY_H_INCLUDED
#define CHECK_DATA_INTEGRITY_H_INCLUDED

// ����������� ��������� �������� ����������� ���������� ���� ������

//��������� �������� ����������� ��������� ������ ���� ����� � ����� (���������� 0 - ���� ��� ���������, 1 - ���� ���� ������ - � �������� �� ������� ������)
bool check_integrity_of_pool_network_record(FILE* hNetworkFile, int EnvironmentResolution, int OutputResolution, int NetworkMode);

//��������� �������� ����������� ����� � �������� ���� �� ����� (���������� 0 - ���� ��� ���������, 1 - ���� ���� ������ - � �������� �� ������� ������)
bool check_integrity_of_pool_network_file(char* NetworkFilename, int NetworksQuantity, int EnvironmentResolution, int OutputResolution, int NetworkMode);

// �������� ����������� ������ ����� �������� ����� ������� ������ ���������
void check_integrity_of_best_populations_data(TGeneralRunSettings* GeneralRunSettings, TModeSettings* ModeSettings, TDirectorySettings* DirectorySettings, bool NoEnVariableCheck, int AgentPopulationQuantity);

#endif // CHECK_DATA_INTEGRITY_H_INCLUDED
