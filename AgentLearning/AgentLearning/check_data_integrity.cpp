#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "service.h"
#include "service_network.h"
#include "environment.h"

#include "check_data_integrity.h"

// Анализ возвращенного значения ошибки (0 - все нормально, 1 - есть ошибка)
bool read_err_analysis(int err_sign)
{
   if ((err_sign == EOF) || (err_sign == 0)) // Если это конец файла или мы не смогли считать значение нужного типа, то возвращаем ошибку
      return 1;
   else
      return 0;
}

//Процедура проверки целостности следующей записи сети из пулов в файле (возвращает 0 - если все нормально, 1 - если есть ошибка - в основном не хватает данных)
bool check_integrity_of_pool_network_record(FILE* hNetworkFile, int EnvironmentResolution, int OutputResolution, int NetworkMode)
{
   TPoolNetwork* PoolNetwork = CreatePoolNetwork();
   int err_sign = 0; // Признак невозможности считывания из файла
   char StrFromFile[255];
   err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем технический номер сети
   if (read_err_analysis(err_sign)) return 1;
   err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер первого родителя
   if (read_err_analysis(err_sign)) return 1;
   PoolNetwork->Parents[0] = atoi(StrFromFile);
   err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер второго родителя
   if (read_err_analysis(err_sign)) return 1;
   PoolNetwork->Parents[1] = atoi(StrFromFile);

   TNeuralPool* CurrentPool = NULL;
   //---------- Заполнение генома пулов --------------
   if (!NetworkMode) // Если режим "одновременный"
   {
      err_sign = fscanf(hNetworkFile, "%s", StrFromFile);
      if (read_err_analysis(err_sign)) return 1;

      while (strcmp(StrFromFile,"|")) // Cчитываем до разделителя между пулами и связями
      {
         ++ (PoolNetwork->PoolQuantity);
         int PoolLayer = atoi(StrFromFile);
         int PoolType;
         if (PoolNetwork->PoolQuantity <= EnvironmentResolution) PoolType = 0; // Входные и выходные нейроны идут в начале списка
         else
            if (PoolNetwork->PoolQuantity <= (EnvironmentResolution + OutputResolution) )
               PoolType = 2;
            else
               PoolType = 1;

         err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем среднее смещения пула
         if (read_err_analysis(err_sign)) return 1;
         double BiasMean = atof(StrFromFile);
         err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем дисперсию смещения пула
         if (read_err_analysis(err_sign)) return 1;
         double BiasVariance = atof(StrFromFile);
         err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем размерность пула
         if (read_err_analysis(err_sign)) return 1;

         if (CurrentPool == NULL) // Если нет еще ни одного пула
         {
            PoolNetwork->PoolsStructure = CreateNeuralPool(PoolNetwork->PoolQuantity, PoolType, atoi(StrFromFile), BiasMean, BiasVariance, PoolLayer, NULL, NULL, NULL);
            CurrentPool = PoolNetwork->PoolsStructure;
         }
         else
         {
            CurrentPool->next = CreateNeuralPool(PoolNetwork->PoolQuantity, PoolType, atoi(StrFromFile), BiasMean, BiasVariance, PoolLayer, NULL, NULL, NULL);
            CurrentPool = CurrentPool->next;
         }
         err_sign = fscanf(hNetworkFile, "%s", StrFromFile);
         if (read_err_analysis(err_sign)) return 1;
      }
   }
   else // Если режим "синхронный"
   {
      PoolNetwork->PoolQuantity = atoi(StrFromFile);
      int PoolID=0;
      while (PoolID < PoolNetwork->PoolQuantity)
      {
         int PoolType;
         if (PoolID < EnvironmentResolution)
            PoolType = 0;
         else
            if (PoolID < EnvironmentResolution + OutputResolution)
               PoolType = 2;
            else
               PoolType = 0;

         err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем среднее смещения пула
         if (read_err_analysis(err_sign)) return 1;
         double BiasMean = atof(StrFromFile);
         err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем дисперсию смещения пула
         if (read_err_analysis(err_sign)) return 1;
         double BiasVariance = atof(StrFromFile);
         err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем размерность пула
         if (read_err_analysis(err_sign)) return 1;

         if (CurrentPool == NULL) // Если нет еще ни одного пула
         {
            PoolNetwork->PoolsStructure = CreateNeuralPool(++PoolID, PoolType, atoi(StrFromFile), BiasMean, BiasVariance, 1, NULL, NULL, NULL);
            CurrentPool = PoolNetwork->PoolsStructure;
         }
         else
         {
            CurrentPool->next = CreateNeuralPool(++PoolID, PoolType, atoi(StrFromFile), BiasMean, BiasVariance, 1, NULL, NULL, NULL);
            CurrentPool = CurrentPool->next;
         }
      }

      err_sign = fscanf(hNetworkFile, "%s", StrFromFile);
      if (read_err_analysis(err_sign)) return 1;
   }
   //---------------------------------------------------
   //----------- Заполнение генома связей сети ------------
   err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID пресинаптического пула
   if (read_err_analysis(err_sign)) return 1;
   while (strcmp(StrFromFile,"||")) // Считываем до разделителя между связями и предикторными связями
   {
      ++(PoolNetwork->ConnectionQuantity);

      TNeuralPool* PrePool = GetPointer2Pool(PoolNetwork, atoi(StrFromFile)); // Ищем пресинаптический пул

      err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID постсинаптического пула
      if (read_err_analysis(err_sign)) return 1;
      CurrentPool = GetPointer2Pool(PoolNetwork, atoi(StrFromFile)); // Ищем постсинаптический пул

      TPoolConnection* CurrentPoolConnection; // Последняя связь постсинаптического пула
      CurrentPoolConnection = CurrentPool->ConnectednessSet;
      if (CurrentPoolConnection != NULL) // Если уже есть входящие связи у пула
         while (CurrentPoolConnection->next!=NULL) // Считываем до последней связи
            CurrentPoolConnection = CurrentPoolConnection->next;

      err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем среднее веса связи
      if (read_err_analysis(err_sign)) return 1;
      double WeightMean = atof(StrFromFile);
      err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем дисперсию веса связи
      if (read_err_analysis(err_sign)) return 1;
      double WeightDisp = atof(StrFromFile);
      err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак экспрессии
      if (read_err_analysis(err_sign)) return 1;
      bool ConnectionEnabled = atoi(StrFromFile);
      err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем поколение выключения
      if (read_err_analysis(err_sign)) return 1;
      int ConnectionDisStep = atoi(StrFromFile);
      err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер инновации
      if (read_err_analysis(err_sign)) return 1;
      double ConnectionInnNumber = atof(StrFromFile);

      if (CurrentPoolConnection != NULL) // Если уже есть входящие синапсы у нейрона
         CurrentPoolConnection->next = CreatePoolConnection(PoolNetwork->ConnectionQuantity, WeightMean, WeightDisp, ConnectionEnabled, ConnectionInnNumber, ConnectionDisStep, PrePool, CurrentPool, NULL);
      else
         CurrentPool->ConnectednessSet = CreatePoolConnection(PoolNetwork->ConnectionQuantity, WeightMean, WeightDisp, ConnectionEnabled, ConnectionInnNumber, ConnectionDisStep, PrePool, CurrentPool, NULL);

      err_sign = fscanf(hNetworkFile, "%s", StrFromFile);
      if (read_err_analysis(err_sign)) return 1;
   }

   err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID пресинаптического пула для предикторной связи
   if (read_err_analysis(err_sign)) return 1;
   while (strcmp(StrFromFile,"|||")) // Считываем до разделителя между предикторными связями и следующей сетью
   {
      ++(PoolNetwork->PredConnectionQuantity);

      TNeuralPool* PrePool = GetPointer2Pool(PoolNetwork, atoi(StrFromFile)); // Ищем пресинаптический пул

      err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID постсинаптического пула
      if (read_err_analysis(err_sign)) return 1;
      CurrentPool = GetPointer2Pool(PoolNetwork, atoi(StrFromFile)); // Ищем постсинаптический пул

      TPoolPredConnection* CurrentPoolPredConnection; // Последняя пред. связь постсинаптического пула
      CurrentPoolPredConnection = CurrentPool->PredConnectednessSet;
      if (CurrentPoolPredConnection != NULL) // Если уже есть входящие связи у пула
         while (CurrentPoolPredConnection->next!=NULL) // Считываем до последней связи
            CurrentPoolPredConnection = CurrentPoolPredConnection->next;

      err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак экспрессии
      if (read_err_analysis(err_sign)) return 1;
      bool PredConnectionEnabled = atoi(StrFromFile);
      err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем поколение выключения
      if (read_err_analysis(err_sign)) return 1;
      int PredConnectionDisStep = atoi(StrFromFile);
      err_sign = fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер инновации
      if (read_err_analysis(err_sign)) return 1;
      double PredConnectionInnNumber = atof(StrFromFile);

      if (CurrentPoolPredConnection != NULL) // Если уже есть входящие предикторные связи у нейрона
         CurrentPoolPredConnection->next = CreatePoolPredConnection(PoolNetwork->PredConnectionQuantity, PredConnectionEnabled, PredConnectionInnNumber, PredConnectionDisStep, PrePool, CurrentPool, NULL);
      else
         CurrentPool->PredConnectednessSet = CreatePoolPredConnection(PoolNetwork->PredConnectionQuantity, PredConnectionEnabled, PredConnectionInnNumber, PredConnectionDisStep, PrePool, CurrentPool, NULL);

      err_sign = fscanf(hNetworkFile, "%s", StrFromFile);
      if (read_err_analysis(err_sign)) return 1;
   }

   PoolNetwork = DeletePoolNetwork(PoolNetwork);
   return 0;
}

