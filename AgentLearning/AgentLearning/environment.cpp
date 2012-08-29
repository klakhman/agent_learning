#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "types.h"
#include "service.h"

#include <time.h>

#include "environment.h"

// Процедура загрузки среды из файла
void InitEnvironment(TEnvironmentAims* EnvironmentAims, char* EnvironmentFilename)
{
   FILE* f_EnvironmentFile = fopen(EnvironmentFilename,"r"); // Открываем файл среды
   char StrFromFile[255];

   fscanf(f_EnvironmentFile, "%s", StrFromFile); // Считываем размерность среды
   EnvironmentAims->EnvironmentResolution = atoi(StrFromFile);

   //Определение размерности выходного вектора
   int OutResolution=-1;
   double ActionQuantity; // Кол-во доступных для кодирования действий при определенном кол-ве выходных нейронов
   do
   {
      OutResolution++;
      ActionQuantity = 1;
      // Подсчитываем кол-во комбинаций (выборку)
      for (int i=0; i<OutResolution; i++)
         ActionQuantity = ActionQuantity * (i+1);
      for (int i=0; i<OutResolution - 2; i++)
         ActionQuantity = ActionQuantity / (i+1);
      ActionQuantity = ActionQuantity / 2;
      //----------------------------------------
   } while (ActionQuantity < (2 * EnvironmentAims->EnvironmentResolution)); // Пока количество действий не будет равно или больше требуемому
   EnvironmentAims->OutputResolution = OutResolution;

   fscanf(f_EnvironmentFile, "%s", StrFromFile); // Считываем кол-во целей
   EnvironmentAims->AimQuantity=atoi(StrFromFile);

   for (int CurrentAim=0; CurrentAim<EnvironmentAims->AimQuantity; CurrentAim++) // Считываем структуру целей
   {
      fscanf(f_EnvironmentFile, "%s", StrFromFile); // Считываем сложность среды
      EnvironmentAims->Aim[CurrentAim].Complexity=atoi(StrFromFile);
      fscanf(f_EnvironmentFile, "%s", StrFromFile); // Считываем награду за цель
      EnvironmentAims->Aim[CurrentAim].Reward=atof(StrFromFile);
      EnvironmentAims->Aim[CurrentAim].VisitTime = 0;

      for (int CurrentBit=0; CurrentBit<EnvironmentAims->Aim[CurrentAim].Complexity; CurrentBit++) // Считываем все биты цели
      {
         fscanf(f_EnvironmentFile, "%s", StrFromFile);
         EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Number = atoi(StrFromFile);
         fscanf(f_EnvironmentFile, "%s", StrFromFile);
         EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Value = atoi(StrFromFile);
      }
    }

    fclose(f_EnvironmentFile);
}

//Процедура перезапуска среды
void ResetEnviroment(TEnvironmentAims* EnvironmentAims)
{
   for (int i=0; i<EnvironmentAims->AimQuantity; i++)
      EnvironmentAims->Aim[i].VisitTime = -100;
}

// Процедура вычисления коэффициента заполненности среды
double CoefFullEnvironment(TEnvironmentAims* EnvironmentAims)
{
   //bool k_en[EnvironmentAims->EnvironmentResolution]; // Массив вхождений какого-либо бита в цель
   bool* k_en = new bool[EnvironmentAims->EnvironmentResolution];
   double CoefFull=0.0; // Коэффициент заполненности среды

   for (int CurrentAim=0; CurrentAim<EnvironmentAims->AimQuantity; CurrentAim++)
   {
        for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++) k_en[i] = false; // Очищаем массив вхождений

        for (int CurrentBit=0; CurrentBit<EnvironmentAims->Aim[CurrentAim].Complexity; CurrentBit++) // Определение битов, входящих в цель (заполнение массива вхождений)
            k_en[EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Number] = true;

        double CoefFullAim = 1; // Сложность цели
        int k_enc = 0; // Количество различных битов входящих в цель

        // Вычисление сложности цели
        for (int k=0; k<EnvironmentAims->Aim[CurrentAim].Complexity; k++) CoefFullAim = CoefFullAim * ( 1.0 / (2.0F * EnvironmentAims->EnvironmentResolution)); // Подсчет вероятности совершения k последовательных действий
        for (int k=0; k<EnvironmentAims->EnvironmentResolution; k++) k_enc += k_en[k]; // Подсчет кол-ва битов входящих в цель
        for (int k=0; k<(EnvironmentAims->Aim[CurrentAim].Complexity - k_enc); k++) CoefFullAim=CoefFullAim*2; // Подсчет вероятности нахождения битов в изначальных правильных состояний
        //----------------------
        CoefFull+=CoefFullAim;
    }
    delete []k_en;

    return CoefFull;
}

