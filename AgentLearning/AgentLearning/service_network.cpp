#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "service.h"

#include "service_network.h"

extern const double Empty;

// Функция создания пустой сети из пулов
TPoolNetwork* CreatePoolNetwork()
{
   TPoolNetwork* NewPoolNetwork = new TPoolNetwork;

   NewPoolNetwork->PoolsStructure = NULL;
   NewPoolNetwork->ConnectionQuantity = 0;
   NewPoolNetwork->ConnectionQuantity = 0;
   NewPoolNetwork->PredConnectionQuantity = 0;
   NewPoolNetwork->Parents[0] = 0;
   NewPoolNetwork->Parents[1] = 0;

   return NewPoolNetwork;
}


// Удаление сети из пулов
TPoolNetwork* DeletePoolNetwork(TPoolNetwork* PoolNetwork)
{
   ErasePoolNetwork(PoolNetwork);

   delete PoolNetwork;

   return NULL;
}

// Функция создания и заполнения новой связи между пулами
TPoolConnection* CreatePoolConnection(int ID, double WeightMean, double WeightVariance, bool Enabled, double InnNumber, int DisStep, TNeuralPool* PrePool, TNeuralPool* PostPool, TPoolConnection* next /*=NULL*/)
{
   TPoolConnection* NewPoolConnection = new TPoolConnection;

   NewPoolConnection->ID = ID;
   NewPoolConnection->WeightMean = WeightMean;
   NewPoolConnection->WeightVariance = WeightVariance;
   NewPoolConnection->Enabled = Enabled;
   NewPoolConnection->InnNumber = InnNumber;
   NewPoolConnection->DisStep = DisStep;
   NewPoolConnection->PrePool = PrePool;
   NewPoolConnection->PostPool = PostPool;
   NewPoolConnection->next=next;

   return NewPoolConnection;
}

// Функция создание и заполнения предикторной связи между пулами
TPoolPredConnection* CreatePoolPredConnection(int ID, bool Enabled, double InnNumber, int DisStep, TNeuralPool* PrePool, TNeuralPool* PostPool, TPoolPredConnection* next /*=NULL*/)
{
   TPoolPredConnection* NewPoolPredConnection = new TPoolPredConnection;

   NewPoolPredConnection->ID = ID;
   NewPoolPredConnection->Enabled = Enabled;
   NewPoolPredConnection->InnNumber = InnNumber;
   NewPoolPredConnection->DisStep = DisStep;
   NewPoolPredConnection->PrePool = PrePool;
   NewPoolPredConnection->PostPool = PostPool;
   NewPoolPredConnection->next = next;

   return NewPoolPredConnection;

}

// Функция создания и заполнения нового нейронального пула
TNeuralPool* CreateNeuralPool(int ID, int Type, int Capacity, double BiasMean, double BiasVariance, int Layer, TPoolConnection* ConnectednessSet /*=NULL*/, TPoolPredConnection* PredConnectednessSet /*=NULL*/, TNeuralPool* next /*=NULL*/)
{
   TNeuralPool* NewNeuralPool = new TNeuralPool;

   NewNeuralPool->ID = ID;
   NewNeuralPool->Type = Type;
   NewNeuralPool->Capacity = Capacity;
   NewNeuralPool->BiasMean = BiasMean;
   NewNeuralPool->BiasVariance = BiasVariance;
   NewNeuralPool->Layer = Layer;
   NewNeuralPool->ConnectednessSet = ConnectednessSet;
   NewNeuralPool->PredConnectednessSet = PredConnectednessSet;
   NewNeuralPool->next = next;

   return NewNeuralPool;
}

// Функция создания пустой нейронной сети
TNeuralNetwork* CreateNeuralNetwork()
{
   TNeuralNetwork* NewNeuralNetwork = new TNeuralNetwork;

   NewNeuralNetwork->NetworkStructure = NULL;
   NewNeuralNetwork->NeuronQuantity = 0;
   NewNeuralNetwork->SynapseQuantity = 0;
   NewNeuralNetwork->PredConnectionQuantity = 0;
   NewNeuralNetwork->Reward = 0;

   return NewNeuralNetwork;
}

// Удаление нейронной сети
TNeuralNetwork* DeleteNeuralNetwork(TNeuralNetwork* NeuralNetwork)
{
   EraseNeuralNetwork(NeuralNetwork);

   delete NeuralNetwork;

   return NULL;
}

// Функция создания и заполнения нового синапса
TSynapse* CreateSynapse(int ID, double Weight, bool Enabled, TNeuron* PreNeuron, TNeuron* PostNeuron, TSynapse* next /*=NULL*/)
{
   TSynapse* NewSynapse = new TSynapse;

   NewSynapse->ID = ID;
   NewSynapse->Weight = Weight;
   NewSynapse->Enabled = Enabled;
   NewSynapse->PreNeuron = PreNeuron;
   NewSynapse->PostNeuron = PostNeuron;
   NewSynapse->next=next;

   return NewSynapse;
}

// Функция создание и заполнения предикторной связи
TPredConnection* CreatePredConnection(int ID, bool Enabled, TNeuron* PreNeuron, TNeuron* PostNeuron, TPredConnection* next /*=NULL*/)
{
   TPredConnection* NewPredConnection = new TPredConnection;

   NewPredConnection->ID = ID;
   NewPredConnection->Enabled = Enabled;
   NewPredConnection->PreNeuron = PreNeuron;
   NewPredConnection->PostNeuron = PostNeuron;
   NewPredConnection->next = next;

   return NewPredConnection;

}

// Функция создания и заполнения нового нейрона
TNeuron* CreateNeuron(int ID, int Type, double Bias, int Layer, bool Active, TNeuralPool* ParentPool, int ParentPoolID, TSynapse* InSynapses /*=NULL*/, TPredConnection* PredConnections /*=NULL*/, TNeuron* next /*=NULL*/)
{
   TNeuron* NewNeuron = new TNeuron;

   NewNeuron->ID = ID;
   NewNeuron->Type = Type;
   NewNeuron->Bias = Bias;
   NewNeuron->Layer = Layer;
   NewNeuron->CurrentOut = 0;
   NewNeuron->PreviousOut = 0;
   NewNeuron->Potential = 0;
   NewNeuron->Active = Active;
   NewNeuron->ParentPool = ParentPool;
   NewNeuron->ParentPoolID = ParentPoolID;
   NewNeuron->InSynapses = InSynapses;
   NewNeuron->PredConnections = PredConnections;
   NewNeuron->next = next;

   return NewNeuron;
}

// Функция получения ссылки на пул с соответствующим ID
TNeuralPool* GetPointer2Pool(TPoolNetwork* PoolNetwork, int PoolID)
{
   TNeuralPool* CurrentPool = PoolNetwork->PoolsStructure;
   while ((CurrentPool != NULL) && (CurrentPool->ID != PoolID))
      CurrentPool = CurrentPool->next;

   return CurrentPool;
}

TPoolConnection* GetPointer2PoolConnection(TPoolNetwork* PoolNetwork, int PoolConnectionID)
{
   TNeuralPool* CurrentPool = PoolNetwork->PoolsStructure;
   TPoolConnection* CurrentConnection = NULL;
   while ((CurrentPool != NULL) && (CurrentConnection->ID != PoolConnectionID))
   {
      CurrentConnection = CurrentPool->ConnectednessSet;
      while ((CurrentConnection != NULL) && (CurrentConnection->ID != PoolConnectionID))
         CurrentConnection = CurrentConnection->next;
      CurrentPool = CurrentPool->next;
   }

   return CurrentConnection;
}

TPoolConnection* GetPointer2PoolConnection(TPoolNetwork* PoolNetwork, int PrePoolID, int PostPoolID)
{
   TNeuralPool* CurrentPool = PoolNetwork->PoolsStructure;
   TPoolConnection* CurrentConnection = NULL;
   while ((CurrentPool != NULL) && (CurrentPool->ID != PostPoolID))
      CurrentPool = CurrentPool->next;

   if (CurrentPool != NULL)
   {
      CurrentConnection = CurrentPool->ConnectednessSet;
      while ((CurrentConnection != NULL) && (CurrentConnection->PrePool->ID != PrePoolID))
         CurrentConnection = CurrentConnection->next;
   }

   return CurrentConnection;
}

TPoolPredConnection* GetPointer2PoolPredConnection(TPoolNetwork* PoolNetwork, int PoolPredConnectionID)
{
   TNeuralPool* CurrentPool = PoolNetwork->PoolsStructure;
   TPoolPredConnection* CurrentConnection = NULL;
   while ((CurrentPool != NULL) && (CurrentConnection->ID != PoolPredConnectionID))
   {
      CurrentConnection = CurrentPool->PredConnectednessSet;
      while ((CurrentConnection != NULL) && (CurrentConnection->ID != PoolPredConnectionID))
         CurrentConnection = CurrentConnection->next;
      CurrentPool = CurrentPool->next;
   }

   return CurrentConnection;

}