//Процедура проверки целостности файла с записями сети из пулов (возвращает 0 - если все нормально, 1 - если есть ошибка - в основном не хватает данных)
bool check_integrity_of_pool_network_file(char* NetworkFilename, int NetworksQuantity, int EnvironmentResolution, int OutputResolution, int NetworkMode)
{
   FILE* hNetworkFile = fopen(NetworkFilename, "r");
   int err_sign = 0;

   for (int CurrentNetwork = 0; CurrentNetwork < NetworksQuantity; ++CurrentNetwork)
   {
      err_sign = check_integrity_of_pool_network_record(hNetworkFile, EnvironmentResolution, OutputResolution, NetworkMode);
      if (err_sign) break;
   }

   return err_sign;
}

// Проверка целостности данных перед анализом путем прогона лучших популяций
void check_integrity_of_best_populations_data(TGeneralRunSettings* GeneralRunSettings, TModeSettings* ModeSettings, TDirectorySettings* DirectorySettings, bool NoEnVariableCheck, int AgentPopulationQuantity)
{
   char LogFilename[255];
   sprintf(LogFilename, "%s/AnalysisIntegrityLog_En%i-%i_Var%i_Noen%i.txt", DirectorySettings->WorkDirectory, GeneralRunSettings->FirstEnvironmentNumber, GeneralRunSettings->LastEnvironmentNumber, GeneralRunSettings->VariableNumber, NoEnVariableCheck);
   FILE* hLogFile = fopen(LogFilename, "w");

   for (int CurrentVarNumber=GeneralRunSettings->VariableNumber; CurrentVarNumber<=GeneralRunSettings->LastVariableNumber; CurrentVarNumber++)
      for (int CurrentEnNumber=GeneralRunSettings->FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings->LastEnvironmentNumber; CurrentEnNumber++)
         for (int CurrentTryNumber=GeneralRunSettings->FirstTryNumber; CurrentTryNumber<=GeneralRunSettings->LastTryNumber; CurrentTryNumber++)
         {
            TRunSettings RunSettings;
            RunSettings.EnvironmentNumber = CurrentEnNumber;
            RunSettings.TryNumber = CurrentTryNumber;
            RunSettings.VariableNumber = CurrentVarNumber;

            char EnvironmentFilename[255];
            GetEnvironmentFilename(EnvironmentFilename, DirectorySettings->EnvironmentDirectory, RunSettings.EnvironmentNumber);

            // Загружаем среду
            TEnvironmentAims EnvironmentAims;
            InitEnvironment(&EnvironmentAims, EnvironmentFilename);

            char BestPopulationFilename[255];
            GetBestPopulationFilename(BestPopulationFilename, &RunSettings, ModeSettings, DirectorySettings->ResultDirectory, NoEnVariableCheck);
            int err_sing = check_integrity_of_pool_network_file(BestPopulationFilename, AgentPopulationQuantity, EnvironmentAims.EnvironmentResolution, EnvironmentAims.OutputResolution, ModeSettings->NetworkMode);
            if (err_sing)
               fprintf(hLogFile, "Environment: %i Try: %i - ERROR\n", CurrentEnNumber, CurrentTryNumber);
            else
               fprintf(hLogFile, "Environment: %i Try: %i - OK\n", CurrentEnNumber, CurrentTryNumber);
         }

   fclose(hLogFile);
}

