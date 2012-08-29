#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"
#include "service.h"
#include "service_network.h"
#include "environment.h"
#include "agentlife.h"
#include "mutation.h"
#include "primary_systemogenesis.h"

#include "evolution.h"

extern const int HistogramCount;

// ��������� ��������� ��������� ��������� ��� ������������������ ������������ ���������
void InitFirstGeneration_ModernEvolution(TAgentGenomePopulation* AgentGenomePopulation, int EnvironmentResolution, int OutputResolution, int InitialPoolCapacity)
{
   for (int CurrentAgent=0; CurrentAgent<AgentGenomePopulation->PopulationAgentQuantity; CurrentAgent++) // ������� ���������
   {
      AgentGenomePopulation->AgentGenome[CurrentAgent] = CreatePoolNetwork();
      TNeuralPool* CurrentPool=NULL;
      int CurrentPoolNumber = 0;
      for (CurrentPoolNumber=0; CurrentPoolNumber < EnvironmentResolution; ++CurrentPoolNumber) // ������� ��������� ����
         if (CurrentPool == NULL) // ���� ��� ������ ���
         {

            AgentGenomePopulation->AgentGenome[CurrentAgent]->PoolsStructure = CreateNeuralPool(CurrentPoolNumber + 1, 0, 1, UniformDistribution(-1,1), 0, 1, NULL, NULL, NULL);
            CurrentPool = AgentGenomePopulation->AgentGenome[CurrentAgent]->PoolsStructure;
         }
         else
         {
            CurrentPool->next = CreateNeuralPool(CurrentPoolNumber + 1, 0, 1, UniformDistribution(-1,1), 0, 1, NULL, NULL, NULL);
            CurrentPool = CurrentPool->next;
         }

      // ������� �������� ����
      for (CurrentPoolNumber=EnvironmentResolution; CurrentPoolNumber<EnvironmentResolution+OutputResolution; ++CurrentPoolNumber)
      {
         CurrentPool->next = CreateNeuralPool(CurrentPoolNumber + 1, 2, 1, UniformDistribution(-1,1), 0, 3, NULL, NULL, NULL);
         CurrentPool = CurrentPool->next;
      }

      // ������� ������������ ���������� ���
      CurrentPool->next = CreateNeuralPool(CurrentPoolNumber + 1, 1, InitialPoolCapacity, UniformDistribution(-1,1), 0, 2, NULL, NULL, NULL);
      CurrentPool = CurrentPool->next;

      TNeuralPool* CurrentPool_2 = AgentGenomePopulation->AgentGenome[CurrentAgent]->PoolsStructure;
      while (CurrentPool_2->Type != 2) // ������� ������ ���������� ������
         CurrentPool_2 = CurrentPool_2->next;

      // ��������� �������� ����� �������� ����� �� ����������� ����
      for (int CurrentConnectionNumber=0; CurrentConnectionNumber<OutputResolution; ++CurrentConnectionNumber)
      {
         CurrentPool_2->ConnectednessSet = CreatePoolConnection(CurrentConnectionNumber + 1, UniformDistribution(-1,1), 0, 1, static_cast<double>(CurrentConnectionNumber + 1), 0, CurrentPool, CurrentPool_2, NULL);
         CurrentPool_2 = CurrentPool_2->next;
      }

      CurrentPool_2 = AgentGenomePopulation->AgentGenome[CurrentAgent]->PoolsStructure; // ������ �� ������ ��������� ���
      // ��������� �������� ����� ����������� ���� �� ������� �����
      TPoolConnection* CurrentConnection = NULL;
      for (int CurrentConnectionNumber=OutputResolution; CurrentConnectionNumber<OutputResolution+EnvironmentResolution; ++CurrentConnectionNumber)
         if (CurrentConnection == NULL) // ���� ��� ������ �������� ����� � ����������� ����
         {
            CurrentPool->ConnectednessSet = CreatePoolConnection(CurrentConnectionNumber + 1, UniformDistribution(-1,1), 0, 1, static_cast<double>(CurrentConnectionNumber + 1), 0, CurrentPool_2, CurrentPool, NULL);
            CurrentConnection = CurrentPool->ConnectednessSet;
            CurrentPool_2 = CurrentPool_2->next; // ���������� �� ��������� ��������� ���
         }
         else
         {
            CurrentConnection->next = CreatePoolConnection(CurrentConnectionNumber + 1, UniformDistribution(-1,1), 0, 1, static_cast<double>(CurrentConnectionNumber + 1), 0, CurrentPool_2, CurrentPool, NULL);
            CurrentConnection = CurrentConnection->next;
            CurrentPool_2 = CurrentPool_2->next; // ���������� �� ��������� ��������� ���
         }
      AgentGenomePopulation->AgentGenome[CurrentAgent]->PoolQuantity = EnvironmentResolution + OutputResolution + 1;
      AgentGenomePopulation->AgentGenome[CurrentAgent]->ConnectionQuantity = EnvironmentResolution + OutputResolution;
      AgentGenomePopulation->AgentGenome[CurrentAgent]->PredConnectionQuantity = 0;
   }
}

