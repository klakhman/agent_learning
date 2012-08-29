#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "types.h"

#include "service.h"

// Процедура перевода десятичного числа в двоичное
void DecToBin(int DecNumber, int* BinNumber, int BinResolution)
{
   /*for (int CurrentPosition = BinResolution; CurrentPosition>0; --CurrentPosition)
   {
      if (DecNumber >= (int) trunc(exp((CurrentPosition-1)*log(2))+0.5))
      {
         BinNumber[BinResolution - CurrentPosition] = 1;
         DecNumber -= (int) trunc(exp((CurrentPosition-1)*log(2))+0.5);
      }
      else
         BinNumber[BinResolution - CurrentPosition] = 0;
   }*/
   //Algorithm from "Thinking in C++" by Bruce Eckel (vol.1 p.127 in russian edition)
   for (int CurrentPosition = BinResolution; CurrentPosition>0; --CurrentPosition)
      BinNumber[BinResolution-CurrentPosition] = (DecNumber & (1 << (CurrentPosition-1))) ? 1 : 0;
}

// Процедура перевода двоичного числа в десятичное
int BinToDec(int* BinNumber, int BinResolution)
{
   int _DecNumber = 0;
   for (int CurrentPosition = 0; CurrentPosition<BinResolution; ++CurrentPosition)
      _DecNumber += static_cast<int>(pow(2.0F, BinResolution - CurrentPosition - 1)) * BinNumber[CurrentPosition];

   return _DecNumber;
}

// Процедура инвертирования двоичного числа
void InvertBin(int* BinNumber, int BinResolution)
{
   int temp;
   for (int Position=0; Position<BinResolution/2; ++Position)
   {
      temp = BinNumber[Position];
      BinNumber[Position] = BinNumber[BinResolution-Position-1];
      BinNumber[BinResolution-Position-1] = temp;
   }
}

// Процедура конвертирования десятичного числа в шестнадцатеричное (символьное, так что в старших разрядах - в начале массива - могут быть нули)
void DecToHex(int DecNumber, char* HexNumber, int HexResolution)
{
   sprintf(HexNumber, "");
   for (int CurrentPosition=0; CurrentPosition<HexResolution; ++CurrentPosition)
   {
      if ((int) (DecNumber / static_cast<int>(pow(16.0f,HexResolution-CurrentPosition-1))) > 9) // Если это символ
         sprintf(HexNumber,"%s%c",HexNumber, 'A' + ((int) (DecNumber / static_cast<int>(pow(16.0F,HexResolution-CurrentPosition-1))) - 10));
         //HexNumber[CurrentPosition] = 'A' + ((int) (DecNumber / pow(16,HexResolution-CurrentPosition-1)) - 10);
      else
         sprintf(HexNumber,"%s%c",HexNumber, '0' + (int) (DecNumber / static_cast<int>(pow(16.0F,HexResolution-CurrentPosition-1))));
         //HexNumber[CurrentPosition] = '0' + (int) (DecNumber / pow(16,HexResolution-CurrentPosition-1));
      DecNumber = DecNumber % 16;
   }
}

// Сортировка методом пузырька
void BubbleSort(int* NumbersArray, int NumbersQuantity)
{
   for (int i=0; i<NumbersQuantity; i++)
      for (int j=0; j<NumbersQuantity-i-1; j++)
         if (NumbersArray[j] > NumbersArray[j+1])
         {
            NumbersArray[j] = NumbersArray[j] + NumbersArray[j+1];
            NumbersArray[j+1] = NumbersArray[j] - NumbersArray[j+1];
            NumbersArray[j] = NumbersArray[j] - NumbersArray[j+1];
         }
}

// Сортировка методом пузырька (дробная)
void BubbleSortdouble(double* NumbersArray, int NumbersQuantity)
{
   for (int i=0; i<NumbersQuantity; i++)
      for (int j=0; j<NumbersQuantity-i-1; j++)
         if (NumbersArray[j] > NumbersArray[j+1])
         {
            NumbersArray[j] = NumbersArray[j] + NumbersArray[j+1];
            NumbersArray[j+1] = NumbersArray[j] - NumbersArray[j+1];
            NumbersArray[j] = NumbersArray[j] - NumbersArray[j+1];
         }
}

