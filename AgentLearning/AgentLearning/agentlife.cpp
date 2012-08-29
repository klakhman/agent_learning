#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "service.h"
#include "service_network.h"
#include "environment.h"
#include "learning.h"

#include "agentlife.h"

//��������� ����������� ����� �������, ���������� ������� � ������� �����
double CalcAgentReward(int* LifeRecord, int AgentLifetime, TEnvironmentAims* EnvironmentAims, int RewardRecoveryTime)
{
   double AgentReward = 0.0; // ������� ������
   for (int CurStep=0; CurStep<AgentLifetime; CurStep++) // ���������� �� ���� ����� �����
   {
      for (int CurAim=0; CurAim<EnvironmentAims->AimQuantity; CurAim++) // ��������� ��� ����
         if ( EnvironmentAims->Aim[CurAim].Complexity <= (CurStep + 1) ) // ���� ����� ����� �� ���������� ���� ����
         {
            int ErrorCheck = 0; //������� ����, ��� ����������� ���� �� ����������
            int CurBit = 0; // ������� ��� ����
            do // ���������� �� ����� � ��������� ���������� ������� ����
            {
               // ����������: ���� ����� ������ �� ������ �� � ����� �������� 0, � ������ ������ ����� ����� �� �����
               // ���� ������� ��� ���� ��������� � ���������� ����� � ����� (c ������ ����������� ������� �����)
               if ( EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Number == (abs(LifeRecord[CurStep - EnvironmentAims->Aim[CurAim].Complexity + 1 + CurBit ]) - 1) )
                  if (LifeRecord[CurStep - EnvironmentAims->Aim[CurAim].Complexity + CurBit + 1] < 0) // ���� ������� ��� �� ������� � ����
                  {
                     if (EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Value == 1) ErrorCheck = 1; // �� ���� � ���� ���� ���� ��������� ��� � ������� �� �������
                  }
                  else // �����
                  {
                     if (EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Value == 0) ErrorCheck = 1; // ���� ���� ���� ��������� ��� � ����, �� �������
                  }
               else // ���� ����� ���� �� ��������� �� �������
                  ErrorCheck = 1;
               CurBit++;
            } while ( (!ErrorCheck) && (CurBit < EnvironmentAims->Aim[CurAim].Complexity) ); // ���� ��� ������ ��� ���� �������� ���� �� ���������
            if (!ErrorCheck) // ���� �� ���� ���������������� ������, ������ ���� ����������
            {
               //AimStatistic[j]+=1;
               //AimTemp[j]+=1;
               // ���������� ������� ������� �� ���� (������� �� ����������� ������� � ������������������)
               AgentReward += min((double) EnvironmentAims->Aim[CurAim].Reward, (CurStep + 1 - EnvironmentAims->Aim[CurAim].VisitTime) * EnvironmentAims->Aim[CurAim].Reward / ((double) RewardRecoveryTime) );
               EnvironmentAims->Aim[CurAim].VisitTime=CurStep + 1;
            }
         }
    }

    return AgentReward;
    /*for (int i=0;i<AimQuantity;i++)
    {
        if (AimTemp[i]) AimStatPure[i]++;
        AimTemp[i]=0;
    }
    return 0;*/
}

