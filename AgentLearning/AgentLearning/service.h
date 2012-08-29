#ifndef SERVICE_H_INCLUDED
#define SERVICE_H_INCLUDED
#include "types.h"
#include <math.h>

inline double pi() { return 3.1415926535F; }

// Функция максимума (целочисленная)
inline int max(int a, int b) { return ((a < b) ? b : a); }

// Функция максимума (дробная)
inline double max(double a, double b) { return ((a < b) ? b : a); }

//Функция минимума (дробная)
inline double min(double a, double b) { return ((a > b) ? b : a); }

// Функция минимума (целочисленная)
inline int min(int a, int b) { return ((a > b) ? b : a); }

//Функция модуля (целочисленная)
inline int abs(int a) { return ((a < 0) ? -a : a); }

//Функция модуля (дробная)
//inline double abs(double a) { return ((a < 0) ? -a : a);}

// Логистическая функция (усеченная, всегда положительная)
inline double tansig(double x) { return ( 1/ (1 + exp(-2*x)) ); }

// Стандартная логистическая функция
inline double tansig_standart(double x) { return (2/(1+exp(-2*x)) - 1); }

// Процедура перевода десятичного числа в двоичное
void DecToBin(int DecNumber, int* BinNumber, int BinResolution);

// Процедура перевода двоичного числа в десятичное
int BinToDec(int* BinNumber, int BinResolution);

// Процедура инвертирования двоичного числа
void InvertBin(int* BinNumber, int BinResolution);

// Процедура конвертирования десятичного числа в шестнадцатеричное (символьное, так что в старших разрядах - в начале массива - могут быть нули)
void DecToHex(int DecNumber, char* HexNumber, int HexResolution);

// Сортировка методом пузырька
void BubbleSort(int* NumbersArray, int NumbersQuantity);

// Сортировка методом пузырька (дробная)
void BubbleSortdouble(double* NumbersArray, int NumbersQuantity);

// Функция генерации псеводослучайного равномерно распределенного числа (куски интервалов не включаются, только если указано)
double UniformDistribution(double A, double B, bool include_start = true, bool include_end = true);

// Функция генерации псевдослучайного равномерно распределенного дискретной величины (куски интервалов включаются)
int UniformDiscreteDistribution(int A, int B);

// Функция генерация псевдослучайного нормально распределенного числа
double NormalDistribution(double Mean, double Variance);

// Функция генерации неповторяющейся последовательности случайных чисел в заданном дипазоне
void GenerateRandomNumbers(int FirstNumber, int LastNumber, int Quantity, int* NumbersSequence);

// Получение полного пути к файлу среды
void GetEnvironmentFilename(char* EnvironmentFilename, char* EnvironmentDirectory, int EnvironmentNumber);

// Получение полного пути к файлу результатов
void GetResultsFilename(char* ResultsFilename, TRunSettings* RunSettings, TModeSettings* ModeSettings, char* ResultsDirectory, bool EnvironmentStability);

// Получение полного пути к файлу с гистограммами
void GetHistogramFilename(char* HistogramFilename, TRunSettings* RunSettings, TModeSettings* ModeSettings, char* ResultsDirectory, bool EnvironmentStability);

// Получение полного пути к файлу лучшей популяции
void GetBestPopulationFilename(char* BestPopulationFilename, TRunSettings* RunSettings, TModeSettings* ModeSettings, char* ResultsDirectory, bool EnvironmentStability);

// Получение полного пути к файлу с лучшими агентами в популяциях
void GetBestAgentFilename(char* BestAgentFilename, TRunSettings* RunSettings, TModeSettings* ModeSettings, char* ResultsDirectory, bool EnvironmentStability);

// Получение полного пути к файлу общего отчета по средам
void GetFullResultFilename(char* ResultFullFilename, TRunSettings* RunSettings, int FirstTry, int LastTry, TModeSettings* ModeSettings, char* ResultsDirectory, bool EnvironmentStability);

#endif // SERVICE_H_INCLUDED