TPoolPredConnection* GetPointer2PoolPredConnection(TPoolNetwork* PoolNetwork, int PrePoolID, int PostPoolID)
{
   TNeuralPool* CurrentPool = PoolNetwork->PoolsStructure;
   TPoolPredConnection* CurrentConnection = NULL;
   while ((CurrentPool != NULL) && (CurrentPool->ID != PostPoolID))
      CurrentPool = CurrentPool->next;

   if (CurrentPool != NULL)
   {
      CurrentConnection = CurrentPool->PredConnectednessSet;
      while ((CurrentConnection != NULL) && (CurrentConnection->PrePool->ID != PostPoolID))
         CurrentConnection = CurrentConnection->next;
   }

   return CurrentConnection;

}

TNeuron* GetPointer2Neuron(TNeuralNetwork* Network, int NeuronID)
{
   TNeuron* CurrentNeuron = Network->NetworkStructure;
   while ((CurrentNeuron != NULL) && (CurrentNeuron->ID != NeuronID))
      CurrentNeuron = CurrentNeuron->next;

   return CurrentNeuron;
}

TSynapse* GetPointer2Synapse(TNeuralNetwork* Network, int SynapseID)
{
   TNeuron* CurrentNeuron = Network->NetworkStructure;
   TSynapse* CurrentSynapse = NULL;
   while ((CurrentNeuron != NULL) && (CurrentSynapse->ID != SynapseID))
   {
      CurrentSynapse = CurrentNeuron->InSynapses;
      while ((CurrentSynapse != NULL) && (CurrentSynapse->ID != SynapseID))
         CurrentSynapse = CurrentSynapse->next;
      CurrentNeuron = CurrentNeuron->next;
   }

   return CurrentSynapse;
}

TPredConnection* GetPointer2PredConnection(TNeuralNetwork* Network, int PredConnectionID)
{
   TNeuron* CurrentNeuron = Network->NetworkStructure;
   TPredConnection* CurrentPredConnection = NULL;
   while ((CurrentNeuron != NULL) && (CurrentPredConnection->ID != PredConnectionID))
   {
      CurrentPredConnection = CurrentNeuron->PredConnections;
      while ((CurrentPredConnection != NULL) && (CurrentPredConnection->ID != PredConnectionID))
         CurrentPredConnection = CurrentPredConnection->next;
      CurrentNeuron = CurrentNeuron->next;
   }

   return CurrentPredConnection;
}

 // Процедура стирания нейронной сети
void EraseNeuralNetwork(TNeuralNetwork* NeuralNetwork)
{
   TNeuron* CurNeuron = NeuralNetwork->NetworkStructure;
   TSynapse* CurSynapse;
   TPredConnection* CurPredConnection;
   while (CurNeuron!=NULL) // Проходимся по всем нейронам
   {
      CurSynapse = CurNeuron->InSynapses;
      while (CurSynapse!=NULL) // Проходимся по все входящим синапсам текущего нейрона
      {
         TSynapse* tmpSynapse = CurSynapse;
         CurSynapse = CurSynapse->next;
         delete tmpSynapse;
      }
      CurPredConnection = CurNeuron->PredConnections;
      while (CurPredConnection!=NULL) // Проходимся по всем входящим предикторным связям
      {
         TPredConnection* tmpPredConnection = CurPredConnection;
         CurPredConnection = CurPredConnection->next;
         delete tmpPredConnection;
      }

      TNeuron* tmpNeuron = CurNeuron;
      CurNeuron = CurNeuron->next;
      delete tmpNeuron;
   }

   NeuralNetwork->NetworkStructure = NULL;

   NeuralNetwork->NeuronQuantity = 0;
   NeuralNetwork->SynapseQuantity = 0;
   NeuralNetwork->PredConnectionQuantity = 0;
   NeuralNetwork->Reward = 0;
}

// Процедура стирания сети из пулов
void ErasePoolNetwork(TPoolNetwork* PoolNetwork)
{
   TNeuralPool* CurPool = PoolNetwork->PoolsStructure;
   TPoolConnection* CurPoolConnection;
   TPoolPredConnection* CurPoolPredConnection;
   while (CurPool!=NULL) // Проходимся по всем пулам
   {
      CurPoolConnection = CurPool->ConnectednessSet;
      while (CurPoolConnection!=NULL) // Проходимся по все входящим связям текущего пула
      {
         TPoolConnection* tmpPoolConnection = CurPoolConnection;
         CurPoolConnection = CurPoolConnection->next;
         delete tmpPoolConnection;
      }
      CurPoolPredConnection = CurPool->PredConnectednessSet;
      while (CurPoolPredConnection!=NULL) // Проходимся по всем входящим предикторным связям
      {
         TPoolPredConnection* tmpPoolPredConnection = CurPoolPredConnection;
         CurPoolPredConnection = CurPoolPredConnection->next;
         delete tmpPoolPredConnection;
      }

      TNeuralPool* tmpPool = CurPool;
      CurPool = CurPool->next;
      delete tmpPool;
   }

   PoolNetwork->PoolsStructure = NULL;

   PoolNetwork->PoolQuantity = 0;
   PoolNetwork->ConnectionQuantity = 0;
   PoolNetwork->PredConnectionQuantity = 0;
   PoolNetwork->Parents[0] = 0;
   PoolNetwork->Parents[1] = 0;
}

// Процедура копирования одной сети в другую
void CopyPoolNetwork(TPoolNetwork* NewPoolNetwork, TPoolNetwork* ParentPoolNetwork)
{
   TNeuralPool* CurParentPool;
   TNeuralPool* CurNewPool = NULL;
   TPoolConnection* CurParentConnection;
   TPoolConnection* CurNewConnection = NULL;
   TPoolPredConnection* CurParentPredConnection;
   TPoolPredConnection* CurNewPredConnection = NULL;

   ErasePoolNetwork(NewPoolNetwork); // Стираем принимающую сеть (если там чтото есть)

   // Сначала копируем все нейроны в принимающую сеть
   CurParentPool = ParentPoolNetwork->PoolsStructure;
   while (CurParentPool!=NULL) // Проходимся по всем пулам родителя
   {
      // Создаем новый пул принимающей сети
      if (CurNewPool == NULL) // Если это первый пул принимающей сети
      {
         NewPoolNetwork->PoolsStructure = CreateNeuralPool(CurParentPool->ID, CurParentPool->Type, CurParentPool->Capacity, CurParentPool->BiasMean, CurParentPool->BiasVariance, CurParentPool->Layer, NULL, NULL, NULL);
         CurNewPool = NewPoolNetwork->PoolsStructure;
      }
      else
      {
         CurNewPool->next = CreateNeuralPool(CurParentPool->ID, CurParentPool->Type, CurParentPool->Capacity, CurParentPool->BiasMean, CurParentPool->BiasVariance, CurParentPool->Layer, NULL, NULL, NULL);
         CurNewPool = CurNewPool->next;
      }

      CurParentPool = CurParentPool->next;
   }

   // Теперь копируем все связи
   CurParentPool = ParentPoolNetwork->PoolsStructure;
   CurNewPool = NewPoolNetwork->PoolsStructure;
   while (CurParentPool!=NULL) // Проходимся по всем пулам родителя
   {
      CurParentConnection = CurParentPool->ConnectednessSet;
      while (CurParentConnection!=NULL) // Проходимся по всем связям родителя
      {
         //Ищем нужный пресинаптический пул
         TNeuralPool* CurPrePool = GetPointer2Pool(NewPoolNetwork, CurParentConnection->PrePool->ID);
         //Создаем новый синапс
         if (CurNewConnection == NULL) // Если это первая связь для пула принимающей сети
         {
            CurNewPool->ConnectednessSet = CreatePoolConnection(CurParentConnection->ID, CurParentConnection->WeightMean, CurParentConnection->WeightVariance, CurParentConnection->Enabled, CurParentConnection->InnNumber, CurParentConnection->DisStep, CurPrePool, CurNewPool, NULL);
            CurNewConnection = CurNewPool->ConnectednessSet;
         }
         else
         {
            CurNewConnection->next = CreatePoolConnection(CurParentConnection->ID, CurParentConnection->WeightMean, CurParentConnection->WeightVariance, CurParentConnection->Enabled, CurParentConnection->InnNumber, CurParentConnection->DisStep, CurPrePool, CurNewPool, NULL);
            CurNewConnection = CurNewConnection->next;
         }
         CurParentConnection = CurParentConnection->next;
      }
      CurNewConnection = NULL; // Так как теперь будут связи нового нейрона

      CurParentPredConnection = CurParentPool->PredConnectednessSet;
      while (CurParentPredConnection!=NULL) // Проходимся по всем пред. связям родителя
      {
         //Ищем нужный пресинаптический пул
         TNeuralPool* CurPrePool = GetPointer2Pool(NewPoolNetwork, CurParentPredConnection->PrePool->ID);
         //Создаем новую пред. связь
         if (CurNewPredConnection == NULL) // Если это первая пред. связь для пула принимающей сети
         {
            CurNewPool->PredConnectednessSet = CreatePoolPredConnection(CurParentPredConnection->ID, CurParentPredConnection->Enabled, CurParentPredConnection->InnNumber, CurParentPredConnection->DisStep, CurPrePool, CurNewPool, NULL);
            CurNewPredConnection = CurNewPool->PredConnectednessSet;
         }
         else
         {
            CurNewPredConnection->next = CreatePoolPredConnection(CurParentPredConnection->ID, CurParentPredConnection->Enabled, CurParentPredConnection->InnNumber, CurParentPredConnection->DisStep, CurPrePool, CurNewPool, NULL);
            CurNewPredConnection = CurNewPredConnection->next;
         }
         CurParentPredConnection = CurParentPredConnection->next;
      }
      CurNewPredConnection = NULL;

      CurParentPool = CurParentPool->next;
      CurNewPool = CurNewPool->next;
   }

   NewPoolNetwork->PoolQuantity = ParentPoolNetwork->PoolQuantity;
   NewPoolNetwork->ConnectionQuantity = ParentPoolNetwork->ConnectionQuantity;
   NewPoolNetwork->PredConnectionQuantity = ParentPoolNetwork->PredConnectionQuantity;
   NewPoolNetwork->Reward = ParentPoolNetwork->Reward;
}



