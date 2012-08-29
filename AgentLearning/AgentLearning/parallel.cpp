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

// ��������� ���������� ���������� �� ����� ��������
void FillSettingsFromFile(const char SettingsFilename[], TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TModeSettings* ModeSettings, TDirectorySettings* DirectorySettings, TGeneralRunSettings* GeneralRunSettings, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings, TLearningSettings* LearningSettings, double* EnVariablePorbability)
{
   FILE *hSettingsFile;
   char tmp_str[255];
   hSettingsFile = fopen(SettingsFilename,"r");

   while (fscanf(hSettingsFile,"%s", tmp_str) != EOF) // ������� ����� � ����������� ������������
   {
      // ��������� ���������
      if (!strcmp(tmp_str,"EvolutionTime")) { fscanf(hSettingsFile, "%s", tmp_str); TimeSettings->EvolutionTime = atoi(tmp_str); }
      if (!strcmp(tmp_str,"AgentLifetime")) { fscanf(hSettingsFile, "%s", tmp_str); TimeSettings->AgentLifetime = atoi(tmp_str); }
      if (!strcmp(tmp_str,"AgentPopulationQuantity")) { fscanf(hSettingsFile,"%s", tmp_str); TimeSettings->AgentPopulationQuantity = atoi(tmp_str); }
      if (!strcmp(tmp_str,"RewardRecoveryTime")) { fscanf(hSettingsFile, "%s", tmp_str); TimeSettings->RewardRecoveryTime = atoi(tmp_str); }

      // ��������� �������
      if (!strcmp(tmp_str,"NetworkMode")) { fscanf(hSettingsFile, "%s", tmp_str); ModeSettings->NetworkMode = atoi(tmp_str); }
      if (!strcmp(tmp_str,"EvolutionMode")) { fscanf(hSettingsFile, "%s", tmp_str); ModeSettings->EvolutionMode = atoi(tmp_str); }
      if (!strcmp(tmp_str,"RewardMode")) { fscanf(hSettingsFile, "%s", tmp_str); ModeSettings->RewardMode = atoi(tmp_str); }

      // ����� ��������� �������
      if (!strcmp(tmp_str,"FirstEnvironmentNumber")) { fscanf(hSettingsFile, "%s", tmp_str); GeneralRunSettings->FirstEnvironmentNumber = atoi(tmp_str); }
      if (!strcmp(tmp_str,"LastEnvironmentNumber")) { fscanf(hSettingsFile, "%s", tmp_str); GeneralRunSettings->LastEnvironmentNumber = atoi(tmp_str); }
      if (!strcmp(tmp_str,"FirstTryNumber")) { fscanf(hSettingsFile, "%s", tmp_str); GeneralRunSettings->FirstTryNumber = atoi(tmp_str); }
      if (!strcmp(tmp_str,"LastTryNumber")) { fscanf(hSettingsFile, "%s", tmp_str); GeneralRunSettings->LastTryNumber = atoi(tmp_str); }
      if (!strcmp(tmp_str,"VariableNumber")) { fscanf(hSettingsFile, "%s", tmp_str); GeneralRunSettings->VariableNumber = atoi(tmp_str); GeneralRunSettings->LastVariableNumber = atoi(tmp_str); }

      //������� ����������
      if (!strcmp(tmp_str,"EnvironmentDirectory")) { fscanf(hSettingsFile, "%s", tmp_str); strcpy(DirectorySettings->EnvironmentDirectory,tmp_str); }
      if (!strcmp(tmp_str,"ResultDirectory")) { fscanf(hSettingsFile, "%s", tmp_str); strcpy(DirectorySettings->ResultDirectory,tmp_str); }
      if (!strcmp(tmp_str,"WorkDirectory")) { fscanf(hSettingsFile, "%s", tmp_str); strcpy(DirectorySettings->WorkDirectory, tmp_str); }

      //��������� ����������� ���������
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

      // ��������� ���������� �������������
      if (!strcmp(tmp_str,"InitialPoolCapacity")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->InitialPoolCapacity = atoi(tmp_str); }
      if (!strcmp(tmp_str,"DevelopSynapseProbability")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->DevelopSynapseProbability = atof(tmp_str); }
      if (!strcmp(tmp_str,"DevelopPredConnectionProbability")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->DevelopPredConnectionProbability = atof(tmp_str); }
      if (!strcmp(tmp_str,"PrimarySystemogenesisTime")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->PrimarySystemogenesisTime = atoi(tmp_str); }
      if (!strcmp(tmp_str,"SpontaneousActivityProb")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->SpontaneousActivityProb = atof(tmp_str); }
      if (!strcmp(tmp_str,"ActiveNeuronsPercent")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->ActiveNeuronsPercent = atof(tmp_str); }
      if (!strcmp(tmp_str,"SynapsesActivityTreshold")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->SynapsesActivityTreshold = atof(tmp_str); }
      if (!strcmp(tmp_str,"SignificanceTreshold")) { fscanf(hSettingsFile, "%s", tmp_str); PrimarySystemogenesisSettings->SignificanceTreshold = atof(tmp_str); }

      // ��������� ��������
      if (!strcmp(tmp_str,"MismatchSignificanceTreshold")) { fscanf(hSettingsFile, "%s", tmp_str); LearningSettings->MismatchSignificanceTreshold = atof(tmp_str); }

      if (!strcmp(tmp_str,"EnVariablePorbability")) { fscanf(hSettingsFile, "%s", tmp_str); *EnVariablePorbability = atof(tmp_str); }
   }
}

