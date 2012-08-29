#include <stdio.h>

#include "types.h"
#include "service.h"
#include "service_network.h"

#include "learning.h"

// Детекция рассогласования на нейроне
// 0 - отсутствие рассогласования, 1 - рассогласование типа "предсказана активация - ее нет", 2 - рассогласование типа "предсказано молчание - есть активация"
int MismatchDetection(TNeuron* TargetNeuron, double SufPredSignificance)
{
   TPredConnection* CurrentPredConnection = TargetNeuron->PredConnections;
   int ActivePrediction = 0; // Количество связей, предсказавшие активацию
   int SilentPrediction = 0; // Количество связей, предсказавшие молчание
   while (CurrentPredConnection != NULL)
   {
      if (CurrentPredConnection->PreNeuron->Active) // Если предсказывающий нейрон активен
      {
         if (CurrentPredConnection->PreNeuron->PreviousOut > 0.5) // Если предсказывающий нейрон был возбужден на предыдущем такте
            ++ActivePrediction;
         else
            ++SilentPrediction;
      }
      CurrentPredConnection = CurrentPredConnection->next;
   }

   int MismatchCheck = 0;
   if (ActivePrediction + SilentPrediction) // Если есть хоть один активный предсказывающий нейрон
   {
      // Если была предсказана активация, но ее нет
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

// Нахождение наиболее активного нейрона в пуле рассогласованного нейрона в момент обучения
TNeuron* FindMostActiveNeuron(TNeuralNetwork* AgentNeuralNetwork, TNeuron* MismatchNeuron)
{
   TNeuron* CurrentNeuron = AgentNeuralNetwork->NetworkStructure;
   TNeuron* MostActiveNeuron = NULL;
   double MaxPotential = 0;
   while (CurrentNeuron != NULL)
   {
      // Если это нейрон из того же пула, что и рассогласованные, он активен и возбудился на данном такте
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

// Модификация структуры связей активирующегося нейрона - оставляем только связи от активных на текущем такте нейронов - возвращаем кол-во удаленных синапсов (отрицательную величину)
int ModifySynapseStructure(TNeuron* ActivatedNeuron)
{
   int DeletedSynapses = 0;
   TSynapse* CurrentSynapse = ActivatedNeuron->InSynapses;
   // Проходимся по всем кроме первого
   if (CurrentSynapse != NULL) // Если вообще есть синапсы
   {
      while (CurrentSynapse->next != NULL)
         if ((CurrentSynapse->next->PreNeuron->CurrentOut <= 0.5) && (CurrentSynapse->next->PreNeuron->Active)) // Если пре-нейрон не был активен, то удаляем связь (только если нейрон вообще активен)
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

// Добавление связи между включающимся и рассогласованным нейроном - вес связи зависит от типа рассогласования - возвращает добавлена ли связь или просто изменена
int AddInterPoolConnection(TNeuron* ActivatedNeuron, TNeuron* MismatchNeuron, int SynapsesQuantity, double WeightValue)
{
   TSynapse* CurrentSynapse = MismatchNeuron->InSynapses;
   while ((CurrentSynapse != NULL) && (CurrentSynapse->PreNeuron->ID != ActivatedNeuron->ID)) // Пытаемся найти связь между этими нейронами, если она уже есть
      CurrentSynapse = CurrentSynapse->next;
   int CreatedSynapseCheck = 0; // Признак того, то мы создали синапс, а не изменили2
   if (CurrentSynapse != NULL) // Если такая связь найдена
   {
      CurrentSynapse->Enabled = 1; // На всякий случай включаем связь
      CurrentSynapse->Weight = WeightValue;
   }
   else // Иначе создаем связь
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

// Процедура переноса предикторных связей, которые предсказали активацию, с рассогласованного неройна на включающийся
int ModifyPredConnectionsStructure(TNeuron* ActivatedNeuron, TNeuron* MismatchNeuron, int PredConnectionsQuantity)
{
   int NewPredConnectionsQuantity = 0;
   TPredConnection* ActNeuronPredConnection = ActivatedNeuron->PredConnections;
   TPredConnection* MisNeuronPredConnection = MismatchNeuron->PredConnections;
   while (MisNeuronPredConnection != NULL)
   {
      if (MisNeuronPredConnection->PreNeuron->PreviousOut > 0.5) // Если связь предсказала активацию
      {
         //Пытаемся найти эту связь у активирующегося нейрона
         ActNeuronPredConnection = ActivatedNeuron->PredConnections;
         while ((ActNeuronPredConnection != NULL) && (ActNeuronPredConnection->PreNeuron->ID != MisNeuronPredConnection->PreNeuron->ID))
            ActNeuronPredConnection = ActNeuronPredConnection->next;
         if (ActNeuronPredConnection == NULL) // Если не была найдена такая связь, то добавляем ее
         {
            if (ActNeuronPredConnection != NULL) // Если вообще есть синапсы
            {
               while (ActNeuronPredConnection->next != NULL) // Ищем последнюю связь
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

// Процедура самообучения агента на одном такте времени
void AgentLearning(TNeuralNetwork* AgentNeuralNetwork, TLearningSettings* LearningSettings)
{
   TNeuron* CurrentNeuron = AgentNeuralNetwork->NetworkStructure;
   //int MismatchCheck[AgentNeuralNetwork->NeuronQuantity]; // Массив признаков рассогласованности нейронов сети
   int* MismatchCheck = new int[AgentNeuralNetwork->NeuronQuantity]; // Массив признаков рассогласованности нейронов сети

   // Сначала определяем рассогласованность всех нейронов (чтобы избежать проверки рассогласованности вновь включающихся нейронов)
   while (CurrentNeuron != NULL)
   {
      if ((CurrentNeuron->Type == 1) && (CurrentNeuron->Active))// Если нейрон внутренний и он активен
         MismatchCheck[CurrentNeuron->ID - 1] = MismatchDetection(CurrentNeuron, LearningSettings->MismatchSignificanceTreshold); // Определяем есть ли рассогласование
      else
         MismatchCheck[CurrentNeuron->ID - 1] = 0;
      CurrentNeuron = CurrentNeuron->next;
   }
   // Проводим процедуру обучения
   CurrentNeuron = AgentNeuralNetwork->NetworkStructure;
   while (CurrentNeuron != NULL)
   {
      if (MismatchCheck[CurrentNeuron->ID - 1]) // Если есть рассогласование
      {
         TNeuron* ActivatedNeuron = FindMostActiveNeuron(AgentNeuralNetwork, CurrentNeuron);
         if (ActivatedNeuron != NULL) // Если найден хотя бы один подходящий нейрон в родительском пуле (он еще не активен и был возбужден на текущем шаге)
         {
            AgentNeuralNetwork->SynapseQuantity += ModifySynapseStructure(ActivatedNeuron); // Модифицируем его синаптические веса
            double WeightValue = 0;
            if (MismatchCheck[CurrentNeuron->ID - 1] == 1) // Если это первый тип рассогласования
               WeightValue = UniformDistribution(0.5, 1);
            else // Если это второй тип рассогласования
               WeightValue = - UniformDistribution(0.5, 1);
            // Добавляем связь между рассогласованным и включающимся нейроном
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