// Сравнение двух полных (!!!) целей разной длины
bool CompareDifLentghAim(TAim* FirstAim, TAim* SecondAim)
{
   // Основная цель данной процедуры - понять равны ли минимальные подцели двух целей
   bool CheckStatus = true;
   // Если длина минимальной подцели у первой цели больше
   if (FirstAim->VisitTime > SecondAim->VisitTime)
      // Если длина минимальной подцели первой цели больше, чем общая длина второй цели, то они не могут быть идентичны
      if (FirstAim->VisitTime > SecondAim->Complexity)
         CheckStatus = false;
      else
      {
         // Проходимся по всем битам до минимальной длины первой цели
         for (int i=0; i<FirstAim->VisitTime; i++)
            // Если хотя бы одно действие неравно, то цели неравны
            if ( (FirstAim->AimStructure[i].Value != SecondAim->AimStructure[i].Value) || (FirstAim->AimStructure[i].Number != SecondAim->AimStructure[i].Number) )
               CheckStatus = false;
      }
   else
      if (SecondAim->VisitTime > FirstAim->Complexity) //!! Было исправлено FirstAim->VisitTime на FirstAim->Complexity
         CheckStatus=false;
      else
      {
         for (int i=0; i<SecondAim->VisitTime; i++)
            if ( (FirstAim->AimStructure[i].Value != SecondAim->AimStructure[i].Value) || (FirstAim->AimStructure[i].Number != SecondAim->AimStructure[i].Number) )
               CheckStatus = false;
      }
   return CheckStatus;

}