// Процедура копирования одной сети в другую
void CopyNeuralNetwork(TNeuralNetwork* NewNetwork, TNeuralNetwork* ParentNetwork)
{
   TNeuron* CurParentNeuron;
   TNeuron* CurNewNeuron=NULL;
   TSynapse* CurParentSynapse;
   TSynapse* CurNewSynapse=NULL;

   EraseNeuralNetwork(NewNetwork); // Стираем принимающую сеть (если там чтото есть)

   // Сначала копируем все нейроны в принимающую сеть
   CurParentNeuron = ParentNetwork->NetworkStructure;
   while (CurParentNeuron!=NULL) // Проходимся по всем нейронам родителя
   {
      // Создаем новый нейрон принимающей сети

      if (CurNewNeuron == NULL) // Если это первый нейрон принимающей сети
      {
         NewNetwork->NetworkStructure = CreateNeuron(CurParentNeuron->ID, CurParentNeuron->Type, CurParentNeuron->Bias, CurParentNeuron->Layer, CurParentNeuron->Active, CurParentNeuron->ParentPool, CurParentNeuron->ParentPoolID, NULL, NULL,  NULL);
         CurNewNeuron = NewNetwork->NetworkStructure;
      }
      else
      {
         CurNewNeuron->next = CreateNeuron(CurParentNeuron->ID, CurParentNeuron->Type, CurParentNeuron->Bias, CurParentNeuron->Layer, CurParentNeuron->Active, CurParentNeuron->ParentPool, CurParentNeuron->ParentPoolID, NULL, NULL,  NULL);
         CurNewNeuron = CurNewNeuron->next;
      }

      CurParentNeuron = CurParentNeuron->next;
   }


   // Теперь копируем все синапсы
   CurParentNeuron = ParentNetwork->NetworkStructure;
   CurNewNeuron = NewNetwork->NetworkStructure;
   while (CurParentNeuron!=NULL) // Проходимся по всем нейронам родителя
   {
      CurParentSynapse = CurParentNeuron->InSynapses;
      while (CurParentSynapse!=NULL) // Проходимся по всей синапсам родителя
      {
         //Ищем нужный пресинаптический нейрон
         TNeuron* CurPreNeuron = GetPointer2Neuron(NewNetwork, CurParentSynapse->PreNeuron->ID);
         //Создаем новый синапс
         if (CurNewSynapse == NULL) // Если это первый синапс для нейрона принимающей сети
         {
            CurNewNeuron->InSynapses = CreateSynapse(CurParentSynapse->ID, CurParentSynapse->Weight, CurParentSynapse->Enabled, CurPreNeuron, CurNewNeuron, NULL);
            CurNewSynapse = CurNewNeuron->InSynapses;
         }
         else
         {
            CurNewSynapse->next = CreateSynapse(CurParentSynapse->ID, CurParentSynapse->Weight, CurParentSynapse->Enabled, CurPreNeuron, CurNewNeuron, NULL);
            CurNewSynapse = CurNewSynapse->next;
         }
         CurParentSynapse = CurParentSynapse->next;
      }
      CurNewSynapse = NULL; // Так как теперь будут синапсы нового нейрона
      CurParentNeuron = CurParentNeuron->next;
      CurNewNeuron = CurNewNeuron->next;

      TPredConnection* CurParentPredConnection = CurParentNeuron->PredConnections;
      TPredConnection* CurNewPredConnection = NULL;
      while (CurParentPredConnection != NULL )
      {
         TNeuron* CurPreNeuron = GetPointer2Neuron(NewNetwork, CurParentPredConnection->PreNeuron->ID);
         if (CurNewPredConnection == NULL) // Если это первый синапс для нейрона принимающей сети
         {
            CurNewNeuron->PredConnections = CreatePredConnection(CurParentPredConnection->ID, CurParentPredConnection->Enabled, CurPreNeuron, CurNewNeuron, NULL);
            CurNewPredConnection = CurNewNeuron->PredConnections;
         }
         else
         {
            CurNewPredConnection->next = CreatePredConnection(CurParentPredConnection->ID, CurParentPredConnection->Enabled, CurPreNeuron, CurNewNeuron, NULL);
            CurNewPredConnection = CurNewPredConnection->next;
         }
         CurParentPredConnection = CurParentPredConnection->next;
      }

      CurParentNeuron = CurParentNeuron->next;
      CurNewNeuron = CurNewNeuron->next;
   }


   NewNetwork->NeuronQuantity = ParentNetwork->NeuronQuantity;
   NewNetwork->SynapseQuantity = ParentNetwork->SynapseQuantity;
   NewNetwork->PredConnectionQuantity = ParentNetwork->PredConnectionQuantity;
   NewNetwork->Reward = ParentNetwork->Reward;
}

// Обнуление внутреннего состояния сети
void ResetNeuralNetwork(TNeuralNetwork* NeuralNetwork)
{
   TNeuron* CurrentNeuron = NeuralNetwork->NetworkStructure;
   while (CurrentNeuron != NULL)
   {
      CurrentNeuron->CurrentOut = 0;
      CurrentNeuron->PreviousOut = 0;
      CurrentNeuron->Potential = 0;
      CurrentNeuron = CurrentNeuron->next;
   }
}

// Процедура исправления ID нейронов (в случае если нейроны были удалены)
void fix_neurons_id(TNeuralNetwork* neural_network)
{
   TNeuron* current_neuron = neural_network->NetworkStructure;
   int neurons_quantity = 0;
   while (NULL != current_neuron)
   {
      current_neuron->ID = ++neurons_quantity;
      current_neuron = current_neuron->next;
   }
   neural_network->NeuronQuantity = neurons_quantity;
}

// Процедура исправления ID синапсов (в случае если синапсы были удалены)
void fix_synapses_id(TNeuralNetwork* neural_network)
{
   TNeuron* current_neuron = neural_network->NetworkStructure;
   TSynapse* current_synapse;
   int synapses_quantity = 0;
   while (NULL != current_neuron)
   {
      current_synapse = current_neuron->InSynapses;
      while (NULL != current_synapse)
      {
         current_synapse->ID = ++synapses_quantity;
         current_synapse = current_synapse->next;
      }
      current_neuron = current_neuron->next;
   }
   neural_network->SynapseQuantity = synapses_quantity;
}

// Процедура исправления ID предикторных связей (в случае если связи были удалены)
void fix_pred_connections_id(TNeuralNetwork* neural_network)
{
   TNeuron* current_neuron = neural_network->NetworkStructure;
   TPredConnection* current_pred_connection;
   int pred_connections_quantity = 0;
   while (NULL != current_neuron)
   {
      current_pred_connection = current_neuron->PredConnections;
      while (NULL != current_pred_connection)
      {
         current_pred_connection->ID = ++pred_connections_quantity;
         current_pred_connection = current_pred_connection->next;
      }
      current_neuron = current_neuron->next;
   }
   neural_network->PredConnectionQuantity = pred_connections_quantity;
}

