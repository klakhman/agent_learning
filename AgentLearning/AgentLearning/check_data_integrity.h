#ifndef CHECK_DATA_INTEGRITY_H_INCLUDED
#define CHECK_DATA_INTEGRITY_H_INCLUDED

// “ехнические процедуры проверки целостности различного рода данных

//ѕроцедура проверки целостности следующей записи сети пулов в файле (возвращает 0 - если все нормально, 1 - если есть ошибка - в основном не хватает данных)
bool check_integrity_of_pool_network_record(FILE* hNetworkFile, int EnvironmentResolution, int OutputResolution, int NetworkMode);

//ѕроцедура проверки целостности файла с запис€ми сети из пулов (возвращает 0 - если все нормально, 1 - если есть ошибка - в основном не хватает данных)
bool check_integrity_of_pool_network_file(char* NetworkFilename, int NetworksQuantity, int EnvironmentResolution, int OutputResolution, int NetworkMode);

// ѕроверка целостности данных перед анализом путем прогона лучших попул€ций
void check_integrity_of_best_populations_data(TGeneralRunSettings* GeneralRunSettings, TModeSettings* ModeSettings, TDirectorySettings* DirectorySettings, bool NoEnVariableCheck, int AgentPopulationQuantity);

#endif // CHECK_DATA_INTEGRITY_H_INCLUDED