/*// ��������� ��������� ��������� ��������� ��� NEAT
void InitFirstGeneration_NEAT(TPopulation* AgentPopulation, int EnvironmentResolution, int OutputResolution)
{
   for (int CurrentAgent=0; CurrentAgent<AgentPopulation->PopulationAgentQuantity; CurrentAgent++) // ������� ���������
   {
      TNeuron* CurrentNeuron=NULL;
      for (int CurrentNeuronNumber=0; CurrentNeuronNumber<EnvironmentResolution; CurrentNeuronNumber++) // ������� ��������� �������
         if (CurrentNeuron == NULL) // ���� ��� ������ ������
         {
            AgentPopulation->Agent[CurrentAgent].Neurons = CreateNeuron(CurrentNeuronNumber + 1, 0, ((rand()%2001)-1000)/1000.0, 1, NULL, NULL);
            CurrentNeuron = AgentPopulation->Agent[CurrentAgent].Neurons;
         }
         else
         {
            CurrentNeuron->next = CreateNeuron(CurrentNeuronNumber + 1, 0, ((rand()%2001)-1000)/1000.0, 1, NULL, NULL);
            CurrentNeuron = CurrentNeuron->next;
         }
      // ������� �������� �������
      for (int CurrentNeuronNumber=EnvironmentResolution; CurrentNeuronNumber<EnvironmentResolution+OutputResolution; CurrentNeuronNumber++)
      {
         CurrentNeuron->next = CreateNeuron(CurrentNeuronNumber + 1, 2, ((rand()%2001)-1000)/1000.0, 2, NULL, NULL);
         CurrentNeuron = CurrentNeuron->next;
      }

      CurrentNeuron = AgentPopulation->Agent[CurrentAgent].Neurons;
      while (CurrentNeuron->type != 2) // ������� �� �������� ��������
         CurrentNeuron = CurrentNeuron->next;

      int CurrentSynapseNumber = 0; //����� �������� �������
      while (CurrentNeuron != NULL) // ���������� �� ���� �������� ��������
      {
         TSynapse* CurrentSynapse = NULL;
         TNeuron* CurrentNeuron_2 = AgentPopulation->Agent[CurrentAgent].Neurons; // ������ �� ������ ������� ������

         for (int i=0; i<EnvironmentResolution; i++) // ���������� �� ���� ������� �������� � ������� �������
            if (CurrentSynapse == NULL) // ���� ��� ������ �������� ������ � ��������� �������
            {
               CurrentNeuron->InSynapses = CreateSynapse(CurrentSynapseNumber + 1, ((rand()%2001)-1000)/1000.0, 1, (double) CurrentSynapseNumber + 1, 0, CurrentNeuron_2, CurrentNeuron, NULL);
               CurrentSynapse = CurrentNeuron->InSynapses;
               CurrentNeuron_2 = CurrentNeuron_2->next; // ���������� �� ��������� ��������� ������
               CurrentSynapseNumber++;
            }
            else
            {
               CurrentSynapse->next = CreateSynapse(CurrentSynapseNumber + 1, ((rand()%2001)-1000)/1000.0, 1, (double) CurrentSynapseNumber + 1, 0, CurrentNeuron_2, CurrentNeuron, NULL);
               CurrentSynapse = CurrentSynapse->next;
               CurrentNeuron_2 = CurrentNeuron_2->next; // ���������� �� ��������� ��������� ������
               CurrentSynapseNumber++;
            }
         CurrentNeuron = CurrentNeuron->next;
      }

      AgentPopulation->Agent[CurrentAgent].NeuronsQuantity = OutputResolution + EnvironmentResolution;
      AgentPopulation->Agent[CurrentAgent].SynapsesQuantity = OutputResolution * EnvironmentResolution;
   }
}*/