//��������� ���������� ���������� �� ��������� ������
void FillCommandParametrs(int argc, char **argv, TGeneralRunSettings* GeneralRunSettings, TModeSettings* ModeSettings, double* EnConProb, double* DisConProb, double* EnVariableProbability, int* ProgramMode, bool* NoEnVariableCheck)
{
   int CurrentArgNumber = 2; // ������� ����� ��������� (� ������ ������� ���� � ����� ��������)
   while (CurrentArgNumber < argc) // ���� �� �� ������ ��� ���������
   {
      switch (argv[CurrentArgNumber][1]) // �������������� �������� � ��������� (�� ������� ����, ��� ��� � ������ "-")
      {
         case 'e': // ���� ��� ������ ������ � ��������� �����
            CurrentArgNumber++;
            GeneralRunSettings->FirstEnvironmentNumber = atoi(argv[CurrentArgNumber]);
            CurrentArgNumber++;
            GeneralRunSettings->LastEnvironmentNumber = atoi(argv[CurrentArgNumber]);
            break;
         case 't': // ���� ��� ������ �������
            CurrentArgNumber++;
            GeneralRunSettings->FirstTryNumber = atoi(argv[CurrentArgNumber]);
            CurrentArgNumber++;
            GeneralRunSettings->LastTryNumber = atoi(argv[CurrentArgNumber]);
            break;
         case 'v': // ���� ��� ����� ������ ����������
            CurrentArgNumber++;
            GeneralRunSettings->VariableNumber = atoi(argv[CurrentArgNumber]);
            GeneralRunSettings->LastVariableNumber = atoi(argv[CurrentArgNumber]);
            break;
         case 'c': // ���� ��� ����������� ��������� � ���������� ��������
            CurrentArgNumber++;
            *EnConProb = atof(argv[CurrentArgNumber]);
            CurrentArgNumber++;
            *DisConProb = atof(argv[CurrentArgNumber]);
            break;
         case 'p': // ���� ��� ������� ������������������� �����
            CurrentArgNumber++;
            *EnVariableProbability = atof(argv[CurrentArgNumber]);
            break;
         case 'x': // ���� ��� ����� ������������� ���������
            CurrentArgNumber++;
            ModeSettings->EvolutionMode = atoi(argv[CurrentArgNumber]);
            break;
         case 'y': // ���� ��� ����� ������� ���������
            CurrentArgNumber++;
            ModeSettings->NetworkMode = atoi(argv[CurrentArgNumber]);
            break;
         case 'z': // ���� ��� ����� ���������� �������� �������
            CurrentArgNumber++;
            ModeSettings->RewardMode = atoi(argv[CurrentArgNumber]);
            break;
         case 'm': // ���� ��� ����� ������ ���������
            CurrentArgNumber++;
            *ProgramMode = atoi(argv[CurrentArgNumber]);
            break;
         case 'f': // ���� ��� ������� �������������� ����� (��� �������)
            CurrentArgNumber++;
            *NoEnVariableCheck = atoi(argv[CurrentArgNumber]);
            break;
      }
      CurrentArgNumber++;
   }
}

