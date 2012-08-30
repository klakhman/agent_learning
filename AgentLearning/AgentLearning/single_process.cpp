#include <time.h>
#include <stdlib.h>
#include "parallel.h"
//#include "types.h"
#include "evolution.h"
#include "environment.h"
#include "agentlife.h"
#include "analysis.h"

#include "single_process.h"


void SerialTasks(int argc, char **argv)
{
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
   // ������� ��������� ��� ��������� �� ����� argv[1]
   FillSettingsFromFile("C:/Runs/settings_learning_adapt.ini", &TimeSettings, &MutationSettings, &ModeSettings, &DirectorySettings, &GeneralRunSettings, &PrimarySystemogenesisSettings, &LearningSettings, &EnVariableProbability);
   // ��������� ��������� �� ��������� ������ (��� ������ ��� ������������� ��������)
   //FillCommandParametrs(argc, argv, &GeneralRunSettings, &ModeSettings, &(MutationSettings.EnConProb), &(MutationSettings.DisConProb), &EnVariableProbability, &ProgramMode, &NoEnVariableCheck);
	printf("%.10f\n", EnVariableProbability);
   // ���������� ���� �������
   for (int CurrentVarNumber=GeneralRunSettings.VariableNumber; CurrentVarNumber<=GeneralRunSettings.LastVariableNumber; CurrentVarNumber++)
   {
      for (int CurrentEnNumber=GeneralRunSettings.FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings.LastEnvironmentNumber; CurrentEnNumber++)
      {
         for (int CurrentTryNumber=GeneralRunSettings.FirstTryNumber; CurrentTryNumber<=GeneralRunSettings.LastTryNumber; CurrentTryNumber++)
         {
				TRunSettings RunSettings; // ��������� �������� �������
				RunSettings.VariableNumber = CurrentVarNumber;
            RunSettings.EnvironmentNumber = CurrentEnNumber;
            RunSettings.TryNumber = CurrentTryNumber;
            // ���������� ���� �������������
            RunSettings.RandomizeRank = (unsigned) time(0);
             // ��������� ������� ��������� ��������
            PrepareEvolutionProcess(&RunSettings, &ModeSettings, &TimeSettings, &MutationSettings, &DirectorySettings, &PrimarySystemogenesisSettings, &LearningSettings,  EnVariableProbability);
         }
      }
   }
}

void SingleAnalysis(int argc, char **argv)
{
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
   // ��������� ��������� �� ��������� ������ (��� ������ ��� ������������� ��������)
   FillCommandParametrs(argc, argv, &GeneralRunSettings, &ModeSettings, &(MutationSettings.EnConProb), &(MutationSettings.DisConProb), &EnVariableProbability, &ProgramMode, &NoEnVariableCheck);
   // ���������� ���� �������
   for (int CurrentVarNumber=GeneralRunSettings.VariableNumber; CurrentVarNumber<=GeneralRunSettings.LastVariableNumber; CurrentVarNumber++)
   {
      for (int CurrentEnNumber=GeneralRunSettings.FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings.LastEnvironmentNumber; CurrentEnNumber++)
      {
         for (int CurrentTryNumber=GeneralRunSettings.FirstTryNumber; CurrentTryNumber<=GeneralRunSettings.LastTryNumber; CurrentTryNumber++)
         {

            TRunSettings RunSettings; // ��������� �������� �������
            RunSettings.VariableNumber = CurrentVarNumber;
            RunSettings.EnvironmentNumber = CurrentEnNumber;
            RunSettings.TryNumber = CurrentTryNumber;
            // ���������� ���� �������������
            RunSettings.RandomizeRank = (unsigned) time(0);

            double AverageReward, MaxReward;
             // ��������� ������� ��������� ��������
            BestPopulationAnalysis(&AverageReward, &MaxReward, &RunSettings, &DirectorySettings, &TimeSettings, &ModeSettings, &LearningSettings, EnVariableProbability, NoEnVariableCheck);

            printf("%.3f\t%.3f\n", AverageReward, MaxReward);
         }
      }
   }
}

/*void PrintLifeRecord(int argc, char **argv)
{
    // ������� ��������� � ���������� ��������
   TTimeSettings TimeSettings;
   TMutationSettings MutationSettings;
   TModeSettings ModeSettings;
   TDirectorySettings DirectorySettings;
   TGeneralRunSettings GeneralRunSettings;
   double EnVariableProbability = 0;
   // ������� ��������� ��� ��������� �� �����
   FillSettingsFromFile(argv[1], &TimeSettings, &MutationSettings, &ModeSettings, &DirectorySettings, &GeneralRunSettings, &EnVariableProbability);

   TEnvironmentAims EnvironmentAims;
   InitEnvironment(&EnvironmentAims, argv[2]);

   TNetwork AgentNetwork;
   AgentNetwork.Neurons = NULL;

   FILE*  hAgentLifeRecordFile = fopen(argv[5], "w");

   srand((unsigned) time(0)); // ������������� ���������� ��������� �����
   // ��� �������� ������� ���������� (����� �������� � ������� ��������)
   int tmp_rand = rand()%1000; // �������� ������ ���������� ��������� �����
   tmp_rand = rand()%1000; // �������� ������ ���������� ��������� �����
   tmp_rand = rand()%1000; // �������� ������ ���������� ��������� �����


   //FillSpecificAgent(&AgentNetwork, argv[3], atoi(argv[4]), EnvironmentAims.EnvironmentResolution, EnvironmentAims.OutputResolution, ModeSettings.NetworkMode);
   for (int i=0; i<atoi(argv[4]); i++)
   {
      FillSpecificAgent(&AgentNetwork, argv[3], i, EnvironmentAims.EnvironmentResolution, EnvironmentAims.OutputResolution, ModeSettings.NetworkMode);
      AgentLifeWithRecord(&AgentNetwork, &EnvironmentAims, TimeSettings.RewardRecoveryTime, EnVariableProbability, TimeSettings.AgentLifetime, ModeSettings.NetworkMode, hAgentLifeRecordFile);
   }
   fclose(hAgentLifeRecordFile);
}*/