// Процедура генерации срелы
double GenerateEnvironment(TEnvironmentAims* EnvironmentAims, int EnvironmentResolution, double RequiredEnvCoefFull, double Eps, int MaxComplexity, int MinMaxComplexity, int MinComplexity)
{
   int CoefOK = 0; // Переменная признак того, что реальный коэффициент заполненности в пределах заданной точности
   double RealEnvCoefFull;
   //int ReplyOK = 0;
   // Записываем размерности среды
   EnvironmentAims->EnvironmentResolution = EnvironmentResolution;
   //Определение размерности выходного вектора
   int OutResolution=-1;
   double ActionQuantity; // Кол-во доступных для кодирования действий при определенном кол-ве выходных нейронов
   do
   {
      OutResolution++;
      ActionQuantity = 1;
      // Подсчитываем кол-во комбинаций (выборку)
      for (int i=0; i<OutResolution; i++)
         ActionQuantity = ActionQuantity * (i+1);
      for (int i=0; i<OutResolution - 2; i++)
         ActionQuantity = ActionQuantity / (i+1);
      ActionQuantity = ActionQuantity / 2;
      //----------------------------------------
   } while (ActionQuantity < (2 * EnvironmentAims->EnvironmentResolution)); // Пока количество действий не будет равно или больше требуемому

   EnvironmentAims->OutputResolution = OutResolution;

   // Генерируем среды, пока не добьемся нужного коэффициента заполненности
   while (!CoefOK)
   {
      //!!!
      int FullAimQuantity = rand()%1000; //1100 Генерируем кол-во полный целей (без учета подцелей)
      //FullAimQuantity = 100; //!!!!
      // Генерируем сами цели
      TEnvironmentAims FullAims;
      for (int CurrentAim=0; CurrentAim<FullAimQuantity; CurrentAim++)
      {
         // Находим длину полной цели
         FullAims.Aim[CurrentAim].Complexity = rand()%(MaxComplexity - MinMaxComplexity + 1) + MinMaxComplexity;
         // Находим длину минимальной подцели (при этом временно записываем ее в переменную VisitTime)
         FullAims.Aim[CurrentAim].VisitTime = rand()%(FullAims.Aim[CurrentAim].Complexity - MinComplexity + 1) + MinComplexity;
         int Fault = 0; // Признак оишбочной цели (идентична ранее сгенеированным)
         // Генерируем новую (неидентичную старым) цель
         do
         {
            Fault = 0;
            // Генерируем непротиворечивую цель
            for (int CurrentBit=0; CurrentBit<FullAims.Aim[CurrentAim].Complexity; CurrentBit++)
            {
               int CurrentPosition;
			   bool CurrentValue; // Сгенерированные значения позиции изменения и направления
               int Stop = 0; // Признак того, что сгенерированый бит потиворечит предыдущим
               do //Генерируем непротиворечивое действие
               {
                  Stop = 0;
                  // Генерируем предполагаемое действие
                  CurrentPosition = rand()%EnvironmentResolution;
                  CurrentValue = rand()%2;
                  int k = CurrentBit - 1;
                  // Пробегаемся по предыдущим действиям до последнего дейтсвия в отношении того же бита
                  while ((!Stop) && (k!=-1))
                  {
                     if (FullAims.Aim[CurrentAim].AimStructure[k].Number == CurrentPosition) Stop = 1;
                     k--;
                  }
                  if (Stop == 1) // Если найдено такое действие
                     if (FullAims.Aim[CurrentAim].AimStructure[k+1].Value != CurrentValue) // Если предыдущий перевод был противоположным, то действие непротиворечиво
                        Stop = 0;

               }while (Stop);

               FullAims.Aim[CurrentAim].AimStructure[CurrentBit].Value = CurrentValue;
               FullAims.Aim[CurrentAim].AimStructure[CurrentBit].Number = CurrentPosition;
            }
            // После генерации непротиворечивой цели необходимо сравнить ее со всеми предыдущими на предмет идентичности
            int CurrentCompareAim=0;
            while ((CurrentCompareAim < CurrentAim) && (!Fault))
            {
               // Сравниваем две цели
               Fault = CompareDifLentghAim(&(FullAims.Aim[CurrentAim]), &(FullAims.Aim[CurrentCompareAim]));
               // Если процедура показывает, что они идентичны, то пытаемся увеличить минимальную сложность подцели пока цели станут не идентичны
               while ((Fault) && (FullAims.Aim[CurrentAim].VisitTime<FullAims.Aim[CurrentAim].Complexity))
               {
                  FullAims.Aim[CurrentAim].VisitTime++;
                  Fault = CompareDifLentghAim(&(FullAims.Aim[CurrentAim]), &(FullAims.Aim[CurrentCompareAim]));
               }
               CurrentCompareAim++;
            }
         } while (Fault); // Сгенерировали правильную цель
      }

      // После генерации всех полных целей необходимо сгенерировать итоговую среду с подцелями
      EnvironmentAims->AimQuantity = 0;
      // Считаем кол-во всех целей
      for (int i=0; i<FullAimQuantity; i++)
         EnvironmentAims->AimQuantity += FullAims.Aim[i].Complexity - FullAims.Aim[i].VisitTime + 1;
      int CurrentAim=0; // Счетчие целей в итоговой среде
      for (int CurrentFullAim = 0; CurrentFullAim<FullAimQuantity; CurrentFullAim++)
         for (int CurrentSubAim = 0; CurrentSubAim<FullAims.Aim[CurrentFullAim].Complexity-FullAims.Aim[CurrentFullAim].VisitTime+1; CurrentSubAim++)
         {
            int Complexity = FullAims.Aim[CurrentFullAim].VisitTime + CurrentSubAim;
            double Reward = 0;
            switch (Complexity){
               case 2:
                  Reward = 20;
                  break;
               case 3:
                  Reward = 30;
                  break;
               case 4:
                  Reward = 40;
                  break;
               case 5:
                  Reward = 50;
                  break;
               case 6:
                  Reward = 60;
                  break;
            }

            EnvironmentAims->Aim[CurrentAim].Complexity = Complexity;
            EnvironmentAims->Aim[CurrentAim].Reward = Reward;
            for (int CurrentBit = 0; CurrentBit<Complexity; CurrentBit++)
            {
               EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Value = FullAims.Aim[CurrentFullAim].AimStructure[CurrentBit].Value;
               EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Number = FullAims.Aim[CurrentFullAim].AimStructure[CurrentBit].Number;
            }
            CurrentAim++;
         }

      RealEnvCoefFull = CoefFullEnvironment(EnvironmentAims);
      // Проверяем реальный коэффициент заполненности на попадание в заданный диапазон
      CoefOK = (RealEnvCoefFull >= RequiredEnvCoefFull-Eps) && (RealEnvCoefFull <= RequiredEnvCoefFull+Eps);
      //CoefOK = (EnvironmentAims->AimQuantity == 100); //!!!
   }

   return RealEnvCoefFull;
}