//��������� �������� ������ � ���� ���� �������
void AgentStep(TNeuralNetwork* AgentNetwork, int* EnvironmentVector, TEnvironmentAims* EnvironmentAims, int* LifeRecord, int AgentLifeStep, bool NetworkMode)
{
   int OutputResolution = EnvironmentAims->OutputResolution; // ����������� ��������� ������������
   int EnvironmentResolution = EnvironmentAims->EnvironmentResolution; // ����������� �����
	//!!! ��������� ��������� � �������������� ������� � �������� �����
	// ����������� �������� �������� ��������� � �������� ������������ (��� ������ ����� ������� � �������� ������� ����� ������������ ������)
	int* distr_input_neurons = new int[EnvironmentResolution]; // ���������� ������� �������� � ������ ������� �������� ����
	memset(distr_input_neurons, 0, sizeof(int)*EnvironmentResolution);
	int network_EnvironmentResolution = 0; // �������� ���-�� ������� �������� � ����
	TNeuron* CurrentNeuron = AgentNetwork->NetworkStructure;
	while (CurrentNeuron->Type == 0){ // ������������ ������������� ������� ��������
		++distr_input_neurons[CurrentNeuron->ParentPoolID - 1];
		++network_EnvironmentResolution;
		CurrentNeuron = CurrentNeuron->next;
	}
	int* distr_output_neurons = new int[OutputResolution]; // ���������� �������� �������� � ������ ������� ��������� ����
	memset(distr_output_neurons, 0, sizeof(int)*OutputResolution);
	int network_OutputResolution = 0; // �������� ���-�� �������� �������� � ����
	while (CurrentNeuron->Type == 2){ // ������������ ������������� �������� ��������
		++distr_output_neurons[CurrentNeuron->ParentPoolID - EnvironmentResolution - 1];
		++network_OutputResolution;
		CurrentNeuron = CurrentNeuron->next;
	}
	int* _EnvironmentVector = new int[network_EnvironmentResolution];
	int CurrentBit = 0;
	for (int i = 0; i < EnvironmentResolution; ++i)
		for (int j=0; j < distr_input_neurons[i]; ++j)
			_EnvironmentVector[CurrentBit++] = EnvironmentVector[i];
	double* _CurrentNetworkOut = new double[network_OutputResolution];
	memset(_CurrentNetworkOut, 0, sizeof(double) * network_OutputResolution);
	//!!!
   if (!NetworkMode) // ���� ����� �������������
      NetworkOutput_standart(AgentNetwork, _EnvironmentVector, _CurrentNetworkOut); // ���������� ������� �������� ������ ��������� ������
   else // ���� ����� ����������
      NetworkOutput_synch(AgentNetwork, _EnvironmentVector, _CurrentNetworkOut);
	
	double* CurrentNetworkOut = new double[OutputResolution]; 
	memset(CurrentNetworkOut, 0, sizeof(double) * OutputResolution);
	//!!!
	// ������� ������� ������ ������� ��������� ����
	CurrentBit = 0;
	for (int i=0; i < OutputResolution; ++i)
		for (int j=0; j < distr_output_neurons[i]; ++j)
			CurrentNetworkOut[i] += _CurrentNetworkOut[CurrentBit++] / distr_output_neurons[i];	

	delete []distr_input_neurons;
	delete []distr_output_neurons;
	delete []_EnvironmentVector;
	delete []_CurrentNetworkOut;
	//!!!

   double MaxOutput_1=-1; // �������� ������� �� �������� ������
   double MaxOutput_2=-1; // �������� ������� �� �������� ������
   int MaxNeuronNumber_1=0; // ����� ������� � ������ �� �������� �������
   int MaxNeuronNumber_2=0; // ����� ������� �� ������ �� �������� �������

   // ����������� ���� ������������ ������� ��������
   for (int i=0; i<OutputResolution; i++)
      if (CurrentNetworkOut[i] > MaxOutput_1)
      {
         MaxOutput_2 = MaxOutput_1;
         MaxNeuronNumber_2 = MaxNeuronNumber_1;
         MaxOutput_1 = CurrentNetworkOut[i];
         MaxNeuronNumber_1 = i+1;
      }
      else
         if (CurrentNetworkOut[i] > MaxOutput_2)
         {
            MaxOutput_2 = CurrentNetworkOut[i];
            MaxNeuronNumber_2 = i+1;
         }
   // ���� ������� �� �� �������, �� ������ ������� ������ (��� ������� ��� �� ���� ������)
   if (MaxNeuronNumber_1 > MaxNeuronNumber_2)
   {
      MaxNeuronNumber_1 = MaxNeuronNumber_1 + MaxNeuronNumber_2;
      MaxNeuronNumber_2 = MaxNeuronNumber_1 - MaxNeuronNumber_2;
      MaxNeuronNumber_1 = MaxNeuronNumber_1 - MaxNeuronNumber_2;
   }

   //��������� ����������� ������� ����
   int CurNeuronNumber_1 = 1; // ����� ������� �������� ������� � ����������� ����
   int CurNeuronNumber_2; // ����� ������� �������� ������� � ����
   int OutCheck = 0; // ������� ����, ��� ������ ���� �������
   int PairCount = 0; // ���-�� ����������� ���
   int ActionNumber = 0;// �������� ����� ������������ ��������
   do
   {
      CurNeuronNumber_2 = CurNeuronNumber_1 + 1;
      do
      {
         // ��������� ������ �� ��� ����
         if ( (MaxNeuronNumber_1 == CurNeuronNumber_1) && (MaxNeuronNumber_2 == CurNeuronNumber_2) )
         {
            ActionNumber = PairCount % (2 * EnvironmentResolution); // ���������� ����� ��������
            OutCheck = 1;
         }
         PairCount++;
         CurNeuronNumber_2++;
      }while ( (CurNeuronNumber_2 <= OutputResolution) && (!OutCheck) );
      CurNeuronNumber_1++;
   }while ( (CurNeuronNumber_1 < OutputResolution) && (!OutCheck) );

   // ��������� ������� ����� � ������ �������� � �������� ����� ������
   if (ActionNumber % 2) // ���� ����������� ����� ���� �������� ����� - ������ ��� ������� �� 1 � 0
   {
      if (EnvironmentVector[(ActionNumber - 1) / 2]) // ���� ��� ������� ��� ��������
      {
         EnvironmentVector[(ActionNumber - 1) / 2] = 0;
         LifeRecord[AgentLifeStep - 1] = - ((ActionNumber - 1) / 2 + 1);
      }
      else
         LifeRecord[AgentLifeStep - 1] = 0; // ����� ����� ������ �� ������
   }
   else // ����� �� 0 � 1
   {
      if (!EnvironmentVector[ActionNumber / 2]) // ���� ��� ������� ��� �����
      {
         EnvironmentVector[ActionNumber / 2] = 1;
         LifeRecord[AgentLifeStep - 1] = ActionNumber / 2 + 1;
      }
      else
         LifeRecord[AgentLifeStep - 1] = 0;
   }

   delete []CurrentNetworkOut;
}