// ��������� ������ ��������� ������� �������
void RecordAgentGenomes(TAgentGenomePopulation* AgentGenomePopulation, char* AgentGenomeFilename)
{
   FILE* hAgentGenomesFile;
   hAgentGenomesFile = fopen(AgentGenomeFilename,"w");

   for (int AgentGenomeNumber=0; AgentGenomeNumber<AgentGenomePopulation->PopulationAgentQuantity; ++AgentGenomeNumber)
      RecordPoolNetwork(AgentGenomePopulation->AgentGenome[AgentGenomeNumber], AgentGenomeNumber + 1, hAgentGenomesFile);

   fclose(hAgentGenomesFile);
}

// ��������� ���������� ������ ����������� (�� ������ ����� ��������)
void PrepareOutputFiles(FILE** hResultFile, FILE** hHistogramFile, FILE** hBestAgentFile, TFilenameSettings* FilenameSettings,TRunSettings* RunSettings, TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TEnvironmentAims* EnvironmentAims, double EnVariableProbability, bool RewardMode)
{
   // �������������� ���� �������� �����������
   *hResultFile = fopen(FilenameSettings->ResultFilename, "w");

   // ������ ���������� �������������
   fprintf(*hResultFile, "EnviromentFile=%i\tAgentPopulation=%i\tEnResolution=%i\t",RunSettings->EnvironmentNumber, TimeSettings->AgentPopulationQuantity, EnvironmentAims->EnvironmentResolution);
   fprintf(*hResultFile, "AddConProb=%.5f\tDelConProb=%.5f\tAddNodeProb=%.5f\tMutProbability=%.5f\tMutMeanDisp=%.5f\t", MutationSettings->AddConnectionProb, MutationSettings->DeleteConnectionProb, MutationSettings->InsertPoolProb, MutationSettings->MutWeightProbability, MutationSettings->MutWeightMeanDisp);
   fprintf(*hResultFile, "MutDispDisp=%.5f\tAddPredConProb=%.5f\tDelPredConProb=%.5f\tDupPoolProb=%.5f\t", MutationSettings->MutWeightDispDisp, MutationSettings->AddPredConnectionProb, MutationSettings->DeletePredConnectionProb, MutationSettings->DuplicatePoolProb);
   fprintf(*hResultFile, "DisConProb=%.5f\tEnConProb=%.5f\tEnVarProb=%.5f\t", MutationSettings->DisConProb, MutationSettings->EnConProb, EnVariableProbability);
   fprintf(*hResultFile, "LifeTime=%i\tTimeReward=%i\tRandRang=%i\n", TimeSettings->AgentLifetime, TimeSettings->RewardRecoveryTime, RunSettings->RandomizeRank);
   // ������ ���������� � ����� �����������
   fprintf(*hResultFile,"Number\t"); // ������ ������ ����� ��������

   if (RewardMode) // ���� ������������ ������� ������
      fprintf(*hResultFile,"AveragePure\tMaxPure\t");

   fprintf(*hResultFile,"Average\tMax\t");

   /*for (int i=0;i<EnvironmentAims->AimQuantity;i++) // ������ � ���� ���������� ������ � ���������� �����
      fprintf(*hResultFile,"N%i_Av\tN%i_Agent\t",i+1,i+1); */
   fprintf(*hResultFile,"Neuron_Average\tCon_Average\n");

   // �������������� ���� � �������������
   *hHistogramFile = fopen(FilenameSettings->HistogramFilename,"w");
   // ������ ���������� � ����� ����������
   for (int i=0;i<10;i++) // ������������� �����������
      fprintf(*hHistogramFile,"Histogram_max/10*%i\t",i+1);
   fprintf(*hHistogramFile, "\n");
   // �������������� ���� � ������� ��������
   *hBestAgentFile = fopen(FilenameSettings->BestAgentDumpFilename, "w");
}