// ��������� ������������� ��������� � �������� ��� �������� ��������
void DecodeTaskMessage(char* TaskMessage, TRunSettings* RunSettings, TModeSettings* ModeSettings, double* DisConProb, double* EnConProb, double* EnVariablePorbability, bool* NoEnVariableCheck)
{
   char tmp_str[10]; // ����������, � ������� ������������ ������� �������� (�����)
   strcpy(tmp_str,"");

   for (unsigned int i=0;i<strlen(TaskMessage);i++) // ���������� �� ����� ���������
   {
      // � ������ ��������� ���������� ����� � ��������� ����������
      if (((TaskMessage[i]>='0') && (TaskMessage[i]<='9')) || TaskMessage[i]=='.') // ���� ������ �����
         sprintf(tmp_str, "%s%c", tmp_str, TaskMessage[i]);
      else // ���� ��� ��� �������� �� �������������� ���������
      {
         switch (TaskMessage[i]) // ���������, ����� � ��� ������� ��������
         {
            case 'e': // ���� ��� ����� �����
                     RunSettings->EnvironmentNumber = atoi(tmp_str);
                     break;
            case 'd': // ���� ��� ����������� ���������� �����
                     *DisConProb = atof(tmp_str);
                     break;
            case 'c': // ���� ��� ����������� ��������� �����
                     *EnConProb = atof(tmp_str);
                     break;
            case 'v': // ���� ��� ������� ������������������� �����
                     *EnVariablePorbability = atof(tmp_str);
                     break;
            case 't': // ���� ��� ����� �������
                     RunSettings->TryNumber = atoi(tmp_str);
                     break;
            case 'p': // ���� ��� ����� ������ ����������
                     RunSettings->VariableNumber = atoi(tmp_str);
                     break;
            case 'g': // ���� ��� ����� ������������� ���������
                     ModeSettings->EvolutionMode = atoi(tmp_str);
                     break;
            case 'n': // ���� ��� ����� ������� ���������
                     ModeSettings->NetworkMode = atoi(tmp_str);
                     break;
            case 'r': // ���� ��� ����� ���������� �������
                     ModeSettings->RewardMode = atoi(tmp_str);
                     break;
            case 'f': // ���� ��� ������� ����, ��� �������������� ������ ���������, ������� ���������������� �� ������������ �����
                     *NoEnVariableCheck = atoi(tmp_str);
                     break;
         }

         strcpy(tmp_str,"");
      }
   }
}