// Функция генерации неповторяющейся последовательности случайных чисел в заданном дипазоне
void GenerateRandomNumbers(int FirstNumber, int LastNumber, int Quantity, int* NumbersSequence)
{
   for (int i=0; i<Quantity; i++)
   {
      int CurrentNumber; // Текущее сгенерированное число
      bool Check; // Признак того, что число уже есть в последовательности
      do
      {
         CurrentNumber = UniformDiscreteDistribution(FirstNumber, LastNumber);

         Check = false;
         int CountCheck = 0; // Счетчик номера текущего номера в текущей последовательности
         while ((!Check)&&(CountCheck<i)) // Пока мы не нашли число или не прошли всю текущую последовательность
         {
            if (NumbersSequence[CountCheck] == CurrentNumber) Check = true;
            CountCheck++;
         }
      } while (Check);
      NumbersSequence[i] =  CurrentNumber;
   }
}

// Функция генерация псевдослучайного нормально распределенного числа
double NormalDistribution(double Mean, double Variance)
{
   /* Генерация с использованием центральной предельной теоремы
   int RandomNumbers = 12; // Количество случайных чисел, которое используется для генерации одного нормально распределенного
   double RandomSum = 0.0; // Сумма последовательности случайных чисел
   for (int i=0; i<RandomNumbers; i++)
      RandomSum += rand()%10001 / 10000.0;

   return Variance * (RandomSum - 6) + Mean; // Variance * sqrt(12.0/RandomNumbers) * (RandomSum - RandomNumbers/6.0) + Mean;*/

   /* Генерация методом Бокса-Мюллера */
   return Variance*sqrt(-2*log(UniformDistribution(0, 1, false)))*cos(2*pi()*UniformDistribution(0, 1, false)) + Mean;
}

// Функция генерации псеводослучайного равномерно распределенного числа (сами куски интервала включаются, если не указано иного)
double UniformDistribution(double A, double B, bool include_start/* = true*/, bool include_end /*= true*/)
{
   //return ((rand()%999 + 1)/1000.0) * (B - A) + A; // куски интервала в этой версии не включаются
   const int scale = 10000;
   // Модифицируем интервал, чтобы удовлетворить условиям включения или не включения концов
   A += (!include_start) * 1.0F/static_cast<double>(scale);
   B -= (!include_end) * 1.0F/static_cast<double>(scale);
   return (UniformDiscreteDistribution(0, scale)/static_cast<double>(scale)) * (B - A) + A;
}

// Функция генерации псевдослучайного равномерно распределенного дискретной величины (куски интервалов включаются)
int UniformDiscreteDistribution(int A, int B)
{
   //return (int) (A +  rand() * ((double) B - A+1)/(RAND_MAX+1));
   // Вариант из книги Кенига-Му (Accelerated C++)
   int range = B - A + 1;
   const int bucket_size = RAND_MAX / range;
   int r;

   do r = rand() / bucket_size;
   while (r>=range);

   return A + r;
}

// Получение полного пути к файлу среды
void GetEnvironmentFilename(char* EnvironmentFilename, char* EnvironmentDirectory, int EnvironmentNumber)
{
   sprintf(EnvironmentFilename,"%s/Environment%i.txt", EnvironmentDirectory, EnvironmentNumber);
}

// Получение полного пути к файлу результатов
void GetResultsFilename(char* ResultsFilename, TRunSettings* RunSettings, TModeSettings* ModeSettings, char* ResultsDirectory, bool EnvironmentStability)
{
   char tmp_str[255];
   sprintf(tmp_str,"%s/En%i/", ResultsDirectory, RunSettings->EnvironmentNumber);
   if (ModeSettings->RewardMode) sprintf(tmp_str,"%sEvolutionWithPenalty/",tmp_str);
   sprintf(tmp_str,"%sResult_En%i",tmp_str,RunSettings->EnvironmentNumber);
   if (EnvironmentStability) sprintf(tmp_str,"%s_noenvar",tmp_str);
   sprintf(tmp_str,"%s_%i(%i)",tmp_str,RunSettings->VariableNumber,RunSettings->TryNumber);
   if (ModeSettings->RewardMode) sprintf(tmp_str,"%s_pen",tmp_str);
   if (ModeSettings->EvolutionMode) sprintf(tmp_str,"%s_mod",tmp_str);
   sprintf(tmp_str,"%s.txt",tmp_str);

   sprintf(ResultsFilename,"%s",tmp_str);
}