// ��������� ���������� ������������� ����������� ������������� ������
void GetRelativeHistogram(TAgentGenomePopulation* AgentGenomePopulation, int* RelativeHistogram, int HistogramCount, double MaxReward)
{
   for (int i=0; i<HistogramCount; i++) // ������� �����������
      RelativeHistogram[i] = 0;

   for (int CurrentAgent=0; CurrentAgent<AgentGenomePopulation->PopulationAgentQuantity; CurrentAgent++)
   {
      // ������� ����������� ������
      int CurrentCount=0; // ������� ������ �����������
      do
      {
         CurrentCount++;
      } while (CurrentCount * static_cast<int>(MaxReward / 10.0F + 1) < AgentGenomePopulation->AgentGenome[CurrentAgent]->Reward);
      RelativeHistogram[CurrentCount-1] = RelativeHistogram[CurrentCount-1] + 1;
   }
}

// ��������� ������ ����������� ����� ��������
void ResultsOutput(TAgentGenomePopulation* AgentGenomePopulation, FILE* hResultFile, FILE* hHistogramFile, FILE* hBestAgent, char* BestPopulationFilename, double* BestAverageReward, int EvolutionStep, bool RewardMode, int PenaltyRewardLimit)
{
   double MaxReward = -1; // ������������ ������� � ���������
   double AverageReward = 0; // ������� ������� � ���������
   //double MaxPureReward = -1; // ������������ ������ (� ������) ������ ������� � ��������� (���� ������������ �����)
   //double AveragePureReward = 0;// ������� ������ (� ������) ������ ������� � ��������� (���� ������������ �����)
   double AveragePoolsQuantity = 0; // ������� ���-�� �������� � ������
   double AverageConnectionQuantity = 0; // ������� ���-�� ��������

   int BestAgent; // ������ ����� � ���������

   // ��������� ��� ����������
   for (int CurrentAgent=0; CurrentAgent<AgentGenomePopulation->PopulationAgentQuantity; ++CurrentAgent)
   {
      if (AgentGenomePopulation->AgentGenome[CurrentAgent]->Reward > MaxReward)
      {
         MaxReward = AgentGenomePopulation->AgentGenome[CurrentAgent]->Reward;
         BestAgent = CurrentAgent;
      }
      AverageReward += AgentGenomePopulation->AgentGenome[CurrentAgent]->Reward;

      /*if (RewardMode) // ���� ������������ �������� �������
      {
         double CurrentAgentRewardWithPenalty = PopulationReward[CurrentAgent] - RewardPenalty(&(AgentPopulation->Agent[CurrentAgent]), PenaltyRewardLimit);
         if (CurrentAgentRewardWithPenalty > MaxPureReward)
            MaxPureReward = CurrentAgentRewardWithPenalty;
         AveragePureReward += CurrentAgentRewardWithPenalty;
      }*/

      AveragePoolsQuantity += AgentGenomePopulation->AgentGenome[CurrentAgent]->PoolQuantity;
      AverageConnectionQuantity += AgentGenomePopulation->AgentGenome[CurrentAgent]->ConnectionQuantity;
   }

   AverageReward = AverageReward / AgentGenomePopulation->PopulationAgentQuantity;
   /*if (RewardMode)
      AveragePureReward = AveragePureReward / AgentPopulation->PopulationAgentQuantity;*/
   AveragePoolsQuantity = AveragePoolsQuantity / AgentGenomePopulation->PopulationAgentQuantity;
   AverageConnectionQuantity = AverageConnectionQuantity / AgentGenomePopulation->PopulationAgentQuantity;

   // ���������� � ���� �����������
   fprintf(hResultFile,"%i\t", EvolutionStep);
   /*if (RewardMode)
      fprintf(hResultFile,"%.2f\t%.2f\t", AveragePureReward, MaxPureReward);*/
   fprintf(hResultFile,"%.2f\t%.2f\t%.2f\t%.2f\n", AverageReward, MaxReward, AveragePoolsQuantity, AverageConnectionQuantity);

   //���������� ������� ������
   RecordPoolNetwork(AgentGenomePopulation->AgentGenome[BestAgent], BestAgent+1, hBestAgent);

   // ��������� ����� �� ���������� ���������
   if (AverageReward >= (*BestAverageReward))
   {
      RecordAgentGenomes(AgentGenomePopulation, BestPopulationFilename);
      *BestAverageReward = AverageReward;
   }

   // ������ ������������� ����������� ������������� ������
   int RelativeHistogram[HistogramCount];
   GetRelativeHistogram(AgentGenomePopulation, RelativeHistogram, HistogramCount, MaxReward);
   // ���������� ������������� �����������
   for (int i=0; i<HistogramCount; i++)
      fprintf(hHistogramFile, "%i\t", RelativeHistogram[i]);
   fprintf(hHistogramFile, "\n");

   //!
   printf("%i:\t%.3f\t%.3f\n", EvolutionStep, AverageReward, MaxReward);
}

