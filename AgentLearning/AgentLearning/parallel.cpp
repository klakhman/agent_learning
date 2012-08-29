#define  _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include "types.h"
#include "evolution.h"
#include "environment.h"
#include "service.h"
#include "analysis.h"

#include "parallel.h"

extern const int MessageLenght;

// Процедура заполнения параметров из файла настроек
void FillSettingsFromFile(const char SettingsFilename[], TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TModeSettings* ModeSettings, TDirectorySettings* DirectorySettings, TGeneralRunSettings* GeneralRunSettings, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings, TLearningSettings* LearningSettings, double* EnVariablePorbability)
{
   FILE *hSettingsFile;
   char tmp_str[255];
   hSettingsFile = fopen(SettingsFilename,"r");

   while (fscanf(hSettingsFile,"%s", tmp_str) != EOF) // Парсинг файла с записанными найстройками
   {
      // Временные параметры
      if (!strcmp(tmp_str,"EvolutionTime")) { fscanf(hSettingsFile, "%s", tmp_str); TimeSettings->EvolutionTime = atoi(tmp_str); }
      if (!strcmp(tmp_str,"AgentLifetime")) { fscanf(hSettingsFile, "%s", tmp_str); TimeSettings->AgentLifetime = atoi(tmp_str); }
      if (!strcmp(tmp_str,"AgentPopulationQuantity")) { fscanf(hSettingsFile,"%s", tmp_str); TimeSettings->AgentPopulationQuantity = atoi(tmp_str); }
      if (!strcmp(tmp_str,"RewardRecoveryTime")) { fscanf(hSettingsFile, "%s", tmp_str); TimeSettings->RewardRecoveryTime = atoi(tmp_str); }

      // Параметры режимов
      if (!strcmp(tmp_str,"NetworkMode")) { fscanf(hSettingsFile, "%s", tmp_str); ModeSettings->NetworkMode = atoi(tmp_str); }
      if (!strcmp(tmp_str,"EvolutionMode")) { fscanf(hSettingsFile, "%s", tmp_str); ModeSettings->EvolutionMode = atoi(tmp_str); }
      if (!strcmp(tmp_str,"RewardMode")) { fscanf(hSettingsFile, "%s", tmp_str); ModeSettings->RewardMode = atoi(tmp_str); }

      // Общие параметры запуска
      if (!strcmp(tmp_str,"FirstEnvironmentNumber")) { fscanf(hSettingsFile, "%s", tmp_str); GeneralRunSettings->FirstEnvironmentNumber = atoi(tmp_str); }
      if (!strcmp(tmp_str,"LastEnvironmentNumber")) { fscanf(hSettingsFile, "%s", tmp_str); GeneralRunSettings->LastEnvironmentNumber = atoi(tmp_str); }
      if (!strcmp(tmp_str,"FirstTryNumber")) { fscanf(hSettingsFile, "%s", tmp_str); GeneralRunSettings->FirstTryNumber = atoi(tmp_str); }
      if (!strcmp(tmp_str,"LastTryNumber")) { fscanf(hSettingsFile, "%s", tmp_str); GeneralRunSettings->LastTryNumber = atoi(tmp_str); }
      if (!strcmp(tmp_str,"VariableNumber")) { fscanf(hSettingsFile, "%s", tmp_str); GeneralRunSettings->VariableNumber = atoi(tmp_str); GeneralRunSettings->LastVariableNumber = atoi(tmp_str); }

      //Рабочие директории
      if (!strcmp(tmp_str,"EnvironmentDirectory")) { fscanf(hSettingsFile, "%s", tmp_str); strcpy(DirectorySettings->EnvironmentDirectory,tmp_str); }
      if (!strcmp(tmp_str,"ResultDirectory")) { fscanf(hSettingsFile, "%s", tmp_str); strcpy(DirectorySettings->ResultDirectory,tmp_str); }
      if (!strcmp(tmp_str,"WorkDirectory")) { fscanf(hSettingsFile, "%s", tmp_str); strcpy(DirectorySettings->WorkDirectory, tmp_str); }

      //Различные мутационные параметры
      if (!strcmp(tmp_str,"MutWeightProbability")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->MutWeightProbability = atof(tmp_str); }
      if (!strcmp(tmp_str,"MutWeightMeanDisp")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->MutWeightMeanDisp = atof(tmp_str); }
      if (!strcmp(tmp_str,"MutWeightDispDisp")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->MutWeightDispDisp = atof(tmp_str); }
      if (!strcmp(tmp_str,"DisLimit")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->DisLimit = atoi(tmp_str); }
      if (!strcmp(tmp_str,"EnConProb")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->EnConProb = atof(tmp_str); }
      if (!strcmp(tmp_str,"DisConProb")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->DisConProb = atof(tmp_str); }
      if (!strcmp(tmp_str,"InsertPoolProb")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->InsertPoolProb = atof(tmp_str); }
      if (!strcmp(tmp_str,"AddConnectionProb")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->AddConnectionProb = atof(tmp_str); }
      if (!strcmp(tmp_str,"DeleteConnectionProb")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->DeleteConnectionProb = atof(tmp_str); }
      if (!strcmp(tmp_str,"AddPredConnectionProb")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->AddPredConnectionProb = atof(tmp_str); }
      if (!strcmp(tmp_str,"DeletePredConnectionProb")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->DeletePredConnectionProb = atof(tmp_str); }
      if (!strcmp(tmp_str,"DuplicatePoolProb")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->DuplicatePoolProb = atof(tmp_str); }
      if (!strcmp(tmp_str,"PoolDivisionCoef")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->PoolDivisionCoef = atof(tmp_str); }
      if (!strcmp(tmp_str,"PoolStandartAmount")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->PoolStandartAmount = atoi(tmp_str); }
      if (!strcmp(tmp_str,"ConnectionStandartAmount")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->ConnectionStandartAmount = atoi(tmp_str); }
      if (!strcmp(tmp_str,"PenaltyRewardLimit")) { fscanf(hSettingsFile, "%s", tmp_str); MutationSettings->PenaltyRewardLimit = atoi(tmp_str); }

      // Параметры первичного системогенеза
      if (!strcmp(tmp_str,"InitialPoolCapacity")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->InitialPoolCapacity = atoi(tmp_str); }
      if (!strcmp(tmp_str,"DevelopSynapseProbability")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->DevelopSynapseProbability = atof(tmp_str); }
      if (!strcmp(tmp_str,"DevelopPredConnectionProbability")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->DevelopPredConnectionProbability = atof(tmp_str); }
      if (!strcmp(tmp_str,"PrimarySystemogenesisTime")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->PrimarySystemogenesisTime = atoi(tmp_str); }
      if (!strcmp(tmp_str,"SpontaneousActivityProb")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->SpontaneousActivityProb = atof(tmp_str); }
      if (!strcmp(tmp_str,"ActiveNeuronsPercent")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->ActiveNeuronsPercent = atof(tmp_str); }
      if (!strcmp(tmp_str,"SynapsesActivityTreshold")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->SynapsesActivityTreshold = atof(tmp_str); }
      if (!strcmp(tmp_str,"SignificanceTreshold")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->SignificanceTreshold = atof(tmp_str); }

      // Параметры обучения
      if (!strcmp(tmp_str,"MismatchSignificanceTreshold")) { fscanf(hSettingsFile, "%s", tmp_str); LearningSettings->MismatchSignificanceTreshold = atof(tmp_str); }

      if (!strcmp(tmp_str,"EnVariablePorbability")) { fscanf(hSettingsFile, "%s", tmp_str); *EnVariablePorbability = atof(tmp_str); }
   }
}