//Рекурентная процедура моделирования нейронной сети - определение выхода нейрона с ID (для стандартного режима)
double NeuronOutput(TNeuron* Neuron, int* InputVector, bool RecurrentMode)
{
   if (RecurrentMode) // Если мы пришли по обратной связи
      return Neuron->PreviousOut;
   else
      if (Neuron->CurrentOut==Empty) // если выход еще неозначен
      {
            // Если нейрон входной
            if (!Neuron->Type)
            {
               Neuron->Potential = 0;
               Neuron->CurrentOut = static_cast<double>(InputVector[(Neuron->ID)-1]);
            }
            else
            {
               double h = 0; // Потенциал нейрона
               h += Neuron->Bias; // Добавляем смещение нейрона
               TSynapse* CurSynapse = Neuron->InSynapses;
               while (CurSynapse!=NULL) // Проходим по все входящим связям текущего нейрона
               {
                  if (CurSynapse->Enabled) // Если синапс включен
                  {
                     double CurInput; // Переменная входа от текущего синапса
                     if ( CurSynapse->PreNeuron->Layer < Neuron->Layer ) // если связь прямая
                        CurInput = NeuronOutput(CurSynapse->PreNeuron, InputVector, 0);
                     else
                        CurInput = NeuronOutput(CurSynapse->PreNeuron, InputVector, 1);

                     if (CurInput > 0.5) // Если пресинаптический нейрон активен
                        h +=  (CurSynapse->Weight) * CurInput;
                  }
                  CurSynapse = CurSynapse->next;
               }
               Neuron->Potential = h;
               if (Neuron->Active) // Если нейрон активный
                  Neuron->CurrentOut = tansig(Neuron->Potential);
               else // Если нейрон молчащий
                  Neuron->CurrentOut = 0;
            }
      }

   return Neuron->CurrentOut;
}

// Процедура прогона нейронной сети и определения ее выхода (для стандартного режима работы)
void NetworkOutput_standart(TNeuralNetwork* Network, int* InputVector, double* OutputVector)
{
   TNeuron* Neuron = Network->NetworkStructure;
   while (Neuron!=NULL) // Перевод текущих связей в предыдущие и очистка текущих выходов
   {
      Neuron->PreviousOut = Neuron->CurrentOut;
      Neuron->CurrentOut = Empty;
      Neuron = Neuron->next;
   }

   Neuron = Network->NetworkStructure;
   int OutputBit = 0; // Текущий номер незаполненного бита выходного вектора
   while (Neuron!=NULL) // Определяем выходы всех нейронов и выходной вектор
   {
      double Output = NeuronOutput(Neuron, InputVector, 0);
      if (Neuron->Type == 2) // Если нейрон выходной, то сразу записываем значение в выходной вектор
      {
         OutputVector[OutputBit] = Output;
         OutputBit++;
      }
      Neuron = Neuron->next;
   }
}


// Процедура прогона нейронной сети и определения ее выхода (для синхронного режима работы)
void NetworkOutput_synch(TNeuralNetwork* Network, int* InputVector, double* OutputVector)
{
   TNeuron* Neuron = Network->NetworkStructure;
   while (Neuron!=NULL) // Перевод текущих связей в предыдущие
   {
      Neuron->PreviousOut = Neuron->CurrentOut;
      Neuron->CurrentOut = 0;
      Neuron = Neuron->next;
   }

   int OutputBit = 0; // Текущий номер незаполненного бита выходного вектора
   Neuron = Network->NetworkStructure;
   while (Neuron != NULL) // Проход по всем нейронам и определение их текущего выхода на основе предыдущих выходов сети
   {
      if (Neuron->Type != 0) // Если нейрон не сенсорный
      {
         TSynapse* CurrentSynapse = Neuron->InSynapses;
         double h=0; // Потенциал текущего нейрона
         h += Neuron->Bias; // Добавляем смещение нейрона
         while (CurrentSynapse!=NULL) // Проход по всем входящим связям нейрона
         {
            if (CurrentSynapse->PreNeuron->PreviousOut > 0.5) // Если пресинаптический нейрон был активен
               h += CurrentSynapse->Weight * CurrentSynapse->PreNeuron->PreviousOut;
            CurrentSynapse = CurrentSynapse->next;
         }
         Neuron->CurrentOut = tansig(h);
         if (Neuron->Type == 2) // Если нейрон выходной, то сразу записываем значение в выходной вектор
         {
            OutputVector[OutputBit] = Neuron->CurrentOut;
            OutputBit++;
         }
      }
      else
         Neuron->CurrentOut = static_cast<double>(InputVector[Neuron->ID - 1]);
   }
}

//Рекурентная процедура определения выхода нейрона с возможностью спонтанной активации
double NeuronOutput_spontan(TNeuron* Neuron, bool RecurrentMode, double SpontaneousActivityProb)
{

   if (RecurrentMode) // Если мы пришли по обратной связи
      return Neuron->PreviousOut;
   else
      if (Neuron->CurrentOut==Empty) // если выход еще неозначен
      {
         if (!Neuron->Active)
         {
            Neuron->CurrentOut = 0;
            Neuron->Potential = 0;
         }
         else
            // Если нейрон входной
            if (!Neuron->Type)
            {
               Neuron->Potential = 0;
               // Если нейрон спонтанно активируется
               if (UniformDistribution(0, 1) <= SpontaneousActivityProb)
                  Neuron->CurrentOut = UniformDistribution(0.5, 1, false);//
               else
                  Neuron->CurrentOut = 0;
            }
            else
            {
               double h = 0; // Потенциал нейрона
               h += Neuron->Bias; // Добавляем смещение нейрона
               TSynapse* CurSynapse = Neuron->InSynapses;
               while (CurSynapse!=NULL) // Проходим по все входящим связям текущего нейрона
               {
                  if (CurSynapse->Enabled) // Если синапс включен
                  {
                     double CurInput; // Переменная входа от текущего синапса
                     if ( CurSynapse->PreNeuron->Layer < Neuron->Layer ) // если связь прямая
                        CurInput = NeuronOutput_spontan(CurSynapse->PreNeuron, 0, SpontaneousActivityProb);
                     else
                        CurInput = NeuronOutput_spontan(CurSynapse->PreNeuron, 1, SpontaneousActivityProb);

                     if (CurInput > 0.5) // Если пресинаптический нейрон активен
                        h +=  (CurSynapse->Weight) * CurInput;
                  }
                  CurSynapse = CurSynapse->next;
               }
               Neuron->Potential = h;
               // Если нейрон спонтанно активируется
               if (UniformDistribution(0, 1) <= SpontaneousActivityProb)
                  Neuron->CurrentOut = UniformDistribution(0.5, 1, false);
               else
                  Neuron->CurrentOut=tansig(Neuron->Potential);
            }
      }

   return Neuron->CurrentOut;
}

// Процедура прогона нейронной сети для процедуры первичного системогенеза
void NetworkOutput_primarysys(TNeuralNetwork* PrimaryNetwork, double SpontaneousActivityProb)
{
   TNeuron* Neuron = PrimaryNetwork->NetworkStructure;
   while (Neuron!=NULL) // Перевод текущих связей в предыдущие и очистка текущих выходов
   {
      Neuron->PreviousOut = Neuron->CurrentOut;
      Neuron->CurrentOut = Empty;
      Neuron = Neuron->next;
   }

   Neuron = PrimaryNetwork->NetworkStructure;
   while (Neuron!=NULL) // Определяем выходы всех нейронов и выходной вектор
   {
      NeuronOutput_spontan(Neuron, 0, SpontaneousActivityProb);
      Neuron = Neuron->next;
   }
}