void Evolution(TModeSettings* ModeSettings, TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TFilenameSettings* FilenameSettings, TRunSettings* RunSettings, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings, TLearningSettings* LearningSettings, double EnVariableProbability)
{
   TEnvironmentAims EnvironmentAims;
   InitEnvironment(&EnvironmentAims, FilenameSettings->EnvironmentFilename); // ��������� �����

   // ����� � ��������� ������������
   FILE* hResultFile; // �������� ���� �����������
   FILE* hHistogramFile; // ���� � ������������� ������������� ������� �� ���������
   FILE* hBestAgentFile; // ���� � ������� �������� � ������ ���������
   PrepareOutputFiles(&hResultFile, &hHistogramFile, &hBestAgentFile, FilenameSettings, RunSettings, TimeSettings, MutationSettings, &EnvironmentAims, EnVariableProbability, ModeSettings->RewardMode);

   TAgentGenomePopulation AgentGenomePopulation; // ������� ��������� �������
   AgentGenomePopulation.PopulationAgentQuantity = TimeSettings->AgentPopulationQuantity;
   TAgentPopulation AgentPopulation;
   AgentPopulation.PopulationAgentQuantity = AgentGenomePopulation.PopulationAgentQuantity;

   if (!ModeSettings->EvolutionMode) // ���� ������������ �������� NEAT
      {}//InitFirstGeneration_NEAT(&AgentGenomePopulation, EnvironmentAims.EnvironmentResolution, EnvironmentAims.OutputResolution);
   else // ���� ������������ ���������������� ������������ ��������
		InitFirstGeneration_ModernEvolution(&AgentGenomePopulation, EnvironmentAims.EnvironmentResolution, EnvironmentAims.OutputResolution, ModeSettings->LearningMode ? PrimarySystemogenesisSettings->InitialPoolCapacity : 1);

   double BestAverageReward = 0; // ������� ������� ������ ���������
   double CurrentConInnovationNumber = (double) AgentGenomePopulation.AgentGenome[0]->ConnectionQuantity;
   double CurrentPredConInnovationNumber = (double) AgentGenomePopulation.AgentGenome[0]->PredConnectionQuantity;

   for (int EvolutionStep=1; EvolutionStep<=TimeSettings->EvolutionTime; ++EvolutionStep)
   {
	   for (int AgentNumber=0; AgentNumber<AgentGenomePopulation.PopulationAgentQuantity; ++AgentNumber) // ��������� ����� ���� �������
      {
         AgentPopulation.Agent[AgentNumber] = CreateNeuralNetwork();
         if (ModeSettings->LearningMode) 
				AgentPrimarySystemogenesis(AgentPopulation.Agent[AgentNumber], AgentGenomePopulation.AgentGenome[AgentNumber], PrimarySystemogenesisSettings);
         else
				AgentLinearPrimarySystemogenesis(AgentPopulation.Agent[AgentNumber], AgentGenomePopulation.AgentGenome[AgentNumber]);
         //!!
         /*char AgentDotFile[255];
         sprintf(AgentDotFile, "C:/EvolutionImages/Agent%i-%i.dot", EvolutionStep, AgentNumber);
         NeuralNetwork2Dot(AgentPopulation.Agent[AgentNumber], AgentDotFile);
         FILE* file = fopen("C:/EvolutionImages/Agent.txt", "w");
         RecordNeuralNetwork(AgentPopulation.Agent[AgentNumber], AgentNumber, file);
         fclose(file);
         printf("\nD\n");
         char c = getchar();*/
         //!!
			AgentLife(AgentPopulation.Agent[AgentNumber], &EnvironmentAims, TimeSettings->RewardRecoveryTime, EnVariableProbability, TimeSettings->AgentLifetime,  ModeSettings->NetworkMode, ModeSettings->LearningMode, LearningSettings);
         AgentGenomePopulation.AgentGenome[AgentNumber]->Reward = AgentPopulation.Agent[AgentNumber]->Reward;
      }
      //����� �����������
      ResultsOutput(&AgentGenomePopulation, hResultFile, hHistogramFile, hBestAgentFile, FilenameSettings->BestPopulationFilename, &BestAverageReward, EvolutionStep, ModeSettings->RewardMode, MutationSettings->PenaltyRewardLimit);
      //!!
      /*double MaxRewardAgent = -1;
      int BestAgent = 0;
      for (int CurrentAgent=0; CurrentAgent<AgentGenomePopulation.PopulationAgentQuantity; CurrentAgent++)
         if (AgentGenomePopulation.AgentGenome[CurrentAgent]->Reward > MaxRewardAgent)
         {
            MaxRewardAgent = AgentGenomePopulation.AgentGenome[CurrentAgent]->Reward;
            BestAgent = CurrentAgent;
         }

      char BestGenomeDotFile[255];
      sprintf(BestGenomeDotFile, "D:/EvolutionImages/BestGenome%i.dot", EvolutionStep);
      char BestAgentDotFile[255];
      sprintf(BestAgentDotFile, "D:/EvolutionImages/BestAgent%i.dot", EvolutionStep);
      PoolNetwork2Dot(AgentGenomePopulation.AgentGenome[BestAgent], BestGenomeDotFile);
      NeuralNetwork2Dot(AgentPopulation.Agent[BestAgent], BestAgentDotFile);*/
      //!!
      GenerateNextPopulation(&AgentGenomePopulation, MutationSettings, EvolutionStep, ModeSettings->EvolutionMode, ModeSettings->RewardMode, &CurrentConInnovationNumber, &CurrentPredConInnovationNumber);
		for (int AgentNumber =0; AgentNumber<AgentPopulation.PopulationAgentQuantity; ++AgentNumber)
         AgentPopulation.Agent[AgentNumber] = DeleteNeuralNetwork(AgentPopulation.Agent[AgentNumber]);
   }
   for (int CurrentAgent=0; CurrentAgent<AgentGenomePopulation.PopulationAgentQuantity; ++CurrentAgent)
      AgentGenomePopulation.AgentGenome[CurrentAgent] = DeletePoolNetwork(AgentGenomePopulation.AgentGenome[CurrentAgent]);

   fclose(hResultFile);
   fclose(hHistogramFile);
   fclose(hBestAgentFile);
}

