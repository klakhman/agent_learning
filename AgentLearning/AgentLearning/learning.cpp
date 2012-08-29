#include <stdio.h>

#include "types.h"
#include "service.h"
#include "service_network.h"

#include "learning.h"

// �������� ��������������� �� �������
// 0 - ���������� ���������������, 1 - ��������������� ���� "����������� ��������� - �� ���", 2 - ��������������� ���� "����������� �������� - ���� ���������"
int MismatchDetection(TNeuron* TargetNeuron, double SufPredSignificance)
{
   TPredConnection* CurrentPredConnection = TargetNeuron->PredConnections;
   int ActivePrediction = 0; // ���������� ������, ������������� ���������
   int SilentPrediction = 0; // ���������� ������, ������������� ��������
   while (CurrentPredConnection != NULL)
   {
      if (CurrentPredConnection->PreNeuron->Active) // ���� ��������������� ������ �������
      {
         if (CurrentPredConnection->PreNeuron->PreviousOut > 0.5) // ���� ��������������� ������ ��� ��������� �� ���������� �����
            ++ActivePrediction;
         else
            ++SilentPrediction;
      }
      CurrentPredConnection = CurrentPredConnection->next;
   }

   int MismatchCheck = 0;
   if (ActivePrediction + SilentPrediction) // ���� ���� ���� ���� �������� ��������������� ������
   {
      // ���� ���� ����������� ���������, �� �� ���
      if ((ActivePrediction/(ActivePrediction+SilentPrediction) > SufPredSignificance) && (TargetNeuron->CurrentOut <= 0.5))
         MismatchCheck = 1;
      else if ((SilentPrediction/(ActivePrediction+SilentPrediction) > SufPredSignificance) && (TargetNeuron->CurrentOut > 0.5))
         MismatchCheck = 2;
      else
         MismatchCheck = 0;

   }
   else
      MismatchCheck = 0;

   return MismatchCheck;
}

// ���������� �������� ��������� ������� � ���� ����������������� ������� � ������ ��������
TNeuron* FindMostActiveNeuron(TNeuralNetwork* AgentNeuralNetwork, TNeuron* MismatchNeuron)
{
   TNeuron* CurrentNeuron = AgentNeuralNetwork->NetworkStructure;
   TNeuron* MostActiveNeuron = NULL;
   double MaxPotential = 0;
   while (CurrentNeuron != NULL)
   {
      // ���� ��� ������ �� ���� �� ����, ��� � ����������������, �� ������� � ���������� �� ������ �����
      if ((CurrentNeuron->ParentPoolID == MismatchNeuron->ParentPoolID) && (!CurrentNeuron->Active) && (CurrentNeuron->Potential > 0))
         if (MaxPotential < CurrentNeuron->Potential)
         {
            MaxPotential = CurrentNeuron->Potential;
            MostActiveNeuron = CurrentNeuron;
         }
      CurrentNeuron = CurrentNeuron->next;
   }
   return MostActiveNeuron;
}

// ����������� ��������� ������ ��������������� ������� - ��������� ������ ����� �� �������� �� ������� ����� �������� - ���������� ���-�� ��������� �������� (������������� ��������)
int ModifySynapseStructure(TNeuron* ActivatedNeuron)
{
   int DeletedSynapses = 0;
   TSynapse* CurrentSynapse = ActivatedNeuron->InSynapses;
   // ���������� �� ���� ����� �������
   if (CurrentSynapse != NULL) // ���� ������ ���� �������
   {
      while (CurrentSynapse->next != NULL)
         if ((CurrentSynapse->next->PreNeuron->CurrentOut <= 0.5) && (CurrentSynapse->next->PreNeuron->Active)) // ���� ���-������ �� ��� �������, �� ������� ����� (������ ���� ������ ������ �������)
         {
            TSynapse* tmpSynapse = CurrentSynapse->next;
            CurrentSynapse->next = tmpSynapse->next;
            delete tmpSynapse;
            ++DeletedSynapses;
         }
         else
            CurrentSynapse = CurrentSynapse->next;
      if ((ActivatedNeuron->InSynapses->PreNeuron->CurrentOut <= 0.5) && (ActivatedNeuron->InSynapses->PreNeuron->Active))
      {
         TSynapse* tmpSynapse = ActivatedNeuron->InSynapses;
         ActivatedNeuron->InSynapses = tmpSynapse->next;
         delete tmpSynapse;
         ++DeletedSynapses;
      }
   }
   return -DeletedSynapses;
}

// ���������� ����� ����� ������������ � ���������������� �������� - ��� ����� ������� �� ���� ��������������� - ���������� ��������� �� ����� ��� ������ ��������
int AddInterPoolConnection(TNeuron* ActivatedNeuron, TNeuron* MismatchNeuron, int SynapsesQuantity, double WeightValue)
{
   TSynapse* CurrentSynapse = MismatchNeuron->InSynapses;
   while ((CurrentSynapse != NULL) && (CurrentSynapse->PreNeuron->ID != ActivatedNeuron->ID)) // �������� ����� ����� ����� ����� ���������, ���� ��� ��� ����
      CurrentSynapse = CurrentSynapse->next;
   int CreatedSynapseCheck = 0; // ������� ����, �� �� ������� ������, � �� ��������2
   if (CurrentSynapse != NULL) // ���� ����� ����� �������
   {
      CurrentSynapse->Enabled = 1; // �� ������ ������ �������� �����
      CurrentSynapse->Weight = WeightValue;
   }
   else // ����� ������� �����
   {
      CurrentSynapse = MismatchNeuron->InSynapses;
      if (CurrentSynapse != NULL)
      {
         while (CurrentSynapse->next != NULL)
            CurrentSynapse = CurrentSynapse->next;
         CurrentSynapse->next = CreateSynapse(SynapsesQuantity+1, WeightValue, 1, ActivatedNeuron, MismatchNeuron, NULL);
      }
      else
         MismatchNeuron->InSynapses = CreateSynapse(SynapsesQuantity+1, WeightValue, 1, ActivatedNeuron, MismatchNeuron, NULL);
      ++CreatedSynapseCheck;
   }
   return CreatedSynapseCheck;
}