//Процедура заполнения нейронной сети из файла
void FillNextNeuralNetwork(TNeuralNetwork* NeuralNetwork, FILE* hNetworkFile, int EnvironmentResolution, int OutputResolution, int NetworkMode)
{
   EraseNeuralNetwork(NeuralNetwork); // Очистка сети

   char StrFromFile[255];
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем технический номер сети

   TNeuron* CurrentNeuron = NULL;
   //---------- Заполнение генома нейронов --------------
   if (!NetworkMode) // Если режим "одновременный"
   {
      fscanf(hNetworkFile, "%s", StrFromFile);

      while (strcmp(StrFromFile,"|")) // Cчитываем до разделителя между нейронами и синапсами
      {
         ++ (NeuralNetwork->NeuronQuantity);
         int NeuronLayer = atoi(StrFromFile);
         int NeuronType;
         if (NeuralNetwork->NeuronQuantity <= EnvironmentResolution) NeuronType = 0; // Входные и выходные нейроны идут в начале списка
         else
            if (NeuralNetwork->NeuronQuantity <= (EnvironmentResolution + OutputResolution) )
               NeuronType = 2;
            else
               NeuronType = 1;

         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем смещение нейрона
         double Bias = atof(StrFromFile);
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер родительского пула нейрона
         int ParentPoolID = atoi(StrFromFile);
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак активности

         if (CurrentNeuron == NULL) // Если нет еще ни одного нейрона
         {
            NeuralNetwork->NetworkStructure = CreateNeuron(NeuralNetwork->NeuronQuantity, NeuronType, Bias, NeuronLayer, atoi(StrFromFile), NULL, ParentPoolID, NULL, NULL, NULL);
            CurrentNeuron = NeuralNetwork->NetworkStructure;
         }
         else
         {
            CurrentNeuron->next = CreateNeuron(NeuralNetwork->NeuronQuantity, NeuronType, Bias, NeuronLayer, atoi(StrFromFile), NULL, ParentPoolID, NULL, NULL, NULL);
            CurrentNeuron = CurrentNeuron->next;
         }
         fscanf(hNetworkFile, "%s", StrFromFile);
      }
   }
   else // Если режим "синхронный"
   {
      NeuralNetwork->NeuronQuantity = atoi(StrFromFile);
      int NeuronID=0;
      while (NeuronID < NeuralNetwork->NeuronQuantity)
      {
         int NeuronType;
         if (NeuronID < EnvironmentResolution)
            NeuronType = 0;
         else
            if (NeuronID < EnvironmentResolution + OutputResolution)
               NeuronType = 2;
            else
               NeuronType = 0;

         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем смещение нейрона
         double Bias =  atof(StrFromFile);
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер родительского пула нейрона
         int ParentPoolID = atoi(StrFromFile);
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак активности

         if (CurrentNeuron == NULL) // Если нет еще ни одного нейрона
         {
            NeuralNetwork->NetworkStructure = CreateNeuron(++NeuronID, NeuronType, Bias, 0, atoi(StrFromFile), NULL, ParentPoolID, NULL, NULL, NULL);
            CurrentNeuron = NeuralNetwork->NetworkStructure;
         }
         else
         {
            CurrentNeuron->next = CreateNeuron(++NeuronID, NeuronType, Bias, 0, atoi(StrFromFile), NULL, ParentPoolID, NULL, NULL, NULL);
            CurrentNeuron = CurrentNeuron->next;
         }
      }

      fscanf(hNetworkFile, "%s", StrFromFile);
   }
   //---------------------------------------------------
   //----------- Заполнение генома связей агента ------------
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID пресинаптического нейрона
   while (strcmp(StrFromFile,"||")) // Считываем до разделителя между синапсами и предикторными связями
   {
      ++(NeuralNetwork->SynapseQuantity);

      TNeuron* PreNeuron = GetPointer2Neuron(NeuralNetwork, atoi(StrFromFile)); // Ищем пресинаптический нейрон

      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID постсинаптического нейрона
      CurrentNeuron = GetPointer2Neuron(NeuralNetwork, atoi(StrFromFile)); // Ищем постсинаптический нейрон

      TSynapse* CurrentSynapse; // Последний синапс постсинаптического нейрона
      CurrentSynapse = CurrentNeuron->InSynapses;
      if (CurrentSynapse != NULL) // Если уже есть входящие синапсы у нейрона
         while (CurrentSynapse->next!=NULL) // Считываем до последнего синапса
            CurrentSynapse = CurrentSynapse->next;

      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем весовой коэффициент
      double SynapseWeight = atof(StrFromFile);
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак экспрессии
      bool SynapseEnabled = atoi(StrFromFile);

      if (CurrentSynapse != NULL) // Если уже есть входящие синапсы у нейрона
         CurrentSynapse->next = CreateSynapse(NeuralNetwork->SynapseQuantity, SynapseWeight, SynapseEnabled, PreNeuron, CurrentNeuron, NULL);
      else
         CurrentNeuron->InSynapses = CreateSynapse(NeuralNetwork->SynapseQuantity, SynapseWeight, SynapseEnabled, PreNeuron, CurrentNeuron, NULL);

      fscanf(hNetworkFile, "%s", StrFromFile);
   }
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID пресинаптического нейрона для предикторной связи

   while (strcmp(StrFromFile,"|||")) // Считываем до разделителя между предикторными связями и следующей сетью
   {
      ++(NeuralNetwork->PredConnectionQuantity);
      TNeuron* PreNeuron = GetPointer2Neuron(NeuralNetwork, atoi(StrFromFile));
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID постсинаптического нейрона
      CurrentNeuron = GetPointer2Neuron(NeuralNetwork, atoi(StrFromFile)); // Ищем постсинаптический нейрон

      TPredConnection* CurrentPredConnection = CurrentNeuron->PredConnections;
      if (CurrentPredConnection != NULL) // Если у нейрона есть хотя бы одна предикторная связь
         while (CurrentPredConnection != NULL) // Ищем последнюю
            CurrentPredConnection = CurrentPredConnection->next;
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак экспрессии
      bool PredConnectionEnabled = atoi(StrFromFile);
      if (CurrentPredConnection != NULL) // Если уже есть входящие предикторные связи у нейрона
         CurrentPredConnection->next = CreatePredConnection(NeuralNetwork->PredConnectionQuantity, PredConnectionEnabled, PreNeuron, CurrentNeuron, NULL);
      else
         CurrentNeuron->PredConnections = CreatePredConnection(NeuralNetwork->PredConnectionQuantity, PredConnectionEnabled, PreNeuron, CurrentNeuron, NULL);

      fscanf(hNetworkFile, "%s", StrFromFile);
   }
   //-----------------------------------------------
}

// Пропуск записи нейронной сети в файле
void SkipNeuralNetworkInFile(FILE* hNetworkFile, int NetworkMode)
{
   char StrFromFile[255];
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем технический номер сети

    //---------- Считывание генома нейронов --------------
   if (NetworkMode) // Если режим "одновременный"
   {
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер слоя нейрона
      while (strcmp(StrFromFile,"|")) // Cчитываем до разделителя между нейронами и синапсами
      {
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем смещение нейрона
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер родительского пула нейрона
         fscanf(hNetworkFile, "%s", StrFromFile); // Признак активности

         fscanf(hNetworkFile, "%s", StrFromFile); // Слой нейрона
      }
   }
   else // Если режим "синхронный"
   {
      int NeuronsQuantity = atoi(StrFromFile);
      for (int CurrentNeuron=0; CurrentNeuron < NeuronsQuantity; ++CurrentNeuron)
      {
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем смещение нейрона
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер родительского пула нейрона
         fscanf(hNetworkFile, "%s", StrFromFile); // Признак активности
      }

      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем разделитель между нейронами и синапсами
   }
    //----------- Заполнение генома связей агента ------------
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID пресинаптического нейрона

   while (strcmp(StrFromFile,"||")) // Считываем до разделителя между агентами
   {
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID постсинаптического нейрона
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем весовой коэффициент
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак экспрессии
      fscanf(hNetworkFile, "%s", StrFromFile); // ID пресинаптического нейрона
   }

   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID пресинаптического нейрона для предикторной связи

   while (strcmp(StrFromFile,"|||")) // Считываем до разделителя между связью и следующей сетью
   {
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID постсинаптического нейрона
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак экспрессии
      fscanf(hNetworkFile, "%s", StrFromFile); // ID пресинаптического нейрона
   }
}

// Загрузка конкретной нейронной сети (по номеру) из файла
void FillSpecificNeuralNetwork(TNeuralNetwork* NeuralNetwork, char* NetworkFilename, int NetworkNumber, int EnvironmentResolution, int OutputResolution, int NetworkMode)
{
   FILE* hNetworkFile = fopen(NetworkFilename, "r");
   // Пропускаем всех агентов, которые идут до нужного
   for (int CurrentNetwork=1; CurrentNetwork < NetworkNumber; ++CurrentNetwork)
      SkipNeuralNetworkInFile(hNetworkFile, NetworkMode);

   FillNextNeuralNetwork(NeuralNetwork, hNetworkFile, EnvironmentResolution, OutputResolution, NetworkMode);

   fclose(hNetworkFile);
}

