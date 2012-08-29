#ifndef EVOLUTION_H_INCLUDED
#define EVOLUTION_H_INCLUDED
#include <stdio.h>

// Процедура генерации стартовой популяции для модернизированного эвлюционного алгоритма
void InitFirstGeneration_ModernEvolution(TAgentGenomePopulation* AgentPopulation, int AgentPopulationQuantity, int EnvironmentResolution, int OutputResolution, int InitialPoolCapacity);

// Процедура генерации стартовой популяции для NEAT
//void InitFirstGeneration_NEAT(TNetwork* AgentPopulation, int AgentPopulationQuantity, int EnvironmentResolution, int OutputResolution);

// Процедура записи популяции агентов
void RecordAgents(TAgentGenomePopulation* AgentGenomePopulation, char* AgentFilename);

// Процедура подготовки файлов результатов (до начала самой эволюции)
void PrepareOutputFiles(FILE** hResultFile, FILE** hHistogramFile, FILE** hBestAgentFile, TFilenameSettings* FilenameSettings,TRunSettings* RunSettings, TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TEnvironmentAims* EnvironmentAims, double EnVariableProbability, bool RewardMode);

// Процедура построения относительной гистограммы расрпеделения наград
//void GetRelativeHistogram(TPopulation* AgentPopulation, int* RelativeHistogram, int HistogramCount, double MaxReward);

// Процедура вывода результатов такта эволюции
void ResultsOutput(TAgentGenomePopulation* AgentPopulation, FILE* hResultFile, FILE* hHistogramFile, FILE* hBestAgent, char* BestPopulationFilename, double* BestAverageReward, int EvolutionStep, bool RewardMode, int PenaltyRewardLimit);

void Evolution(TModeSettings* ModeSettings, TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TFilenameSettings* FilenameSettings, TRunSettings* RunSettings, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings, TLearningSettings* LearningSettings, double EnVariableProbability);

// Рабочая процедура подготовки к запуску эволюции
void PrepareEvolutionProcess(TRunSettings* RunSettings, TModeSettings* ModeSettings, TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TDirectorySettings* DirectorySettings, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings, TLearningSettings* LearningSettings, double EnVariableProbability);
#endif // EVOLUTION_H_INCLUDED