//Процедура заполнения параметров из командной строки
void FillCommandParametrs(int argc, char **argv, TGeneralRunSettings* GeneralRunSettings, TModeSettings* ModeSettings, double* EnConProb, double* DisConProb, double* EnVariableProbability, int* ProgramMode, bool* NoEnVariableCheck)
{
   int CurrentArgNumber = 2; // Текущий номер параметра (в первом записан путь к файлу настроек)
   while (CurrentArgNumber < argc) // Пока мы не прошли все параметры
   {
      switch (argv[CurrentArgNumber][1]) // Расшифровываем параметр и заполняем (по второму полю, так как в первом "-")
      {
         case 'e': // Если это номера первой и последней среды
            CurrentArgNumber++;
            GeneralRunSettings->FirstEnvironmentNumber = atoi(argv[CurrentArgNumber]);
            CurrentArgNumber++;
            GeneralRunSettings->LastEnvironmentNumber = atoi(argv[CurrentArgNumber]);
            break;
         case 't': // Если это номера попыток
            CurrentArgNumber++;
            GeneralRunSettings->FirstTryNumber = atoi(argv[CurrentArgNumber]);
            CurrentArgNumber++;
            GeneralRunSettings->LastTryNumber = atoi(argv[CurrentArgNumber]);
            break;
         case 'v': // Если это номер набора параметров
            CurrentArgNumber++;
            GeneralRunSettings->VariableNumber = atoi(argv[CurrentArgNumber]);
            GeneralRunSettings->LastVariableNumber = atoi(argv[CurrentArgNumber]);
            break;
         case 'c': // Если это вероятности включения и выключения синапсов
            CurrentArgNumber++;
            *EnConProb = atof(argv[CurrentArgNumber]);
            CurrentArgNumber++;
            *DisConProb = atof(argv[CurrentArgNumber]);
            break;
         case 'p': // Если это степень квазистационарности среды
            CurrentArgNumber++;
            *EnVariableProbability = atof(argv[CurrentArgNumber]);
            break;
         case 'x': // Если это режим эволюционного алгоритма
            CurrentArgNumber++;
            ModeSettings->EvolutionMode = atoi(argv[CurrentArgNumber]);
            break;
         case 'y': // Если это режим прогона нейросети
            CurrentArgNumber++;
            ModeSettings->NetworkMode = atoi(argv[CurrentArgNumber]);
            break;
         case 'z': // Если это режим начисления штрафной функции
            CurrentArgNumber++;
            ModeSettings->RewardMode = atoi(argv[CurrentArgNumber]);
            break;
         case 'm': // Если это режим работы программы
            CurrentArgNumber++;
            *ProgramMode = atoi(argv[CurrentArgNumber]);
            break;
         case 'f': // Если это признак стационарности среды (для анализа)
            CurrentArgNumber++;
            *NoEnVariableCheck = atoi(argv[CurrentArgNumber]);
            break;
      }
      CurrentArgNumber++;
   }
}