//Процедура заполнения сети из пулов из файла
void FillNextPoolNetwork(TPoolNetwork* PoolNetwork, FILE* hNetworkFile, int EnvironmentResolution, int OutputResolution, int NetworkMode)
{
   ErasePoolNetwork(PoolNetwork); // Очистка сети

   char StrFromFile[255];
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем технический номер сети
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер первого родителя
   PoolNetwork->Parents[0] = atoi(StrFromFile);
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер второго родителя
   PoolNetwork->Parents[1] = atoi(StrFromFile);

   TNeuralPool* CurrentPool = NULL;
   //---------- Заполнение генома пулов --------------
   if (!NetworkMode) // Если режим "одновременный"
   {
      fscanf(hNetworkFile, "%s", StrFromFile);

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

         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем среднее смещения пула
         double BiasMean = atof(StrFromFile);
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем дисперсию смещения пула
         double BiasVariance = atof(StrFromFile);
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем размерность пула

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
         fscanf(hNetworkFile, "%s", StrFromFile);
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

         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем среднее смещения пула
         double BiasMean = atof(StrFromFile);
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем дисперсию смещения пула
         double BiasVariance = atof(StrFromFile);
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем размерность пула

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

      fscanf(hNetworkFile, "%s", StrFromFile);
   }
   //---------------------------------------------------
   //----------- Заполнение генома связей сети ------------
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID пресинаптического пула
   while (strcmp(StrFromFile,"||")) // Считываем до разделителя между связями и предикторными связями
   {
      ++(PoolNetwork->ConnectionQuantity);

      TNeuralPool* PrePool = GetPointer2Pool(PoolNetwork, atoi(StrFromFile)); // Ищем пресинаптический пул

      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID постсинаптического пула
      CurrentPool = GetPointer2Pool(PoolNetwork, atoi(StrFromFile)); // Ищем постсинаптический пул

      TPoolConnection* CurrentPoolConnection; // Последняя связь постсинаптического пула
      CurrentPoolConnection = CurrentPool->ConnectednessSet;
      if (CurrentPoolConnection != NULL) // Если уже есть входящие связи у пула
         while (CurrentPoolConnection->next!=NULL) // Считываем до последней связи
            CurrentPoolConnection = CurrentPoolConnection->next;

      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем среднее веса связи
      double WeightMean = atof(StrFromFile);
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем дисперсию веса связи
      double WeightDisp = atof(StrFromFile);
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак экспрессии
      bool ConnectionEnabled = atoi(StrFromFile);
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем поколение выключения
      int ConnectionDisStep = atoi(StrFromFile);
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер инновации
      double ConnectionInnNumber = atof(StrFromFile);

      if (CurrentPoolConnection != NULL) // Если уже есть входящие синапсы у нейрона
         CurrentPoolConnection->next = CreatePoolConnection(PoolNetwork->ConnectionQuantity, WeightMean, WeightDisp, ConnectionEnabled, ConnectionInnNumber, ConnectionDisStep, PrePool, CurrentPool, NULL);
      else
         CurrentPool->ConnectednessSet = CreatePoolConnection(PoolNetwork->ConnectionQuantity, WeightMean, WeightDisp, ConnectionEnabled, ConnectionInnNumber, ConnectionDisStep, PrePool, CurrentPool, NULL);

      fscanf(hNetworkFile, "%s", StrFromFile);
   }

   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID пресинаптического пула для предикторной связи
   while (strcmp(StrFromFile,"|||")) // Считываем до разделителя между предикторными связями и следующей сетью
   {
      ++(PoolNetwork->PredConnectionQuantity);

      TNeuralPool* PrePool = GetPointer2Pool(PoolNetwork, atoi(StrFromFile)); // Ищем пресинаптический пул

      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID постсинаптического пула
      CurrentPool = GetPointer2Pool(PoolNetwork, atoi(StrFromFile)); // Ищем постсинаптический пул

      TPoolPredConnection* CurrentPoolPredConnection; // Последняя пред. связь постсинаптического пула
      CurrentPoolPredConnection = CurrentPool->PredConnectednessSet;
      if (CurrentPoolPredConnection != NULL) // Если уже есть входящие связи у пула
         while (CurrentPoolPredConnection->next!=NULL) // Считываем до последней связи
            CurrentPoolPredConnection = CurrentPoolPredConnection->next;

      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак экспрессии
      bool PredConnectionEnabled = atoi(StrFromFile);
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем поколение выключения
      int PredConnectionDisStep = atoi(StrFromFile);
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер инновации
      double PredConnectionInnNumber = atof(StrFromFile);

      if (CurrentPoolPredConnection != NULL) // Если уже есть входящие предикторные связи у нейрона
         CurrentPoolPredConnection->next = CreatePoolPredConnection(PoolNetwork->PredConnectionQuantity, PredConnectionEnabled, PredConnectionInnNumber, PredConnectionDisStep, PrePool, CurrentPool, NULL);
      else
         CurrentPool->PredConnectednessSet = CreatePoolPredConnection(PoolNetwork->PredConnectionQuantity, PredConnectionEnabled, PredConnectionInnNumber, PredConnectionDisStep, PrePool, CurrentPool, NULL);

      fscanf(hNetworkFile, "%s", StrFromFile);
   }
   //-----------------------------------------------
}

// Пропуск записи сети из пулов в файле
void SkipPoolNetworkInFile(FILE* hNetworkFile, int NetworkMode)
{
   char StrFromFile[255];
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем технический номер сети
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер первого родителя
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер второго родителя

    //---------- Считывание генома пулов --------------
   if (NetworkMode) // Если режим "одновременный"
   {
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем номер слоя пула
      while (strcmp(StrFromFile,"|")) // Cчитываем до разделителя между нейронами и синапсами
      {
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем среднее смещения пула
         fscanf(hNetworkFile, "%s", StrFromFile); // Дисперсия смещения пула
         fscanf(hNetworkFile, "%s", StrFromFile); // Размерность пула

         fscanf(hNetworkFile, "%s", StrFromFile); // Слой пула
      }
   }
   else // Если режим "синхронный"
   {
      int NeuronsQuantity = atoi(StrFromFile);
      for (int CurrentNeuron=0; CurrentNeuron < NeuronsQuantity; ++CurrentNeuron)
      {
         fscanf(hNetworkFile, "%s", StrFromFile); // Считываем смещение пула
         fscanf(hNetworkFile, "%s", StrFromFile); // Дисперсия смещения пула
         fscanf(hNetworkFile, "%s", StrFromFile); // Размерность пула
      }

      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем разделитель между пулами и связями
   }
    //----------- Заполнение генома связей агента ------------
   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID пресинаптического пула

   while (strcmp(StrFromFile,"||")) // Считываем до разделителя между связями и предикторными связями
   {
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID постсинаптического пула

      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем среднее веса
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем дисперсию веса
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак экспрессии
      fscanf(hNetworkFile, "%s", StrFromFile); // Такт выключения
      fscanf(hNetworkFile, "%s", StrFromFile); // Номер инновации

      fscanf(hNetworkFile, "%s", StrFromFile); // ID пресинаптического пула
   }

   fscanf(hNetworkFile, "%s", StrFromFile); // Считываем ID пресинаптического пула для предикторной связи

   while (strcmp(StrFromFile,"|||")) // Считываем до разделителя между связью и следующей сетью
   {
      fscanf(hNetworkFile, "%s", StrFromFile); // Считываем признак экспрессии
      fscanf(hNetworkFile, "%s", StrFromFile); // Такт выключения
      fscanf(hNetworkFile, "%s", StrFromFile); // Номер инновации

      fscanf(hNetworkFile, "%s", StrFromFile); // ID пресинаптического пула
   }
}

// Загрузка конкретной сети из пулов (по номеру) из файла
void FillSpecificPoolNetwork(TPoolNetwork* PoolNetwork, char* NetworkFilename, int NetworkNumber, int EnvironmentResolution, int OutputResolution, int NetworkMode)
{
   FILE* hNetworkFile = fopen(NetworkFilename, "r");
   // Пропускаем всех агентов, которые идут до нужного
   for (int CurrentNetwork=1; CurrentNetwork<NetworkNumber; ++CurrentNetwork)
      SkipPoolNetworkInFile(hNetworkFile, NetworkMode);

   FillNextPoolNetwork(PoolNetwork, hNetworkFile, EnvironmentResolution, OutputResolution, NetworkMode);

   fclose(hNetworkFile);
}

// Процедура записи одной сети в файл
void RecordNeuralNetwork(TNeuralNetwork* NeuralNetwork, int NetworkNumber, FILE* hNetworkFile)
{
   TNeuron* CurrentNeuron;
   CurrentNeuron =NeuralNetwork->NetworkStructure;

   fprintf(hNetworkFile, "#%i\n", NetworkNumber);
   // Записываем все нейроны
   while (CurrentNeuron != NULL) // Проходимся по всем нейронам
   {
      fprintf(hNetworkFile, "%i\t%.3f\t%i\t%i\n", CurrentNeuron->Layer, CurrentNeuron->Bias, CurrentNeuron->ParentPoolID, CurrentNeuron->Active);
      CurrentNeuron = CurrentNeuron->next;
   }

   fprintf(hNetworkFile,"|\n"); // Записываем разделить между нейронами и синапсами
   // Записываем информацию о синапсах
   CurrentNeuron = NeuralNetwork->NetworkStructure;
   while (CurrentNeuron != NULL)
   {
      TSynapse* CurrentSynapse = CurrentNeuron->InSynapses;
      while (CurrentSynapse != NULL) // Проходимся по всем синапсам
      {
         fprintf(hNetworkFile, "%i\t%i\t%.4f\t%i\n", CurrentSynapse->PreNeuron->ID, CurrentSynapse->PostNeuron->ID, CurrentSynapse->Weight, CurrentSynapse->Enabled);
         CurrentSynapse = CurrentSynapse->next;
      }
      CurrentNeuron = CurrentNeuron->next;
   }
   fprintf(hNetworkFile,"||\n"); // Записываем разделитель между синапсами и предикторными связями

   // Записываем информацию о предикторных связях
   CurrentNeuron = NeuralNetwork->NetworkStructure;
   while (CurrentNeuron != NULL)
   {
      TPredConnection* CurrentPredConnection = CurrentNeuron->PredConnections;
      while (CurrentPredConnection != NULL) // Проходимся по всем предикторным связям
      {
         fprintf(hNetworkFile, "%i\t%i\t%i\n", CurrentPredConnection->PreNeuron->ID, CurrentPredConnection->PostNeuron->ID, CurrentPredConnection->Enabled);
         CurrentPredConnection = CurrentPredConnection->next;
      }
      CurrentNeuron = CurrentNeuron->next;
   }
   fprintf(hNetworkFile,"|||\n"); // Записываем разделитель между сетями
}

