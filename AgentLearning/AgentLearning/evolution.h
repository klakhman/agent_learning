#ifndef EVOLUTION_H_INCLUDED
#define EVOLUTION_H_INCLUDED
#include <stdio.h>

// ��������� ��������� ��������� ��������� ��� ������������������ ������������ ���������
void InitFirstGeneration_ModernEvolution(TAgentGenomePopulation* AgentPopulation, int AgentPopulationQuantity, int EnvironmentResolution, int OutputResolution, int InitialPoolCapacity);

// ��������� ��������� ��������� ��������� ��� NEAT
//void InitFirstGeneration_NEAT(TNetwork* AgentPopulation, int AgentPopulationQuantity, int EnvironmentResolution, int OutputResolution);

// ��������� ������ ��������� �������
void RecordAgents(TAgentGenomePopulation* AgentGenomePopulation, char* AgentFilename);

// ��������� ���������� ������ ����������� (�� ������ ����� ��������)
void PrepareOutputFiles(FILE** hResultFile, FILE** hHistogramFile, FILE** hBestAgentFile, TFilenameSettings* FilenameSettings,TRunSettings* RunSettings, TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TEnvironmentAims* EnvironmentAims, double EnVariableProbability, bool RewardMode);

// ��������� ���������� ������������� ����������� ������������� ������
//void GetRelativeHistogram(TPopulation* AgentPopulation, int* RelativeHistogram, int HistogramCount, double MaxReward);

// ��������� ������ ����������� ����� ��������
void ResultsOutput(TAgentGenomePopulation* AgentPopulation, FILE* hResultFile, FILE* hHistogramFile, FILE* hBestAgent, char* BestPopulationFilename, double* BestAverageReward, int EvolutionStep, bool RewardMode, int PenaltyRewardLimit);

void Evolution(TModeSettings* ModeSettings, TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TFilenameSettings* FilenameSettings, TRunSettings* RunSettings, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings, TLearningSettings* LearningSettings, double EnVariableProbability);

// ������� ��������� ���������� � ������� ��������
void PrepareEvolutionProcess(TRunSettings* RunSettings, TModeSettings* ModeSettings, TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TDirectorySettings* DirectorySettings, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings, TLearningSettings* LearningSettings, double EnVariableProbability);
#endif // EVOLUTION_H_INCLUDED
