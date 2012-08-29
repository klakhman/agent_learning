#ifndef PARALLEL_H_INCLUDED
#define PARALLEL_H_INCLUDED

#include "types.h"

// Процедура заполнения параметров из файла настроек
void FillSettingsFromFile(const char SettingsFilename[], TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TModeSettings* ModeSettings, TDirectorySettings* DirectorySettings, TGeneralRunSettings* GeneralRunSettings, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings, TLearningSettings* LearningSettings, double* EnVariablePorbability);

//Процедура заполнения параметров из командной строки
void FillCommandParametrs(int argc, char **argv, TGeneralRunSettings* GeneralRunSettings, TModeSettings* ModeSettings, double* EnSynProb, double* DisSynProb, double* EnVariableProbability, int* ProgramMode, bool* NoEnVariableCheck);

// Процедура декодирования сообщения с заданием для рабочего процесса
void DecodeTaskMessage(char* TaskMessage, TRunSettings* RunSettings, TModeSettings* ModeSettings, double* DisSynProb, double* EnSynProb, double* EnVariablePorbability, bool* NoEnVariableCheck);

// Основная процедура распараллеливания - выдача и выполнение заданий
void ParallelTasks(int argc, char **argv);

// Основная процедура распараллеливания анализа эволюции
void ParallelAnalysis(int argc, char **argv);


#endif // PARALLEL_H_INCLUDED