//��������� ������������� ����� ������
void AgentLife(TNeuralNetwork* AgentNetwork, TEnvironmentAims* EnvironmentAims, int RewardRecoveryTime, double EnVarProb, int AgentLifetime, bool NetworkMode /* = false*/, bool LearningMode /* = false*/, TLearningSettings* LearningSettings /* = NULL*/)
{
   ResetEnviroment(EnvironmentAims); // ������������� �����
   ResetNeuralNetwork(AgentNetwork);

   //int EnvironmentVector[EnvironmentAims->EnvironmentResolution]; // ������ ��������� �����
   int* EnvironmentVector = new int[EnvironmentAims->EnvironmentResolution];
   for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++) // ���������� ��������� �������������� ���������
      EnvironmentVector[i] =  rand() % 2;

   //int LifeRecord[AgentLifetime]; // ������ ����� ������
   int* LifeRecord = new int[AgentLifetime];
   // ��������� ������� �����
   for (int AgentLifeStep=1; AgentLifeStep<=AgentLifetime; AgentLifeStep++)
   {
      AgentStep(AgentNetwork, EnvironmentVector, EnvironmentAims, LifeRecord, AgentLifeStep, NetworkMode);
      if (LearningMode) AgentLearning(AgentNetwork, LearningSettings);
      // ��������� ��������� ������� ��������� (��� ����������������� �����)
      for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++)
         if (rand()%10001 < EnVarProb*10001) // ��������� ���� �� ������� ���
         {
            if (EnvironmentVector[i] == 1) // ���� ��� ��� ��������
               EnvironmentVector[i] = 0; // �������� �� ����
            else
               EnvironmentVector[i] = 1; // ����� �������� �� �������
         }
   }
   delete []EnvironmentVector;
   // ������������ ��������� ������� �������
   AgentNetwork->Reward = CalcAgentReward(LifeRecord, AgentLifetime, EnvironmentAims, RewardRecoveryTime);
   delete []LifeRecord;
}