// ��������� �������� ������������ ������, ������� ����������� ���������, � ����������������� ������� �� ������������
int ModifyPredConnectionsStructure(TNeuron* ActivatedNeuron, TNeuron* MismatchNeuron, int PredConnectionsQuantity)
{
   int NewPredConnectionsQuantity = 0;
   TPredConnection* ActNeuronPredConnection = ActivatedNeuron->PredConnections;
   TPredConnection* MisNeuronPredConnection = MismatchNeuron->PredConnections;
   while (MisNeuronPredConnection != NULL)
   {
      if (MisNeuronPredConnection->PreNeuron->PreviousOut > 0.5) // ���� ����� ����������� ���������
      {
         //�������� ����� ��� ����� � ��������������� �������
         ActNeuronPredConnection = ActivatedNeuron->PredConnections;
         while ((ActNeuronPredConnection != NULL) && (ActNeuronPredConnection->PreNeuron->ID != MisNeuronPredConnection->PreNeuron->ID))
            ActNeuronPredConnection = ActNeuronPredConnection->next;
         if (ActNeuronPredConnection == NULL) // ���� �� ���� ������� ����� �����, �� ��������� ��
         {
            if (ActNeuronPredConnection != NULL) // ���� ������ ���� �������
            {
               while (ActNeuronPredConnection->next != NULL) // ���� ��������� �����
                  ActNeuronPredConnection = ActNeuronPredConnection->next;
               ActNeuronPredConnection->next = CreatePredConnection(PredConnectionsQuantity+1, 1, MisNeuronPredConnection->PreNeuron, ActivatedNeuron, NULL);
            }
            else
               ActivatedNeuron->PredConnections = CreatePredConnection(PredConnectionsQuantity+1, 1, MisNeuronPredConnection->PreNeuron, ActivatedNeuron, NULL);
            ++NewPredConnectionsQuantity;
         }
      }
      MisNeuronPredConnection = MisNeuronPredConnection->next;
   }
   return PredConnectionsQuantity;
}

// ��������� ������������ ������ �� ����� ����� �������
void AgentLearning(TNeuralNetwork* AgentNeuralNetwork, TLearningSettings* LearningSettings)
{
   TNeuron* CurrentNeuron = AgentNeuralNetwork->NetworkStructure;
   //int MismatchCheck[AgentNeuralNetwork->NeuronQuantity]; // ������ ��������� ������������������ �������� ����
   int* MismatchCheck = new int[AgentNeuralNetwork->NeuronQuantity]; // ������ ��������� ������������������ �������� ����

   // ������� ���������� ������������������ ���� �������� (����� �������� �������� ������������������ ����� ������������ ��������)
   while (CurrentNeuron != NULL)
   {
      if ((CurrentNeuron->Type == 1) && (CurrentNeuron->Active))// ���� ������ ���������� � �� �������
         MismatchCheck[CurrentNeuron->ID - 1] = MismatchDetection(CurrentNeuron, LearningSettings->MismatchSignificanceTreshold); // ���������� ���� �� ���������������
      else
         MismatchCheck[CurrentNeuron->ID - 1] = 0;
      CurrentNeuron = CurrentNeuron->next;
   }
   // �������� ��������� ��������
   CurrentNeuron = AgentNeuralNetwork->NetworkStructure;
   while (CurrentNeuron != NULL)
   {
      if (MismatchCheck[CurrentNeuron->ID - 1]) // ���� ���� ���������������
      {
         TNeuron* ActivatedNeuron = FindMostActiveNeuron(AgentNeuralNetwork, CurrentNeuron);
         if (ActivatedNeuron != NULL) // ���� ������ ���� �� ���� ���������� ������ � ������������ ���� (�� ��� �� ������� � ��� ��������� �� ������� ����)
         {
            AgentNeuralNetwork->SynapseQuantity += ModifySynapseStructure(ActivatedNeuron); // ������������ ��� ������������� ����
            double WeightValue = 0;
            if (MismatchCheck[CurrentNeuron->ID - 1] == 1) // ���� ��� ������ ��� ���������������
               WeightValue = UniformDistribution(0.5, 1);
            else // ���� ��� ������ ��� ���������������
               WeightValue = - UniformDistribution(0.5, 1);
            // ��������� ����� ����� ���������������� � ������������ ��������
            AgentNeuralNetwork->SynapseQuantity += AddInterPoolConnection(ActivatedNeuron, CurrentNeuron, AgentNeuralNetwork->SynapseQuantity, WeightValue);
            AgentNeuralNetwork->PredConnectionQuantity += ModifyPredConnectionsStructure(ActivatedNeuron, CurrentNeuron, AgentNeuralNetwork->PredConnectionQuantity);
            ActivatedNeuron->Active = true;
            ActivatedNeuron->CurrentOut = 0;
         }
      }
      CurrentNeuron = CurrentNeuron->next;
   }
   delete []MismatchCheck;
   fix_synapses_id(AgentNeuralNetwork);
   fix_pred_connections_id(AgentNeuralNetwork);
}