// �������� ��������� ����������������� - ������ � ���������� �������
void ParallelTasks(int argc, char **argv)
{
   MPI_Init(&argc, &argv);
   //------- �������� ���������/��������� ��������� --------
   char InMessage[MessageLenght]; // �������� ��������� ��� ��������
   char OutMessage[MessageLenght]; // ��������� ��������� ��� ��������
   int PrRank; // ����� ��������
   int PrsSize; // ����� ���-�� ���������
   int type = 99; // ��������� ��� ���������
   MPI_Status status;

   MPI_Comm_size(MPI_COMM_WORLD, &PrsSize); // ����������� ������ ���������� ���������
   MPI_Comm_rank(MPI_COMM_WORLD, &PrRank); // ����������� ��������� ������ ������

   // ������� ��������� � ���������� ��������
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

   // ������� ��������� ��� ��������� �� �����
   FillSettingsFromFile(argv[1], &TimeSettings, &MutationSettings, &ModeSettings, &DirectorySettings, &GeneralRunSettings, &PrimarySystemogenesisSettings, &LearningSettings, &EnVariableProbability);

   if (PrRank == 0) // ���� ��� ������������ �������
   {
      // ��������� ��������� �� ��������� ������ (��� ������ ��� ������������� ��������)
      FillCommandParametrs(argc, argv, &GeneralRunSettings, &ModeSettings, &(MutationSettings.EnConProb), &(MutationSettings.DisConProb), &EnVariableProbability, &ProgramMode, &NoEnVariableCheck);

      char RunLogFilename[255];
      sprintf(RunLogFilename, "%s/EvolutionRunLog_En%i-%i_Var%i_EnVar%.5f_EvM%i.txt", DirectorySettings.WorkDirectory, GeneralRunSettings.FirstEnvironmentNumber, GeneralRunSettings.LastEnvironmentNumber, GeneralRunSettings.VariableNumber, EnVariableProbability, ModeSettings.EvolutionMode);
      FILE* hRunLogFile = fopen(RunLogFilename, "w");

      // ������ �������� ��������� ���� ������� ������ �� ���������� ���������
      for (int CurrentVarNumber=GeneralRunSettings.VariableNumber; CurrentVarNumber<=GeneralRunSettings.LastVariableNumber; CurrentVarNumber++)
      {
         for (int CurrentEnNumber=GeneralRunSettings.FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings.LastEnvironmentNumber; CurrentEnNumber++)
         {
            for (int CurrentTryNumber=GeneralRunSettings.FirstTryNumber; CurrentTryNumber<=GeneralRunSettings.LastTryNumber; CurrentTryNumber++)
            {
               // ���� �� ���� ��������� ������ ����������� �������
               if( ( (CurrentVarNumber - GeneralRunSettings.VariableNumber)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentTryNumber - GeneralRunSettings.FirstTryNumber + 1) ) <= (PrsSize-1))
               {
                  // ���������� ����� ��������, �������� �� ������ ������� �������
                  int PrRankSend = (CurrentVarNumber - GeneralRunSettings.VariableNumber)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentTryNumber - GeneralRunSettings.FirstTryNumber + 1);
                  // ���������� ���������
                  sprintf(OutMessage,"%ie%.5fd%.5fc%.5fv%it%ip%ig%in%ir",CurrentEnNumber, MutationSettings.DisConProb, MutationSettings.EnConProb, EnVariableProbability, CurrentTryNumber,CurrentVarNumber, ModeSettings.EvolutionMode, ModeSettings.NetworkMode, ModeSettings.RewardMode);
                  MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
                   // ���������� � ��� ������ �������
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Issued for pr.%i\n", CurrentEnNumber, CurrentTryNumber, PrRankSend);
               }
               // ���� ��� �������� �������� ����������� �������, �� ���� ���������� ������� � �� ���� ������ ����� �������
               else
               {
                  // ���� �������� ���������
                  MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, MPI_ANY_SOURCE, type, MPI_COMM_WORLD, &status);
                  char tmp_str[10]; // ������, �������������� ��� ����������� ������ ��������
                  strcpy(tmp_str,"");
                  int PrRankSend; // ����� �������� ��� ������� �������
                  int CurrentTry, CurrentEnvironment; // �������������� �������, ������� �������� �������
                  // ����������� ���������
                  for (unsigned int i=0; i<strlen(InMessage); i++)
                  {
                     if (((InMessage[i]>='0') && (InMessage[i]<='9'))||(InMessage[i]=='.')) // ���� ������ ����� ��� �����
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
                  // ���������� � ��� ����� �������
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Done from pr.%i\n", CurrentEnvironment, CurrentTry, PrRankSend);
                  // ���������� ��������� � �������� �������
                  sprintf(OutMessage,"%ie%.5fd%.5fc%.5fv%it%ip%ig%in%ir",CurrentEnNumber, MutationSettings.DisConProb, MutationSettings.EnConProb, EnVariableProbability, CurrentTryNumber,CurrentVarNumber, ModeSettings.EvolutionMode, ModeSettings.NetworkMode, ModeSettings.RewardMode);
                  MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
                  // ���������� � ��� ������ �������
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Issued for pr.%i\n", CurrentEnNumber, CurrentTryNumber, PrRankSend);
               }
            }
         }
      }
      // ����� ��� ������� �����������, ���� ���� ��� ��� ����� ��������� � �� ���� �������� ���� ��������� ������� � ����������
      int PrQuit = PrsSize-1; // ���������� ��������� ������� ��� ����������� � ���������� ��������� �� ���������
      while (PrQuit > 0) // ���� �� ����������� ��� ��������
      {
         MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, MPI_ANY_SOURCE, type, MPI_COMM_WORLD, &status);
         char tmp_str[10]; // ������, �������������� ��� ����������� ������ ��������
         strcpy(tmp_str,"");

         int PrRankSend; // ����� �������� ��� ������� �������
         int CurrentTry, CurrentEnvironment; // �������������� �������, ������� �������� �������
         // ����������� ���������
         for (unsigned int i=0; i<strlen(InMessage); i++)
         {
            if (((InMessage[i]>='0') && (InMessage[i]<='9'))||(InMessage[i]=='.')) // ���� ������ ����� ��� �����
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
         // ���������� � ��� ����� �������
         fprintf(hRunLogFile, "Environment: %i Try: %i - Done from pr.%i\n", CurrentEnvironment, CurrentTry, PrRankSend);

         // ���������� ��������� � ������ � ��������
         strcpy(OutMessage,"q");
         MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
         PrQuit--;
      }
      fclose(hRunLogFile);
   }

   else // ���� ��� ���� �� ������� ���������
   {
      MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status); //���� ��������� � ��������
      while (strcmp(InMessage,"q")) // ���� �� ���� ������� � ������
      {
            TRunSettings RunSettings; // ��������� �������� �������

            //���������� ��������� � ��������
            DecodeTaskMessage(InMessage, &RunSettings, &ModeSettings, &(MutationSettings.DisConProb), &(MutationSettings.EnConProb), &EnVariableProbability, NULL);

            // ���������� ���� �������������
            RunSettings.RandomizeRank = (unsigned) time(0) + PrRank; // � ���� �������������� ��������� ����� ����������� ����� ��������, ����� �������� ���������� ���������������� ��������

            // ��������� ������� ��������� ��������
            PrepareEvolutionProcess(&RunSettings, &ModeSettings, &TimeSettings, &MutationSettings, &DirectorySettings, &PrimarySystemogenesisSettings, &LearningSettings, EnVariableProbability);

            //������� ������ � ���������� ������ ��� ��������
            sprintf(OutMessage,"%ie%it%io",RunSettings.EnvironmentNumber, RunSettings.TryNumber, PrRank);
            MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD);

            //�������� ������ �������
            MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
      }
   }
    MPI_Finalize();
}

