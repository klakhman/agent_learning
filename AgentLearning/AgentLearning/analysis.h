#ifndef ANALYSIS_H_INCLUDED
#define ANALYSIS_H_INCLUDED

// Процедура прогона популяции
void PopulationRun(double* AverageReward, double* MaxReward, TAgentPopulation* AgentPopulation, TEnvironmentAims* EnvironmentAims, TTimeSettings* TimeSettings, TLearningSettings* LearningSettings, double EnVariableProb, bool NetworkMode);

// Процедра анализа показателей лучшей популяции агентов
void BestPopulationAnalysis(double* AverageReward, double* MaxReward, TRunSettings* RunSettings, TDirectorySettings* DirectorySettings, TTimeSettings* TimeSettings, TModeSettings* ModeSettings, TLearningSettings* LearningSettings, double EnVariableProb, int NoEnVariableCheck);

// Пропуск заголовка файла результатов
void SkipResultFileHead(FILE* hResultFile, int RewardMode);

// Процедура композиции общего отчета результатов эволюции по средам
void MakeFullResultFile(TGeneralRunSettings* GeneralRunSettings, TModeSettings* ModeSettings, TDirectorySettings* DirectorySettings, int EvolutionTime, double EnvironmentVariableProb);

// Проверка целостности данных перед анализом
void CheckIntegrityOfData(TGeneralRunSettings* GeneralRunSettings, TModeSettings* ModeSettings, TDirectorySettings* DirectorySettings, bool NoEnVariableCheck, int AgentPopulationQuantity);

// Проверка статистической значимости различия пар значений двух последовательностей в зависимости от параметра
void CheckExternalPairsStatSignificance(const char FirstAnalysisFilename[], const char SecondAnalysisFilename[], const char ResultAnalysisFilename[], const char CriterionFilename[], double SignificanceLevel, int PairQuantity, int MemberQuantity, int TryQuantity);

// Проверка статистической значимости динамики значений последовательности в зависимости от параметра
void CheckInternalStatSignificance(const char AnalysisFilename[], const char ResultAnalysisFilename[], const char CriterionFilename[], double SignificanceLevel, int ParameterQuantity, int MemberQuantity, int TryQuantity);

// Составление отчета о среднем количестве нейронов и связей в зависимости от сложности среды
//void MakeNetworkParametersSummary(int argc, char **argv);

void MakeEnvironmentBlockSummary(int argc, char** argv);

void TransEnv(int argc, char** argv);


#endif // ANALYSIS_H_INCLUDED