// Процедура декодирования сообщения с заданием для рабочего процесса
void DecodeTaskMessage(char* TaskMessage, TRunSettings* RunSettings, TModeSettings* ModeSettings, double* DisConProb, double* EnConProb, double* EnVariablePorbability, bool* NoEnVariableCheck)
{
   char tmp_str[10]; // Переменная, в которую записывается текущее значение (число)
   strcpy(tmp_str,"");

   for (unsigned int i=0;i<strlen(TaskMessage);i++) // Проходимся по всему сообщению
   {
      // В начале полностью записываем число в строковую переменную
      if (((TaskMessage[i]>='0') && (TaskMessage[i]<='9')) || TaskMessage[i]=='.') // Если символ число
         sprintf(tmp_str, "%s%c", tmp_str, TaskMessage[i]);
      else // Если это уже указание на принадлежность параметра
      {
         switch (TaskMessage[i]) // Проверяем, какой у нас текущий параметр
         {
            case 'e': // Если это номер среды
                     RunSettings->EnvironmentNumber = atoi(tmp_str);
                     break;
            case 'd': // Если это вероятность выключения связи
                     *DisConProb = atof(tmp_str);
                     break;
            case 'c': // Если это вероятность включения связи
                     *EnConProb = atof(tmp_str);
                     break;
            case 'v': // Если это степень квазистационарности среды
                     *EnVariablePorbability = atof(tmp_str);
                     break;
            case 't': // Если это номер попытки
                     RunSettings->TryNumber = atoi(tmp_str);
                     break;
            case 'p': // Если это номер набора параметров
                     RunSettings->VariableNumber = atoi(tmp_str);
                     break;
            case 'g': // Если это режим эволюционного алгоритма
                     ModeSettings->EvolutionMode = atoi(tmp_str);
                     break;
            case 'n': // Если это режим прогона нейросети
                     ModeSettings->NetworkMode = atoi(tmp_str);
                     break;
            case 'r': // Если это режим начисления награды
                     ModeSettings->RewardMode = atoi(tmp_str);
                     break;
            case 'f': // Если это признак того, что осуществляется прогон популяции, которая эволюционировала на стационарной среде
                     *NoEnVariableCheck = atoi(tmp_str);
                     break;
         }

         strcpy(tmp_str,"");
      }
   }
}