// Запись среды в файл
void WriteEnvironmentToFile(TEnvironmentAims* EnvironmentAims, char* EnvironmentFilename)
{
   FILE* hEnvironmentFile = fopen(EnvironmentFilename, "w");

   fprintf(hEnvironmentFile, "%i\n%i\n", EnvironmentAims->EnvironmentResolution, EnvironmentAims->AimQuantity);
   for (int CurrentAim=0; CurrentAim<EnvironmentAims->AimQuantity; CurrentAim++)
   {
      fprintf(hEnvironmentFile, "%i\t%.1f\n", EnvironmentAims->Aim[CurrentAim].Complexity,  EnvironmentAims->Aim[CurrentAim].Reward);
      for (int CurrentBit=0; CurrentBit <  EnvironmentAims->Aim[CurrentAim].Complexity; CurrentBit++)
         fprintf(hEnvironmentFile, "%i\t%i\t",  EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Number, EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Value);
      fprintf(hEnvironmentFile, "\n");
   }

   fclose(hEnvironmentFile);
}

// Генерация пула сред
void GenerateEnvironmentsPool(double StartCoefficient, double EndCoefficient, double Step, int ReplyEnvQuanity, int FirstEnvironmentID, char* EnvironmentDirectory, int EnvironmentResolution, double Eps, int MaxComplexity, int MinMaxComplexity, int MinComplexity)
{
   srand((unsigned) time(0)); // Инициализация генератора случайных числе
   // Три тестовых запуска генератора (чтобы развести в стороны значения)
   int tmp_rand = rand()%1000; // Тестовый запуск генератора случайных чисел
   tmp_rand = rand()%1000; // Тестовый запуск генератора случайных чисел
   tmp_rand = rand()%1000; // Тестовый запуск генератора случайных чисел


   int EnvironmentsQuantity = static_cast<int>((EndCoefficient - StartCoefficient)/Step) + 1;

   char ListOfEnvFilename[255];
   sprintf(ListOfEnvFilename,"%s/ListOfEnvironments%i-%i.txt", EnvironmentDirectory, FirstEnvironmentID, FirstEnvironmentID + EnvironmentsQuantity*ReplyEnvQuanity - 1);
   FILE* hListOfEnvFile = fopen(ListOfEnvFilename, "w");

   for (int i=0; i<EnvironmentsQuantity; i++)
   {
      double RequiredCoefficient = StartCoefficient + i*Step;
      for (int j=0; j < ReplyEnvQuanity; j++)
      {
         TEnvironmentAims EnvironmentAims;
         double RealCoefficient = GenerateEnvironment(&EnvironmentAims, EnvironmentResolution, RequiredCoefficient, Eps, MaxComplexity, MinMaxComplexity, MinComplexity);

         char EnvironmentFilename[255];
         GetEnvironmentFilename(EnvironmentFilename, EnvironmentDirectory, FirstEnvironmentID + i*ReplyEnvQuanity + j);
         WriteEnvironmentToFile(&EnvironmentAims,EnvironmentFilename);
         printf("%i:\tAimQuantity = %i\t Coefficient = %.4f\n", FirstEnvironmentID + i*ReplyEnvQuanity+j, EnvironmentAims.AimQuantity, RealCoefficient);
         fprintf(hListOfEnvFile, "%i\t%.4f\t%.4f\t%i\n", FirstEnvironmentID + i*ReplyEnvQuanity+j, RealCoefficient, 1.0/RealCoefficient, EnvironmentAims.AimQuantity);
      }
   }

   fclose(hListOfEnvFile);
}