// Процедура записи одной сети из пулов в файл
void RecordPoolNetwork(TPoolNetwork* PoolNetwork, int NetworkNumber, FILE* hNetworkFile)
{
   TNeuralPool* CurrentPool;
   CurrentPool = PoolNetwork->PoolsStructure;

   fprintf(hNetworkFile, "#%i\t%i\t%i\n", NetworkNumber, PoolNetwork->Parents[0], PoolNetwork->Parents[1]);
   // Записываем все пулы
   while (CurrentPool != NULL) // Проходимся по всем пулам
   {
      fprintf(hNetworkFile, "%i\t%.3f\t%.3f\t%i\n", CurrentPool->Layer, CurrentPool->BiasMean, CurrentPool->BiasVariance, CurrentPool->Capacity);
      CurrentPool = CurrentPool->next;
   }

   fprintf(hNetworkFile,"|\n"); // Записываем разделить между пулами и связями
   // Записываем информацию о связях
   CurrentPool = PoolNetwork->PoolsStructure;
   while (CurrentPool != NULL)
   {
      TPoolConnection* CurrentPoolConnection = CurrentPool->ConnectednessSet;
      while (CurrentPoolConnection != NULL) // Проходимся по всем связям
      {
         fprintf(hNetworkFile, "%i\t%i\t%.4f\t%.4f\t%i\t%i\t%.1f\n", CurrentPoolConnection->PrePool->ID, CurrentPoolConnection->PostPool->ID, CurrentPoolConnection->WeightMean, CurrentPoolConnection->WeightVariance, CurrentPoolConnection->Enabled, CurrentPoolConnection->DisStep, CurrentPoolConnection->InnNumber);
         CurrentPoolConnection = CurrentPoolConnection->next;
      }
      CurrentPool = CurrentPool->next;
   }
   fprintf(hNetworkFile,"||\n"); // Записываем разделитель между связями и предикторными связями

   // Записываем информацию о предикторных связях
   CurrentPool = PoolNetwork->PoolsStructure;
   while (CurrentPool != NULL)
   {
      TPoolPredConnection* CurrentPoolPredConnection = CurrentPool->PredConnectednessSet;
      while (CurrentPoolPredConnection != NULL) // Проходимся по всем предикторным связям
      {
         fprintf(hNetworkFile, "%i\t%i\t%i\t%i\t%.1f\n", CurrentPoolPredConnection->PrePool->ID, CurrentPoolPredConnection->PostPool->ID, CurrentPoolPredConnection->Enabled, CurrentPoolPredConnection->DisStep, CurrentPoolPredConnection->InnNumber);
         CurrentPoolPredConnection = CurrentPoolPredConnection->next;
      }
      CurrentPool = CurrentPool->next;
   }

   fprintf(hNetworkFile,"|||\n"); // Записываем разделитель между сетями
}

// Преобразование нейронной сети в файл .dot для дальнейшей визуализации в виде графа
void NeuralNetwork2Dot(TNeuralNetwork* NeuralNetwork, const char NetworkFigFile[])
{
   FILE* hNetworkFigFile = fopen(NetworkFigFile, "w");

   fprintf(hNetworkFigFile, "digraph G { \n\trankdir=LR;\n"); // Инициализируем и указываем, что слои должны следовать слево на право

   TNeuron* CurrentNeuron = NeuralNetwork->NetworkStructure;
   // Подсчитываем кол-во слоев в сети
   while ((CurrentNeuron != NULL) && (CurrentNeuron->Type != 2)) // Находим первый выходной нейрон
      CurrentNeuron = CurrentNeuron->next;
   int LayersQuantity = CurrentNeuron->Layer;
   // Подсчитываем кол-во пулов в сети
   CurrentNeuron = NeuralNetwork->NetworkStructure;
   int PoolsQuantity = 0;
   while (CurrentNeuron != NULL)
   {
      if (CurrentNeuron->ParentPoolID > PoolsQuantity)
         PoolsQuantity = CurrentNeuron->ParentPoolID;
      CurrentNeuron = CurrentNeuron->next;
   }
   // Записываем нейроны послойно
   for (int CurrentLayer = 1; CurrentLayer <= LayersQuantity; ++CurrentLayer)
   {
      CurrentNeuron = NeuralNetwork->NetworkStructure;
      fprintf(hNetworkFigFile, "\t{ rank = same; \n");
      while (CurrentNeuron != NULL)
      {
         if ((CurrentNeuron->Layer == CurrentLayer) && (CurrentNeuron->Active)) //!! Тут не отображаем неактивные нейроны
         {
            char Hex[3];
            // Определяем насыщенность цвета нейрона (отражает принадлежность к пулу)
            DecToHex((PoolsQuantity - CurrentNeuron->ParentPoolID + 1) * (255-30) / PoolsQuantity + 30, Hex, 2); // Делаем так чтобы все нейроны были не совсем темные, при этом белые былибы входные
            char Color[7]; // Цвет нейрона
            sprintf(Color, "%s%s%s", Hex, Hex, Hex); // Оттенок серого
            char Active[100];
            sprintf(Active,"");
            if ((CurrentNeuron->Active) && (CurrentNeuron->CurrentOut == 0) && (CurrentNeuron->Potential != 0)) // Если нейрон только что стал активен, то обводим его красным (нейрон только что стал активным, если у него ненулевой потенциал, но выход нулевой)
               sprintf(Active, ", penwidth=2.0, color=\"red\"");

            fprintf(hNetworkFigFile, "\t\tsubgraph cluster%i{\n \t\t\tstyle=dashed;\n\t\t\tcolor=lightgrey;\n\t\t\t\"%i\"[label=\"%i; %i\", shape=\"circle\", style=filled, fillcolor=\"#%s\"%s];\n\t\t}\n", CurrentNeuron->ParentPoolID, CurrentNeuron->ID, CurrentNeuron->ID, CurrentNeuron->ParentPoolID, Color, Active);
         }
         CurrentNeuron = CurrentNeuron->next;
      }
      fprintf(hNetworkFigFile,"\t}\n");
   }
   // Записываем синапсы
   double MaxWeightValue = 1.0;
   CurrentNeuron = NeuralNetwork->NetworkStructure;
   TSynapse* CurrentSynapse = NULL;
   while (CurrentNeuron != NULL)
   {
      CurrentSynapse = CurrentNeuron->InSynapses;
      while (CurrentSynapse != NULL)
      {
         if ((CurrentSynapse->Enabled) && (CurrentSynapse->PreNeuron->Active) && (CurrentSynapse->PostNeuron->Active)) //!! Отображаем связи только между активными нейронами
         {
            char Hex[3];
            DecToHex(static_cast<int>(min(abs(255 * CurrentSynapse->Weight / MaxWeightValue), 255.0)), Hex, 2); // Определяем насыщенность цвета линии
            char Color[7]; // Цвет линии
            if (CurrentSynapse->Weight < 0)
               sprintf(Color, "0000%s", Hex); // Оттенок синего
            else
               sprintf(Color, "%s0000", Hex); // Оттенок красного
            //printf("\n%i\t%i\t%.3f\t%s\t%s\n", CurrentSynapse->PreNeuron->ID, CurrentSynapse->PostNeuron->ID, CurrentSynapse->Weight,Hex, Color);

            fprintf(hNetworkFigFile, "\t\"%i\" -> \"%i\" [arrowsize=0.7, color=\"#%s\", penwidth=2.0];\n", CurrentSynapse->PreNeuron->ID, CurrentSynapse->PostNeuron->ID, Color);

            //fprintf(hNetworkFigFile, "\t\"%i\" -> \"%i\" [label=\"%.3f\", fontsize=10, arrowsize=0.7, color=\"#%s\", penwidth=2.0];\n", CurrentSynapse->PreNeuron->ID, CurrentSynapse->PostNeuron->ID, CurrentSynapse->Weight, Color);
         }
         CurrentSynapse = CurrentSynapse->next;
      }
      CurrentNeuron = CurrentNeuron->next;
   }
   CurrentNeuron = NeuralNetwork->NetworkStructure;
   // Записываем предикторные связи
   TPredConnection* CurrentPredConnection = NULL;
   while (CurrentNeuron != NULL)
   {
      CurrentPredConnection = CurrentNeuron->PredConnections;
      while (CurrentPredConnection != NULL)
      {
         if ((CurrentPredConnection -> Enabled) && (CurrentPredConnection->PreNeuron->Active) && (CurrentPredConnection->PostNeuron->Active)) //!! Отображаем связи только между активными нейронами
            fprintf(hNetworkFigFile, "\t\"%i\" -> \"%i\" [style=dashed, arrowsize=0.7, color=\"#000000\", penwidth=2.0];\n", CurrentPredConnection->PreNeuron->ID, CurrentPredConnection->PostNeuron->ID);
         CurrentPredConnection = CurrentPredConnection->next;
      }
      CurrentNeuron = CurrentNeuron->next;
   }
   fprintf(hNetworkFigFile, "}");
   fclose(hNetworkFigFile);

   char ExecLine[255];
   sprintf(ExecLine, "dot -Tjpg %s -o %s.jpg", NetworkFigFile, NetworkFigFile);
   system(ExecLine);
}