// Основная процедура распараллеливания - выдача и выполнение заданий
void ParallelTasks(int argc, char **argv)
{
   MPI_Init(&argc, &argv);
   //------- Создание семейства/структуры процессов --------
   char InMessage[MessageLenght]; // Входящее сообщение для процесса
   char OutMessage[MessageLenght]; // Исходящее сообщение для процесса
   int PrRank; // Номер процесса
   int PrsSize; // Общее кол-во процессов
   int type = 99; // Служебный тип сообщений
   MPI_Status status;

   MPI_Comm_size(MPI_COMM_WORLD, &PrsSize); // Определение общего количества процессов
   MPI_Comm_rank(MPI_COMM_WORLD, &PrRank); // Определение процессом своего номера

   // Создаем структуры и переменные настроек
   TTimeSettings TimeSettings;
   TMutationSettings MutationSettings;
   TModeSettings ModeSettings;
   TDirectorySettings DirectorySettings;
   TGeneralRunSettings GeneralRunSettings;
   TPrimarySystemogenesisSettings PrimarySystemogenesisSettings;
   TLearningSettings LearningSettings;
   double EnVariableProbability = 0;
   int ProgramMode = 0;
   bool NoEnVariableCheck = 0;

   // Сначала заполняем все настройки из файла
   FillSettingsFromFile(argv[1], &TimeSettings, &MutationSettings, &ModeSettings, &DirectorySettings, &GeneralRunSettings, &PrimarySystemogenesisSettings, &LearningSettings, &EnVariableProbability);

   if (PrRank == 0) // Если это родительский процесс
   {
      // Заполняем настройки из командной строки (это только для родительского процесса)
      FillCommandParametrs(argc, argv, &GeneralRunSettings, &ModeSettings, &(MutationSettings.EnConProb), &(MutationSettings.DisConProb), &EnVariableProbability, &ProgramMode, &NoEnVariableCheck);

      char RunLogFilename[255];
      sprintf(RunLogFilename, "%s/EvolutionRunLog_En%i-%i_Var%i_EnVar%.5f_EvM%i.txt", DirectorySettings.WorkDirectory, GeneralRunSettings.FirstEnvironmentNumber, GeneralRunSettings.LastEnvironmentNumber, GeneralRunSettings.VariableNumber, EnVariableProbability, ModeSettings.EvolutionMode);
      FILE* hRunLogFile = fopen(RunLogFilename, "w");

      // Выдача дочерним процессам всех заданий данных на выполнение программе
      for (int CurrentVarNumber=GeneralRunSettings.VariableNumber; CurrentVarNumber<=GeneralRunSettings.LastVariableNumber; CurrentVarNumber++)
      {
         for (int CurrentEnNumber=GeneralRunSettings.FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings.LastEnvironmentNumber; CurrentEnNumber++)
         {
            for (int CurrentTryNumber=GeneralRunSettings.FirstTryNumber; CurrentTryNumber<=GeneralRunSettings.LastTryNumber; CurrentTryNumber++)
            {
               // Если не всем процессам выданы изначальные задания
               if( ( (CurrentVarNumber - GeneralRunSettings.VariableNumber)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentTryNumber - GeneralRunSettings.FirstTryNumber + 1) ) <= (PrsSize-1))
               {
                  // Определяем номер процесса, которому мы должны послать задание
                  int PrRankSend = (CurrentVarNumber - GeneralRunSettings.VariableNumber)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentTryNumber - GeneralRunSettings.FirstTryNumber + 1);
                  // Составляем сообщение
                  sprintf(OutMessage,"%ie%.5fd%.5fc%.5fv%it%ip%ig%in%ir",CurrentEnNumber, MutationSettings.DisConProb, MutationSettings.EnConProb, EnVariableProbability, CurrentTryNumber,CurrentVarNumber, ModeSettings.EvolutionMode, ModeSettings.NetworkMode, ModeSettings.RewardMode);
                  MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
                   // Записываем в лог выдачу задания
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Issued for pr.%i\n", CurrentEnNumber, CurrentTryNumber, PrRankSend);
               }
               // Если все процессы получили изначальные задания, то ждем завершения заданий и по ходу выдаем новые задания
               else
               {
                  // Ждем входяшее сообщение
                  MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, MPI_ANY_SOURCE, type, MPI_COMM_WORLD, &status);
                  char tmp_str[10]; // Строка, использующаяся для определения номера процесса
                  strcpy(tmp_str,"");
                  int PrRankSend; // Номер процесса для высылки задания
                  int CurrentTry, CurrentEnvironment; // Идентификаторы задания, которое выполнял процесс
                  // Расшифровка сообщения
                  for (unsigned int i=0; i<strlen(InMessage); i++)
                  {
                     if (((InMessage[i]>='0') && (InMessage[i]<='9'))||(InMessage[i]=='.')) // если символ число или точка
                        sprintf(tmp_str,"%s%c",tmp_str,InMessage[i]);
                     else
                     {
                        switch (InMessage[i]) {
                           case 'e':
                              CurrentEnvironment = atoi(tmp_str);
                              break;
                           case 't':
                              CurrentTry = atoi(tmp_str);
                              break;
                           case 'o':
                              PrRankSend = atoi(tmp_str);
                              break;
                        }
                        strcpy(tmp_str,"");
                     }
                  }
                  // Записываем в лог прием задания
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Done from pr.%i\n", CurrentEnvironment, CurrentTry, PrRankSend);
                  // Составляем сообщение и высылаем задание
                  sprintf(OutMessage,"%ie%.5fd%.5fc%.5fv%it%ip%ig%in%ir",CurrentEnNumber, MutationSettings.DisConProb, MutationSettings.EnConProb, EnVariableProbability, CurrentTryNumber,CurrentVarNumber, ModeSettings.EvolutionMode, ModeSettings.NetworkMode, ModeSettings.RewardMode);
                  MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
                  // Записываем в лог выдачу задания
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Issued for pr.%i\n", CurrentEnNumber, CurrentTryNumber, PrRankSend);
               }
            }
         }
      }
      // Когда все задания закончились, ждем пока все они будут выполнены и по ходу посылаем всем процессам команду о завершении
      int PrQuit = PrsSize-1; // Количество процессов которые еще выполняются и необходимо дождаться их окончания
      while (PrQuit > 0) // Пока не завершаться все процессы
      {
         MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, MPI_ANY_SOURCE, type, MPI_COMM_WORLD, &status);
         char tmp_str[10]; // Строка, использующаяся для определения номера процесса
         strcpy(tmp_str,"");

         int PrRankSend; // Номер процесса для высылки задания
         int CurrentTry, CurrentEnvironment; // Идентификаторы задания, которое выполнял процесс
         // Расшифровка сообщения
         for (unsigned int i=0; i<strlen(InMessage); i++)
         {
            if (((InMessage[i]>='0') && (InMessage[i]<='9'))||(InMessage[i]=='.')) // если символ число или точка
               sprintf(tmp_str,"%s%c",tmp_str,InMessage[i]);
            else
            {
               switch (InMessage[i]) {
                  case 'e':
                     CurrentEnvironment = atoi(tmp_str);
                     break;
                  case 't':
                     CurrentTry = atoi(tmp_str);
                     break;
                  case 'o':
                     PrRankSend = atoi(tmp_str);
                     break;
               }
               strcpy(tmp_str,"");
            }
         }
         // Записываем в лог прием задания
         fprintf(hRunLogFile, "Environment: %i Try: %i - Done from pr.%i\n", CurrentEnvironment, CurrentTry, PrRankSend);

         // Составляем сообщение о выходе и высылаем
         strcpy(OutMessage,"q");
         MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
         PrQuit--;
      }
      fclose(hRunLogFile);
   }

   else // Если это один из рабочих процессов
   {
      MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status); //Ждем сообщения с заданием
      while (strcmp(InMessage,"q")) // Если не было команды о выходе
      {
            TRunSettings RunSettings; // Параметры текущего запуска

            //Декодируем сообщение с заданием
            DecodeTaskMessage(InMessage, &RunSettings, &ModeSettings, &(MutationSettings.DisConProb), &(MutationSettings.EnConProb), &EnVariableProbability, NULL);

            // Определяем ядро инициализации
            RunSettings.RandomizeRank = (unsigned) time(0) + PrRank; // К ядру инизиацлизации случайных чисел добавляется номер процесса, чтобы развести изначально инициализируемые процессы

            // Запускаем рабочую процедуру эволюции
            PrepareEvolutionProcess(&RunSettings, &ModeSettings, &TimeSettings, &MutationSettings, &DirectorySettings, &PrimarySystemogenesisSettings, &LearningSettings, EnVariableProbability);

            //Посылка ответа о завершении работы над заданием
            sprintf(OutMessage,"%ie%it%io",RunSettings.EnvironmentNumber, RunSettings.TryNumber, PrRank);
            MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD);

            //Ожидание нового задания
            MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
      }
   }
    MPI_Finalize();
}