// �������� ���������� ����� � ����������� ���������� � �������� �������
double CheckRewardInRealTime(int* LifeRecord, int AgentLifeStep, TEnvironmentAims* EnvironmentAims, int RewardRecoveryTime)
{
   AgentLifeStep--; // ��������� �� ������� ������� ��� (����� ������ ����� ���� � ����)

   double AgentReward = 0.0; // ������� ������
   for (int CurAim=0; CurAim<EnvironmentAims->AimQuantity; CurAim++) // ��������� ��� ����
      if ( EnvironmentAims->Aim[CurAim].Complexity <= (AgentLifeStep + 1) ) // ���� ����� ����� �� ���������� ���� ����
      {
         int ErrorCheck = 0; //������� ����, ��� ����������� ���� �� ����������
         int CurBit = 0; // ������� ��� ����
         do // ���������� �� ����� � ��������� ���������� ������� ����
         {
            // ����������: ���� ����� ������ �� ������ �� � ����� �������� 0, � ������ ������ ����� ����� �� �����
            // ���� ������� ��� ���� ��������� � ���������� ����� � ����� (c ������ ����������� ������� �����)
            if ( EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Number == (abs(LifeRecord[AgentLifeStep - EnvironmentAims->Aim[CurAim].Complexity + 1 + CurBit ]) - 1) )
               if (LifeRecord[AgentLifeStep - EnvironmentAims->Aim[CurAim].Complexity + CurBit + 1] < 0) // ���� ������� ��� �� ������� � ����
               {
                  if (EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Value == 1) ErrorCheck = 1; // �� ���� � ���� ���� ���� ��������� ��� � ������� �� �������
               }
               else // �����
               {
                  if (EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Value == 0) ErrorCheck = 1; // ���� ���� ���� ��������� ��� � ����, �� �������
               }
            else // ���� ����� ���� �� ��������� �� �������
               ErrorCheck = 1;
            CurBit++;
         } while ( (!ErrorCheck) && (CurBit < EnvironmentAims->Aim[CurAim].Complexity) ); // ���� ��� ������ ��� ���� �������� ���� �� ���������
         if (!ErrorCheck) // ���� �� ���� ���������������� ������, ������ ���� ����������
         {
            // ���������� ������� ������� �� ���� (������� �� ����������� ������� � ������������������)
            double LocalAgentReward = min((double) EnvironmentAims->Aim[CurAim].Reward, (AgentLifeStep + 1 - EnvironmentAims->Aim[CurAim].VisitTime) * EnvironmentAims->Aim[CurAim].Reward / ((double) RewardRecoveryTime) );
            EnvironmentAims->Aim[CurAim].VisitTime=AgentLifeStep + 1;
            AgentReward += LocalAgentReward;

            printf("Agent have reached aim #%i - local reward: %.3f\n", CurAim, LocalAgentReward);
         }
      }
    return AgentReward;
}

// ����������� ����� ������ � ��������� ������
void AgentLifeView(TNeuralNetwork* AgentNetwork, TEnvironmentAims*  EnvironmentAims, int RewardRecoveryTime, double EnVarProb, int AgentLifetime, bool NetworkMode)
{
   ResetEnviroment(EnvironmentAims); // ������������� �����
   ResetNeuralNetwork(AgentNetwork);

   AgentNetwork->Reward = 0;
   //int EnvironmentVector[EnvironmentAims->EnvironmentResolution]; // ������ ��������� �����
   int* EnvironmentVector = new int[EnvironmentAims->EnvironmentResolution];
   for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++) // ���������� ��������� �������������� ���������
      EnvironmentVector[i] =  rand() % 2;

   //int LifeRecord[AgentLifetime]; // ������ ����� ������
   int* LifeRecord = new int[AgentLifetime];
   // ��������� ������� �����
   for (int AgentLifeStep=1; AgentLifeStep<=AgentLifetime; AgentLifeStep++)
   {
      // �������� ��������� �� �������� ����
      printf("%i:\t%.3f\t", AgentLifeStep, AgentNetwork->Reward);
      for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++)
         printf("%i ", EnvironmentVector[i]);
      printf("\t->\t");

      AgentStep(AgentNetwork, EnvironmentVector, EnvironmentAims, LifeRecord, AgentLifeStep, NetworkMode);

      // �������� ��������� ����� �������� ����
      for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++)
         if ((abs(LifeRecord[AgentLifeStep-1]) - 1) == i)
            printf("[%i] ", EnvironmentVector[i]);
         else
            printf("%i ", EnvironmentVector[i]);
      printf("\n");

      TNeuron* OutputNeuron = AgentNetwork->NetworkStructure;
      while (OutputNeuron!=NULL)
      {
         if (OutputNeuron->Type==2)
            printf("%.2f\t", OutputNeuron->CurrentOut);
         OutputNeuron = OutputNeuron->next;
      }
      printf("\n");

      AgentNetwork->Reward += CheckRewardInRealTime(LifeRecord, AgentLifeStep, EnvironmentAims, RewardRecoveryTime); // ������� ������� ������ �� ������� ���, � ����� ���������� ���������� � ����������� �����

      char c = getchar();

      // ��������� ��������� ������� ��������� (��� ����������������� �����)
      for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++)
         if (rand()%10001 < EnVarProb*10001) // ��������� ���� �� ������� ���
         {
            if (EnvironmentVector[i] == 1) // ���� ��� ��� ��������
               EnvironmentVector[i] = 0; // �������� �� ����
            else
               EnvironmentVector[i] = 1; // ����� �������� �� �������
         }
   }
   delete []LifeRecord;
   delete []EnvironmentVector;
}