// Получение полного пути к файлу с гистограммами
void GetHistogramFilename(char* HistogramFilename, TRunSettings* RunSettings, TModeSettings* ModeSettings, char* ResultsDirectory, bool EnvironmentStability)
{
   char tmp_str[255];
   sprintf(tmp_str,"%s/En%i/", ResultsDirectory, RunSettings->EnvironmentNumber);
   if (ModeSettings->RewardMode) sprintf(tmp_str,"%sEvolutionWithPenalty/",tmp_str);
   sprintf(tmp_str,"%sHistogram_En%i",tmp_str,RunSettings->EnvironmentNumber);
   if (EnvironmentStability) sprintf(tmp_str,"%s_noenvar",tmp_str);
   sprintf(tmp_str,"%s_%i(%i)",tmp_str,RunSettings->VariableNumber,RunSettings->TryNumber);
   if (ModeSettings->RewardMode) sprintf(tmp_str,"%s_pen",tmp_str);
   if (ModeSettings->EvolutionMode) sprintf(tmp_str,"%s_mod",tmp_str);
   sprintf(tmp_str,"%s.txt",tmp_str);

   sprintf(HistogramFilename,"%s",tmp_str);
}

// Получение полного пути к файлу лучшей популяции
void GetBestPopulationFilename(char* BestPopulationFilename, TRunSettings* RunSettings, TModeSettings* ModeSettings, char* ResultsDirectory, bool EnvironmentStability)
{
   char tmp_str[255];
   sprintf(tmp_str,"%sEn%i/", ResultsDirectory, RunSettings->EnvironmentNumber);
   if (ModeSettings->RewardMode) sprintf(tmp_str,"%sEvolutionWithPenalty/",tmp_str);
   sprintf(tmp_str,"%sEn%i",tmp_str,RunSettings->EnvironmentNumber);
   if (EnvironmentStability) sprintf(tmp_str,"%s_noenvar",tmp_str);
   sprintf(tmp_str,"%s_%i(%i)_maxaverage",tmp_str,RunSettings->VariableNumber,RunSettings->TryNumber);
   if (ModeSettings->RewardMode) sprintf(tmp_str,"%s_pen",tmp_str);
   if (ModeSettings->EvolutionMode) sprintf(tmp_str,"%s_mod",tmp_str);
   sprintf(tmp_str,"%s.txt",tmp_str);

   sprintf(BestPopulationFilename,"%s",tmp_str);
}

// Получение полного пути к файлу с лучшими агентами в популяциях
void GetBestAgentFilename(char* BestAgentFilename, TRunSettings* RunSettings, TModeSettings* ModeSettings, char* ResultsDirectory, bool EnvironmentStability)
{
   char tmp_str[255];
   sprintf(tmp_str,"%s/En%i/", ResultsDirectory, RunSettings->EnvironmentNumber);
   if (ModeSettings->RewardMode) sprintf(tmp_str,"%sEvolutionWithPenalty/",tmp_str);
   sprintf(tmp_str,"%sEn%i",tmp_str,RunSettings->EnvironmentNumber);
   if (EnvironmentStability) sprintf(tmp_str,"%s_noenvar",tmp_str);
   sprintf(tmp_str,"%s_%i(%i)_bestagents",tmp_str,RunSettings->VariableNumber,RunSettings->TryNumber);
   if (ModeSettings->RewardMode) sprintf(tmp_str,"%s_pen",tmp_str);
   if (ModeSettings->EvolutionMode) sprintf(tmp_str,"%s_mod",tmp_str);
   sprintf(tmp_str,"%s.txt",tmp_str);

   sprintf(BestAgentFilename,"%s",tmp_str);
}

// Получение полного пути к файлу общего отчета по средам
void GetFullResultFilename(char* ResultFullFilename, TRunSettings* RunSettings, int FirstTry, int LastTry, TModeSettings* ModeSettings, char* ResultsDirectory, bool EnvironmentStability)
{
   char tmp_str[255];
   sprintf(tmp_str,"%s/En%i/", ResultsDirectory, RunSettings->EnvironmentNumber);
   if (ModeSettings->RewardMode) sprintf(tmp_str,"%sEvolutionWithPenalty/",tmp_str);
   sprintf(tmp_str,"%sFullResult(%i-%i)_En%i",tmp_str, FirstTry, LastTry, RunSettings->EnvironmentNumber);
   if (EnvironmentStability) sprintf(tmp_str,"%s_noenvar",tmp_str);
   sprintf(tmp_str,"%s_%i",tmp_str,RunSettings->VariableNumber);
   if (ModeSettings->RewardMode) sprintf(tmp_str,"%s_pen",tmp_str);
   if (ModeSettings->EvolutionMode) sprintf(tmp_str,"%s_mod",tmp_str);
   sprintf(tmp_str,"%s.txt",tmp_str);

   sprintf(ResultFullFilename,"%s",tmp_str);
}