// Преобразование нейронной сети в файл .dot для дальнейшей визуализации в виде графа, но без слоев
void NeuralNetwork2UnLayeredDot(TNeuralNetwork* NeuralNetwork, const char NetworkFigFile[])
{
   FILE* hNetworkFigFile = fopen(NetworkFigFile, "w");

   fprintf(hNetworkFigFile, "digraph G { \n"); // Инициализируем

   TNeuron* CurrentNeuron = NeuralNetwork->NetworkStructure;
   // Подсчитываем кол-во пулов в сети
   CurrentNeuron = NeuralNetwork->NetworkStructure;
   int PoolsQuantity = 0;
   while (CurrentNeuron != NULL)
   {
      if (CurrentNeuron->ParentPoolID > PoolsQuantity)
         PoolsQuantity = CurrentNeuron->ParentPoolID;
      CurrentNeuron = CurrentNeuron->next;
   }
   // Записываем нейроны популам
   for (int CurrentPool = 1; CurrentPool <= PoolsQuantity; ++CurrentPool)
   {
      CurrentNeuron = NeuralNetwork->NetworkStructure;
      fprintf(hNetworkFigFile, "\tsubgraph cluster%i{ \t\tstyle=dashed;\n\t\tcolor=lightgrey\n", CurrentPool);
      while (CurrentNeuron != NULL)
      {
         if (CurrentNeuron->ParentPoolID == CurrentPool)
            fprintf(hNetworkFigFile, "\t\t\"%i\"[label=\"%i\", shape=\"circle\"];\n", CurrentNeuron->ID, CurrentNeuron->ID);

         CurrentNeuron = CurrentNeuron->next;
      }
      fprintf(hNetworkFigFile,"\t}\n");
   }
   // Записываем синапсы
   double MaxWeightValue = 1.0;
   CurrentNeuron = NeuralNetwork->NetworkStructure;
   TSynapse* CurrentSynapse = NULL;
   while (CurrentNeuron != NULL)
   {
      CurrentSynapse = CurrentNeuron->InSynapses;
      while (CurrentSynapse != NULL)
      {
         if (CurrentSynapse->Enabled)
         {
            char Hex[3];
            DecToHex(static_cast<int>(min(abs(255 * CurrentSynapse->Weight / MaxWeightValue), 255.0)), Hex, 2); // Определяем насыщенность цвета линии
            char Color[7]; // Цвет линии
            if (CurrentSynapse->Weight < 0)
               sprintf(Color, "0000%s", Hex); // Оттенок синего
            else
               sprintf(Color, "%s0000", Hex); // Оттенок красного
            printf("\n%i\t%i\t%.3f\t%s\t%s\n", CurrentSynapse->PreNeuron->ID, CurrentSynapse->PostNeuron->ID, CurrentSynapse->Weight,Hex, Color);

            fprintf(hNetworkFigFile, "\t\"%i\" -> \"%i\" [arrowsize=0.7, color=\"#%s\", penwidth=2.0];\n", CurrentSynapse->PreNeuron->ID, CurrentSynapse->PostNeuron->ID, Color);

            //fprintf(hNetworkFigFile, "\t\"%i\" -> \"%i\" [label=\"%.3f\", fontsize=10, arrowsize=0.7, color=\"#%s\", penwidth=2.0];\n", CurrentSynapse->PreNeuron->ID, CurrentSynapse->PostNeuron->ID, CurrentSynapse->Weight, Color);
         }
         CurrentSynapse = CurrentSynapse->next;
      }
      CurrentNeuron = CurrentNeuron->next;
   }
   CurrentNeuron = NeuralNetwork->NetworkStructure;
   // Записываем предикторные связи
   TPredConnection* CurrentPredConnection = NULL;
   while (CurrentNeuron != NULL)
   {
      CurrentPredConnection = CurrentNeuron->PredConnections;
      while (CurrentPredConnection != NULL)
      {
         if (CurrentPredConnection -> Enabled)
            fprintf(hNetworkFigFile, "\t\"%i\" -> \"%i\" [style=dashed, arrowsize=0.7, color=\"#000000\", penwidth=2.0];\n", CurrentPredConnection->PreNeuron->ID, CurrentPredConnection->PostNeuron->ID);
         CurrentPredConnection = CurrentPredConnection->next;
      }
      CurrentNeuron = CurrentNeuron->next;
   }
   fprintf(hNetworkFigFile, "}");
   fclose(hNetworkFigFile);

   char ExecLine[255];
   sprintf(ExecLine, "dot -Tjpg %s -o %s.jpg", NetworkFigFile, NetworkFigFile);
   system(ExecLine);
}

// Преобразование сети из пулов в файл .dot для дальнейшей визуализации в виде графа
void PoolNetwork2Dot(TPoolNetwork* PoolNetwork, const char NetworkFigFile[])
{
   FILE* hNetworkFigFile = fopen(NetworkFigFile, "w");

   fprintf(hNetworkFigFile, "digraph G { \n\trankdir=LR;\n"); // Инициализируем и указываем, что слои должны следовать слево на право

   TNeuralPool* CurrentPool = PoolNetwork->PoolsStructure;
   // Подсчитываем кол-во слоев в сети
   while ((CurrentPool != NULL) && (CurrentPool->Type != 2)) // Находим первый выходной пул
      CurrentPool = CurrentPool->next;
   int LayersQuantity = CurrentPool->Layer;
   // Записываем пулы послойно
   for (int CurrentLayer = 1; CurrentLayer <= LayersQuantity; ++CurrentLayer)
   {
      CurrentPool = PoolNetwork->PoolsStructure;
      fprintf(hNetworkFigFile, "\t{ rank = same; \n");
      while (CurrentPool != NULL)
      {
         if (CurrentPool->Layer == CurrentLayer)
            fprintf(hNetworkFigFile, "\t\t\"%i\"[label=\"%i\", shape=\"circle\"];\n", CurrentPool->ID, CurrentPool->ID);
         CurrentPool = CurrentPool->next;
      }
      fprintf(hNetworkFigFile,"\t}\n");
   }
   // Записываем связи
   double MaxWeightValue = 1.0;
   CurrentPool = PoolNetwork->PoolsStructure;
   TPoolConnection* CurrentConnection = NULL;
   while (CurrentPool != NULL)
   {
      CurrentConnection = CurrentPool->ConnectednessSet;
      while (CurrentConnection != NULL)
      {
         if (CurrentConnection->Enabled)
         {
            char Hex[3];
            DecToHex(static_cast<int>(min(abs(255 * CurrentConnection->WeightMean / MaxWeightValue), 255.0)), Hex, 2); // Определяем насыщенность цвета линии
            char Color[7]; // Цвет линии
            if (CurrentConnection->WeightMean < 0)
               sprintf(Color, "0000%s", Hex); // Оттенок синего
            else
               sprintf(Color, "%s0000", Hex); // Оттенок красного
            //printf("\n%i\t%i\t%.3f\t%s\t%s\n", CurrentConnection->PrePool->ID, CurrentConnection->PostPool->ID, CurrentConnection->WeightMean,Hex, Color);

            fprintf(hNetworkFigFile, "\t\"%i\" -> \"%i\" [label=\"%.3f\", fontsize=10, arrowsize=0.7, color=\"#%s\", penwidth=2.0];\n", CurrentConnection->PrePool->ID, CurrentConnection->PostPool->ID, CurrentConnection->WeightMean, Color);
         }
         CurrentConnection = CurrentConnection->next;
      }
      CurrentPool = CurrentPool->next;
   }
   CurrentPool = PoolNetwork->PoolsStructure;
   // Записываем предикторные связи
   TPoolPredConnection* CurrentPredConnection = NULL;
   while (CurrentPool != NULL)
   {
      CurrentPredConnection = CurrentPool->PredConnectednessSet;
      while (CurrentPredConnection != NULL)
      {
         if (CurrentPredConnection -> Enabled)
            fprintf(hNetworkFigFile, "\t\"%i\" -> \"%i\" [style=dashed, arrowsize=0.7, color=\"#000000\", penwidth=2.0];\n", CurrentPredConnection->PrePool->ID, CurrentPredConnection->PostPool->ID);
         CurrentPredConnection = CurrentPredConnection->next;
      }
      CurrentPool = CurrentPool->next;
   }
   fprintf(hNetworkFigFile, "}");
   fclose(hNetworkFigFile);

   char ExecLine[255];
   sprintf(ExecLine, "dot -Tjpg %s -o %s.jpg", NetworkFigFile, NetworkFigFile);
   system(ExecLine);
}