// �������� ��������� ����������������� ������� ��������
void ParallelAnalysis(int argc, char **argv)
{
   MPI_Init(&argc, &argv);

   //------- �������� ���������/��������� ��������� --------
   char InMessage[MessageLenght]; // �������� ��������� ��� ��������
   char OutMessage[MessageLenght]; // ��������� ��������� ��� ��������
   int PrRank; // ����� ��������
   int PrsSize; // ����� ���-�� ���������
   int type = 99; // ��������� ��� ���������
   MPI_Status status;

   MPI_Comm_size(MPI_COMM_WORLD, &PrsSize); // ����������� ������ ���������� ���������
   MPI_Comm_rank(MPI_COMM_WORLD, &PrRank); // ����������� ��������� ������ ������

   // ������� ��������� � ���������� ��������
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
   // ������� ��������� ��� ��������� �� �����
   FillSettingsFromFile(argv[1], &TimeSettings, &MutationSettings, &ModeSettings, &DirectorySettings, &GeneralRunSettings, &PrimarySystemogenesisSettings, &LearningSettings, &EnVariableProbability);

   if (PrRank == 0) // ���� ��� ������������ �������
   {
      // ��������� ��������� �� ��������� ������ (��� ������ ��� ������������� ��������)
      FillCommandParametrs(argc, argv, &GeneralRunSettings, &ModeSettings, &(MutationSettings.EnConProb), &(MutationSettings.DisConProb), &EnVariableProbability, &ProgramMode, &NoEnVariableCheck);

      // �������� ����������� ������ ����� ��������
      CheckIntegrityOfData(&GeneralRunSettings, &ModeSettings, &DirectorySettings, NoEnVariableCheck, TimeSettings.AgentPopulationQuantity);

      char RunLogFilename[255];
      sprintf(RunLogFilename, "%s/AnalysisRunLog_En%i-%i_Var%i_Noen%i_EnVar%.5f_EvM%i.txt", DirectorySettings.WorkDirectory, GeneralRunSettings.FirstEnvironmentNumber, GeneralRunSettings.LastEnvironmentNumber, GeneralRunSettings.VariableNumber, NoEnVariableCheck, EnVariableProbability, ModeSettings.EvolutionMode);
      FILE* hRunLogFile = fopen(RunLogFilename, "w");

      // ������� ������� � ������������ ������ ��� ���� ������������� �������� (��� ���� ���� � ��� ���� �������)
      //double AverageRewardArray[(GeneralRunSettings.LastVariableNumber - GeneralRunSettings.VariableNumber + 1)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1)];
      //double MaxRewardArray[(GeneralRunSettings.LastVariableNumber - GeneralRunSettings.VariableNumber + 1)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1)];
      double* AverageRewardArray = new double[(GeneralRunSettings.LastVariableNumber - GeneralRunSettings.VariableNumber + 1)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1)];
      double* MaxRewardArray = new double[(GeneralRunSettings.LastVariableNumber - GeneralRunSettings.VariableNumber + 1)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1)];

      // ������ �������� ��������� ���� ������� ������ �� ���������� ���������
      for (int CurrentVarNumber=GeneralRunSettings.VariableNumber; CurrentVarNumber<=GeneralRunSettings.LastVariableNumber; CurrentVarNumber++)
      {
         for (int CurrentEnNumber=GeneralRunSettings.FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings.LastEnvironmentNumber; CurrentEnNumber++)
         {
            for (int CurrentTryNumber=GeneralRunSettings.FirstTryNumber; CurrentTryNumber<=GeneralRunSettings.LastTryNumber; CurrentTryNumber++)
            {
               // ���� �� ���� ��������� ������ ����������� �������
               if( ( (CurrentVarNumber - GeneralRunSettings.VariableNumber)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentTryNumber - GeneralRunSettings.FirstTryNumber + 1) ) <= (PrsSize-1))
               {
                  // ���������� ����� ��������, �������� �� ������ ������� �������
                  int PrRankSend = (CurrentVarNumber - GeneralRunSettings.VariableNumber)*(GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber + 1)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1) + (CurrentTryNumber - GeneralRunSettings.FirstTryNumber + 1);
                  // ���������� ���������
                  sprintf(OutMessage,"%ie%.5fd%.5fc%.5fv%it%ip%ig%in%ir%if",CurrentEnNumber, MutationSettings.DisConProb, MutationSettings.EnConProb, EnVariableProbability, CurrentTryNumber,CurrentVarNumber, ModeSettings.EvolutionMode, ModeSettings.NetworkMode, ModeSettings.RewardMode, NoEnVariableCheck);
                  MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
                  // ���������� � ��� ������ �������
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Issued for pr.%i\n", CurrentEnNumber, CurrentTryNumber, PrRankSend);
               }
               // ���� ��� �������� �������� ����������� �������, �� ���� ���������� ������� � �� ���� ������ ����� �������
               else
               {
                  // ���� �������� ���������
                  MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, MPI_ANY_SOURCE, type, MPI_COMM_WORLD, &status);
                  char tmp_str[10]; // ������, �������������� ��� ����������� ������ ��������
                  strcpy(tmp_str,"");

                  int PrRankSend; // ����� �������� ��� ������� �������
                  double _AverageReward, _MaxReward; // ������ � �������, ������� ������� �����
                  int CurrentTry, CurrentEnvironment; // �������������� �������, ������� �������� �������
                  // ����������� ���������
                  for (unsigned int i=0; i<strlen(InMessage); i++)
                  {
                     if (((InMessage[i]>='0') && (InMessage[i]<='9'))||(InMessage[i]=='.')) // ���� ������ ����� ��� �����
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
                  // ���������� � ��� ����� �������
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Done from pr.%i\n", CurrentEnvironment, CurrentTry, PrRankSend);

                  //���������� ���������� ������ �������
                  AverageRewardArray[(CurrentEnvironment - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTry - GeneralRunSettings.FirstTryNumber] = _AverageReward;
                  MaxRewardArray[(CurrentEnvironment - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTry - GeneralRunSettings.FirstTryNumber] = _MaxReward;
                  // ���������� ��������� � �������� �������
                  sprintf(OutMessage,"%ie%.5fd%.5fc%.5fv%it%ip%ig%in%ir%if",CurrentEnNumber, MutationSettings.DisConProb, MutationSettings.EnConProb, EnVariableProbability, CurrentTryNumber, CurrentVarNumber, ModeSettings.EvolutionMode, ModeSettings.NetworkMode, ModeSettings.RewardMode, NoEnVariableCheck);
                  MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
                  // ���������� � ��� ������ ���������� �������
                  fprintf(hRunLogFile, "Environment: %i Try: %i - Issued for pr.%i\n", CurrentEnNumber, CurrentTryNumber, PrRankSend);
               }
            }
         }
      }
      // ����� ��� ������� �����������, ���� ���� ��� ��� ����� ��������� � �� ���� �������� ���� ��������� ������� � ����������
      int PrQuit = PrsSize-1; // ���������� ��������� ������� ��� ����������� � ���������� ��������� �� ���������
      while (PrQuit > 0) // ���� �� ����������� ��� ��������
      {
         MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, MPI_ANY_SOURCE, type, MPI_COMM_WORLD, &status);
         char tmp_str[10]; // ������, �������������� ��� ����������� ������ ��������
         strcpy(tmp_str,"");
         int PrRankSend; // ����� �������� ��� ������� �������
         double _AverageReward, _MaxReward; // ������ � �������, ������� ������� �����
         int CurrentTry, CurrentEnvironment; // �������������� �������, ������� �������� �������
         // ����������� ���������
         for (unsigned int i=0; i<strlen(InMessage); i++)
         {
            if (((InMessage[i]>='0') && (InMessage[i]<='9'))||(InMessage[i]=='.')) // ���� ������ ����� ��� �����
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

         // ���������� � ��� ����� �������
         fprintf(hRunLogFile, "Environment: %i Try: %i - Done from pr.%i\n", CurrentEnvironment, CurrentTry, PrRankSend);

         //���������� ���������� ������ �������
         AverageRewardArray[(CurrentEnvironment - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTry - GeneralRunSettings.FirstTryNumber] = _AverageReward;
         MaxRewardArray[(CurrentEnvironment - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTry - GeneralRunSettings.FirstTryNumber] = _MaxReward;
         // ���������� ��������� � ������ � ��������
         strcpy(OutMessage,"q");
         MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, PrRankSend, type, MPI_COMM_WORLD);
         PrQuit--;
      }

      fclose(hRunLogFile);

      // ������������ ������ ����� �����������
      char NoEnVariableString[10]; // ������� ����, ��� ����������� ������, ����������� �� ������������ �����
      if (NoEnVariableCheck)
         strcpy(NoEnVariableString, "_noenvar");
      else
         strcpy(NoEnVariableString, "");

      // ������� ���� � ��������
      char AnalysisFilename[255];
      sprintf(AnalysisFilename, "%sBestPopulationAnalysis/BestPopAnalysis_en%i-%i%s_%.4f_%i", DirectorySettings.ResultDirectory, GeneralRunSettings.FirstEnvironmentNumber, GeneralRunSettings.LastEnvironmentNumber, NoEnVariableString, EnVariableProbability, GeneralRunSettings.VariableNumber);
      if (ModeSettings.RewardMode)
         strcat(AnalysisFilename, "_pen");
      if (ModeSettings.EvolutionMode)
         strcat(AnalysisFilename, "_mod");
      strcat(AnalysisFilename, ".txt");

      FILE* hAnalysisFile = fopen(AnalysisFilename, "w");
      // ������ ��� �������� ������� ����������� �� ���� ������
      //double AverageEnvReward[GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber+1];
      double* AverageEnvReward = new double[GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber+1];

      for (int i=0; i<GeneralRunSettings.LastEnvironmentNumber - GeneralRunSettings.FirstEnvironmentNumber+1; i++)
         AverageEnvReward[i] = 0;
      // ���������� ������ ���������� ���� ��������
      for (int CurrentEnNumber=GeneralRunSettings.FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings.LastEnvironmentNumber; CurrentEnNumber++)
      {
         // ��������� ����� � ������� �� ����������� ���������
         char EnvironmentFilename[255];
         GetEnvironmentFilename(EnvironmentFilename, DirectorySettings.EnvironmentDirectory, CurrentEnNumber);
         TEnvironmentAims EnvironmentAims;
         InitEnvironment(&EnvironmentAims, EnvironmentFilename);
         double EnFullCoefficient = CoefFullEnvironment(&EnvironmentAims);

         // ���������� ��� ����������
         for (int CurrentTryNumber = GeneralRunSettings.FirstTryNumber; CurrentTryNumber<=GeneralRunSettings.LastTryNumber; CurrentTryNumber++)
         {
            char tmp_str[255];
            // ���������� ������ � ����� � ����
            sprintf(tmp_str,"%i\t%i\t%.6f\t%.3f\t", CurrentEnNumber, EnvironmentAims.AimQuantity, EnFullCoefficient, 1.0/EnFullCoefficient);
            fprintf(hAnalysisFile, "%s", tmp_str);
            // ���������� ������ �������
            fprintf(hAnalysisFile,"%.2f\t", AverageRewardArray[(CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTryNumber - GeneralRunSettings.FirstTryNumber]);
            fprintf(hAnalysisFile,"%.2f\n", MaxRewardArray[(CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTryNumber - GeneralRunSettings.FirstTryNumber]);
            // ����� �� ������� ������� ��� ������ �����
            AverageEnvReward[CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber] += AverageRewardArray[(CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber)*(GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber+1) + CurrentTryNumber - GeneralRunSettings.FirstTryNumber];
         }

         AverageEnvReward[CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber] = AverageEnvReward [CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber] / ((double) GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1);
      }
      // ���������� � ���� ����������� ���������� ��� ���� ����
      fprintf(hAnalysisFile,"\n");
      for (int CurrentEnNumber = GeneralRunSettings.FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings.LastEnvironmentNumber; CurrentEnNumber++)
      {
         // ��������� ����� � ������� �� ����������� ���������
         char EnvironmentFilename[255];
         GetEnvironmentFilename(EnvironmentFilename, DirectorySettings.EnvironmentDirectory, CurrentEnNumber);
         TEnvironmentAims EnvironmentAims;
         InitEnvironment(&EnvironmentAims, EnvironmentFilename);
         double EnFullCoefficient = CoefFullEnvironment(&EnvironmentAims);

         // ���������� ������ � ����� � ���������� ����������
         fprintf(hAnalysisFile, "%i\t%i\t%.6f\t%.3f\n", CurrentEnNumber, EnvironmentAims.AimQuantity, EnFullCoefficient, AverageEnvReward[CurrentEnNumber - GeneralRunSettings.FirstEnvironmentNumber]);
      }
      delete []AverageEnvReward;
      delete []AverageRewardArray;
      delete []MaxRewardArray;
      fclose(hAnalysisFile);
   }

   else // ���� ��� ���� �� ������� ���������
   {
      MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status); //���� ��������� � ��������
      while (strcmp(InMessage,"q")) // ���� �� ���� ������� � ������
      {

            TRunSettings RunSettings; // ��������� �������� �������

            //���������� ��������� � ��������
            DecodeTaskMessage(InMessage, &RunSettings, &ModeSettings, &(MutationSettings.DisConProb), &(MutationSettings.EnConProb), &EnVariableProbability, &NoEnVariableCheck);

            // ���������� ���� �������������
            RunSettings.RandomizeRank = (unsigned) time(0) + PrRank; // � ���� �������������� ��������� ����� ����������� ����� ��������, ����� �������� ���������� ���������������� ��������

            double AverageReward, MaxReward; //������ �������
            // ��������� ������� ��������� ������� ������� ��������
            BestPopulationAnalysis(&AverageReward, &MaxReward, &RunSettings, &DirectorySettings, &TimeSettings, &ModeSettings, &LearningSettings, EnVariableProbability, NoEnVariableCheck);


            //������� ������ � ���������� ������ ��� ��������
            sprintf(OutMessage,"%ie%it%.2fa%.2fm%io",RunSettings.EnvironmentNumber, RunSettings.TryNumber, AverageReward, MaxReward, PrRank);
            MPI_Send(OutMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD);

            //�������� ������ �������
            MPI_Recv(InMessage, MessageLenght-1, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
      }
   }
    MPI_Finalize();
}