// Основная процедура распараллеливания анализа эволюции
void ParallelAnalysis(int argc, char **argv)
{
   MPI_Init(&argc, &argv);

   //------- Создание семейства/структуры процессов --------
   char InMessage[MessageLenght]; // Входящее сообщение для процесса
   char OutMessage[MessageLenght]; // Исходящее сообщение для процесса
   int PrRank; // Номер процесса
   int PrsSize; // Общее кол-во процессов
   int type = 99; // Служебный тип сообщений
   MPI_Status status;

   MPI_Comm_size(MPI_COMM_WORLD, &PrsSize); // Определение общего количества процессов
   MPI_Comm_rank(MPI_COMM_WORLD, &PrRank); // Определение процессом своего номера

   // Создаем структуры и переменные настроек
   TTimeSettings TimeSettings;
   TMutationSettings MutationSettings;
   TModeSettings ModeSettings;
   TDirectorySettings DirectorySettings;
   TGeneralRunSettings GeneralRunSettings;
   TPrimarySystemogenesisSettings PrimarySystemogenesisSettings;
   TLearningSettings LearningSettings;
   double EnVariableProbability = 0;
   int ProgramMode = 0;
   bool NoEnVariableCheck = 0;
   // Сначала заполняем все настройки из файла
   FillSettingsFromFile(argv[1], &TimeSettings, &MutationSettings, &ModeSettings, &DirectorySettings, &GeneralRunSettings, &PrimarySystemogenesisSettings, &LearningSettings, &EnVariableProbability);

   if (PrRank == 0) // Если это родительский процесс
   {
      // Заполняем настройки из командной строки (это только для родительского процесса)
      FillCommandParametrs(argc, argv, &GeneralRunSettings, &ModeSettings, &(MutationSettings.EnConProb), &(MutationSettings.DisConProb), &EnVariableProbability, &ProgramMode, &NoEnVariableCheck);

      // Проверка целостности данных перед анализом
      CheckIntegrityOfData(&GeneralRunSettings, &ModeSettings, &DirectorySettings, NoEnVariableCheck, TimeSettings.AgentPopulationQuantity);

      char RunLogFilename[255];
      sprintf(RunLogFilename, "%s/AnalysisRunLog_En%i-%i_Var%i_Noen%i_EnVar%.5f_EvM%i.txt", DirectorySettings.WorkDirectory, GeneralRunSettings.FirstEnvironmentNumber, GeneralRunSettings.LastEnvironmentNumber, GeneralRunSettings.VariableNumber, NoEnVariableCheck, EnVariableProbability, ModeSettings.EvolutionMode);
      FILE* hRunLogFile = fopen(RunLogFilename, "w");

      // Массивы средних и максимальных наград для всех анализируемых запусков (для всех сред и для всех попыток)
      //double AverageRewardArray[(GeneralRunSettings.LastVariableNumber - GeneralRunSettings.VariableNumber + 1)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1)];
      //double MaxRewardArray[(GeneralRunSettings.LastVariableNumber - GeneralRunSettings.VariableNumber + 1)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1)];
      double* AverageRewardArray = new double[(GeneralRunSettings.LastVariableNumber - GeneralRunSettings.VariableNumber + 1)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1)];
      double* MaxRewardArray = new double[(GeneralRunSettings.LastVariableNumber - GeneralRunSettings.VariableNumber + 1)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1)];

      // Выдача дочерним процессам всех заданий данных на выполнение программе
      for (int CurrentVarNumber=GeneralRunSettings.VariableNumber; CurrentVarNumber<=GeneralRunSettings.LastVariableNumber; CurrentVarNumber++)
      {
         for (int CurrentEnNumber=GeneralRunSettings.FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings.LastEnvironmentNumber; CurrentEnNumber++)
         {
            for (int CurrentTryNumber=GeneralRunSettings.FirstTryNumber; CurrentTryNumber<=GeneralRunSettings.LastTryNumber; CurrentTryNumber++)
            {
               // Если не всем процессам выданы изначальные задания
               if( ( (CurrentVarNumber - GeneralRunSettings.VariableNumber)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentTryNumber - GeneralRunSettings.FirstTryNumber + 1) ) <= (PrsSize-1))
               {
                  // Определяем номер процесса, которому мы должны послать задание
                  int PrRankSend = (CurrentVarNumber - GeneralRunSettings.VariableNumber)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentTryNumber - GeneralRunSettings.FirstTryNumber + 1);
                  // Составляем сообщение
                  sprintf(OutMessage,"%ie%.5fd%.5fc%.5fv%it%ip%ig%in%ir%if",CurrentEnNumber, MutationSettings.DisConProb, MutationSettings.EnConProb, EnVariableProbability, CurrentTryNumber,CurrentVarNumber, ModeSettings.EvolutionMode, ModeSettings.NetworkMode, ModeSettings.RewardMode, NoEnVariableCheck);
                  MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
                  // Записываем в лог выдачу задания
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Issued for pr.%i\n", CurrentEnNumber, CurrentTryNumber, PrRankSend);
               }
               // Если все процессы получили изначальные задания, то ждем завершения заданий и по ходу выдаем новые задания
               else
               {
                  // Ждем входяшее сообщение
                  MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, MPI_ANY_SOURCE, type, MPI_COMM_WORLD, &status);
                  char tmp_str[10]; // Строка, использующаяся для определения номера процесса
                  strcpy(tmp_str,"");

                  int PrRankSend; // Номер процесса для высылки задания
                  double _AverageReward, _MaxReward; // Данные о прогоне, которые прислал агент
                  int CurrentTry, CurrentEnvironment; // Идентификаторы задания, которое выполнял процесс
                  // Расшифровка сообщения
                  for (unsigned int i=0; i<strlen(InMessage); i++)
                  {
                     if (((InMessage[i]>='0') && (InMessage[i]<='9'))||(InMessage[i]=='.')) // если символ число или точка
                        sprintf(tmp_str,"%s%c",tmp_str,InMessage[i]);
                     else
                     {
                        switch (InMessage[i]) {
                           case 'e':
                              CurrentEnvironment = atoi(tmp_str);
                              break;
                           case 't':
                              CurrentTry = atoi(tmp_str);
                              break;
                           case 'o':
                              PrRankSend = atoi(tmp_str);
                              break;
                           case 'a':
                              _AverageReward = atof(tmp_str);
                              break;
                           case 'm':
                              _MaxReward = atof(tmp_str);
                              break;
                        }
                        strcpy(tmp_str,"");
                     }
                  }
                  // Записываем в лог прием задания
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Done from pr.%i\n", CurrentEnvironment, CurrentTry, PrRankSend);

                  //Записываем полученные данные анализа
                  AverageRewardArray[(CurrentEnvironment - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTry - GeneralRunSettings.FirstTryNumber] = _AverageReward;
                  MaxRewardArray[(CurrentEnvironment - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTry - GeneralRunSettings.FirstTryNumber] = _MaxReward;
                  // Составляем сообщение и высылаем задание
                  sprintf(OutMessage,"%ie%.5fd%.5fc%.5fv%it%ip%ig%in%ir%if",CurrentEnNumber, MutationSettings.DisConProb, MutationSettings.EnConProb, EnVariableProbability, CurrentTryNumber, CurrentVarNumber, ModeSettings.EvolutionMode, ModeSettings.NetworkMode, ModeSettings.RewardMode, NoEnVariableCheck);
                  MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
                  // Записываем в лог выдачу очередного задания
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Issued for pr.%i\n", CurrentEnNumber, CurrentTryNumber, PrRankSend);
               }
            }
         }
      }
      // Когда все задания закончились, ждем пока все они будут выполнены и по ходу посылаем всем процессам команду о завершении
      int PrQuit = PrsSize-1; // Количество процессов которые еще выполняются и необходимо дождаться их окончания
      while (PrQuit > 0) // Пока не завершаться все процессы
      {
         MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, MPI_ANY_SOURCE, type, MPI_COMM_WORLD, &status);
         char tmp_str[10]; // Строка, использующаяся для определения номера процесса
         strcpy(tmp_str,"");
         int PrRankSend; // Номер процесса для высылки задания
         double _AverageReward, _MaxReward; // Данные о прогоне, которые прислал агент
         int CurrentTry, CurrentEnvironment; // Идентификаторы задания, которое выполнял процесс
         // Расшифровка сообщения
         for (unsigned int i=0; i<strlen(InMessage); i++)
         {
            if (((InMessage[i]>='0') && (InMessage[i]<='9'))||(InMessage[i]=='.')) // если символ число или точка
               sprintf(tmp_str,"%s%c",tmp_str,InMessage[i]);
            else
            {
               switch (InMessage[i]) {
                  case 'e':
                     CurrentEnvironment = atoi(tmp_str);
                     break;
                  case 't':
                     CurrentTry = atoi(tmp_str);
                     break;
                  case 'o':
                     PrRankSend = atoi(tmp_str);
                     break;
                  case 'a':
                     _AverageReward = atof(tmp_str);
                     break;
                  case 'm':
                     _MaxReward = atof(tmp_str);
                     break;
               }
               strcpy(tmp_str,"");
            }
         }

         // Записываем в лог прием задания
         fprintf(hRunLogFile, "Environment: %i Try: %i - Done from pr.%i\n", CurrentEnvironment, CurrentTry, PrRankSend);

         //Записываем полученные данные анализа
         AverageRewardArray[(CurrentEnvironment - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTry - GeneralRunSettings.FirstTryNumber] = _AverageReward;
         MaxRewardArray[(CurrentEnvironment - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTry - GeneralRunSettings.FirstTryNumber] = _MaxReward;
         // Составляем сообщение о выходе и высылаем
         strcpy(OutMessage,"q");
         MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
         PrQuit--;
      }

      fclose(hRunLogFile);

      // Осуществляем запись общих результатов
      char NoEnVariableString[10]; // Признак того, что прогонялись агенты, обучавшиеся на стационарной среде
      if (NoEnVariableCheck)
         strcpy(NoEnVariableString, "_noenvar");
      else
         strcpy(NoEnVariableString, "");

      // Создаем файл с анализом
      char AnalysisFilename[255];
      sprintf(AnalysisFilename, "%sBestPopulationAnalysis/BestPopAnalysis_en%i-%i%s_%.4f_%i", DirectorySettings.ResultDirectory, GeneralRunSettings.FirstEnvironmentNumber, GeneralRunSettings.LastEnvironmentNumber, NoEnVariableString, EnVariableProbability, GeneralRunSettings.VariableNumber);
      if (ModeSettings.RewardMode)
         strcat(AnalysisFilename, "_pen");
      if (ModeSettings.EvolutionMode)
         strcat(AnalysisFilename, "_mod");
      strcat(AnalysisFilename, ".txt");

      FILE* hAnalysisFile = fopen(AnalysisFilename, "w");
      // Массив для хранения средних результатов по всем средам
      //double AverageEnvReward[GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber+1];
      double* AverageEnvReward = new double[GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber+1];

      for (int i=0; i<GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber+1; i++)
         AverageEnvReward[i] = 0;
      // Записываем полные результаты всех прогонов
      for (int CurrentEnNumber=GeneralRunSettings.FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings.LastEnvironmentNumber; CurrentEnNumber++)
      {
         // Загружаем среду и находим ее коэффициент сложности
         char EnvironmentFilename[255];
         GetEnvironmentFilename(EnvironmentFilename, DirectorySettings.EnvironmentDirectory, CurrentEnNumber);
         TEnvironmentAims EnvironmentAims;
         InitEnvironment(&EnvironmentAims, EnvironmentFilename);
         double EnFullCoefficient = CoefFullEnvironment(&EnvironmentAims);

         // Записываем все результаты
         for (int CurrentTryNumber = GeneralRunSettings.FirstTryNumber; CurrentTryNumber<=GeneralRunSettings.LastTryNumber; CurrentTryNumber++)
         {
            char tmp_str[255];
            // Записываем данные о среде в файл
            sprintf(tmp_str,"%i\t%i\t%.6f\t%.3f\t", CurrentEnNumber, EnvironmentAims.AimQuantity, EnFullCoefficient, 1.0/EnFullCoefficient);
            fprintf(hAnalysisFile, "%s", tmp_str);
            // Записываем данные прогона
            fprintf(hAnalysisFile,"%.2f\t", AverageRewardArray[(CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTryNumber - GeneralRunSettings.FirstTryNumber]);
            fprintf(hAnalysisFile,"%.2f\n", MaxRewardArray[(CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTryNumber - GeneralRunSettings.FirstTryNumber]);
            // Сразу же считаем среднее для каждой среды
            AverageEnvReward[CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber] += AverageRewardArray[(CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTryNumber - GeneralRunSettings.FirstTryNumber];
         }

         AverageEnvReward[CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber] = AverageEnvReward [CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber] / ((double) GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1);
      }
      // Записываем в файл усредненные результаты для всех сред
      fprintf(hAnalysisFile,"\n");
      for (int CurrentEnNumber = GeneralRunSettings.FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings.LastEnvironmentNumber; CurrentEnNumber++)
      {
         // Загружаем среду и находим ее коэффициент сложности
         char EnvironmentFilename[255];
         GetEnvironmentFilename(EnvironmentFilename, DirectorySettings.EnvironmentDirectory, CurrentEnNumber);
         TEnvironmentAims EnvironmentAims;
         InitEnvironment(&EnvironmentAims, EnvironmentFilename);
         double EnFullCoefficient = CoefFullEnvironment(&EnvironmentAims);

         // Записываем данные о среде и результаты усреднения
         fprintf(hAnalysisFile, "%i\t%i\t%.6f\t%.3f\n", CurrentEnNumber, EnvironmentAims.AimQuantity, EnFullCoefficient, AverageEnvReward[CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber]);
      }
      delete []AverageEnvReward;
      delete []AverageRewardArray;
      delete []MaxRewardArray;
      fclose(hAnalysisFile);
   }

   else // Если это один из рабочих процессов
   {
      MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status); //Ждем сообщения с заданием
      while (strcmp(InMessage,"q")) // Если не было команды о выходе
      {

            TRunSettings RunSettings; // Параметры текущего запуска

            //Декодируем сообщение с заданием
            DecodeTaskMessage(InMessage, &RunSettings, &ModeSettings, &(MutationSettings.DisConProb), &(MutationSettings.EnConProb), &EnVariableProbability, &NoEnVariableCheck);

            // Определяем ядро инициализации
            RunSettings.RandomizeRank = (unsigned) time(0) + PrRank; // К ядру инизиацлизации случайных чисел добавляется номер процесса, чтобы развести изначально инициализируемые процессы

            double AverageReward, MaxReward; //Данные анализа
            // Запускаем рабочую процедуру анализа запуска эволюции
            BestPopulationAnalysis(&AverageReward, &MaxReward, &RunSettings, &DirectorySettings, &TimeSettings, &ModeSettings, &LearningSettings, EnVariableProbability, NoEnVariableCheck);


            //Посылка ответа о завершении работы над заданием
            sprintf(OutMessage,"%ie%it%.2fa%.2fm%io",RunSettings.EnvironmentNumber, RunSettings.TryNumber, AverageReward, MaxReward, PrRank);
            MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD);

            //Ожидание нового задания
            MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
      }
   }
    MPI_Finalize();
}