// ������� ��������� ���������� � ������� ��������
void PrepareEvolutionProcess(TRunSettings* RunSettings, TModeSettings* ModeSettings, TTimeSettings* TimeSettings, TMutationSettings* MutationSettings, TDirectorySettings* DirectorySettings, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings, TLearningSettings* LearningSettings, double EnVariableProbability)
{
   TFilenameSettings FilenameSettings;

   // ��������� ����� ����� �����
   GetEnvironmentFilename(FilenameSettings.EnvironmentFilename, DirectorySettings->EnvironmentDirectory, RunSettings->EnvironmentNumber);

   // ��������� ����� ����� �����������
   bool EnvironmentStability = (EnVariableProbability == 0);
   GetResultsFilename(FilenameSettings.ResultFilename, RunSettings, ModeSettings, DirectorySettings->ResultDirectory, EnvironmentStability);
   // ��������� ����� ����� ����������
   GetHistogramFilename(FilenameSettings.HistogramFilename, RunSettings, ModeSettings, DirectorySettings->ResultDirectory, EnvironmentStability);

   GetBestPopulationFilename(FilenameSettings.BestPopulationFilename, RunSettings, ModeSettings, DirectorySettings->ResultDirectory, EnvironmentStability);

   GetBestAgentFilename(FilenameSettings.BestAgentDumpFilename, RunSettings, ModeSettings, DirectorySettings->ResultDirectory, EnvironmentStability);

   srand(RunSettings->RandomizeRank); // ������������� ���������� ��������� �����
   // ��� �������� ������� ���������� (����� �������� � ������� ��������)
   int tmp_rand = rand()%1000; // �������� ������ ���������� ��������� �����
   tmp_rand = rand()%1000; // �������� ������ ���������� ��������� �����
   tmp_rand = rand()%1000; // �������� ������ ���������� ��������� �����

   Evolution(ModeSettings, TimeSettings, MutationSettings, &FilenameSettings, RunSettings, PrimarySystemogenesisSettings, LearningSettings, EnVariableProbability);

}
