#include <math.h>
#include <stdlib.h>

#include "types.h"
#include "service.h"
#include "service_network.h"

#include "mutation.h"


// Процедура мутации - удаления из агента связей, которые выключены более некоторого количества поколений
void MutationDeleteConPop(TPoolNetwork* KidPoolNetwork, int EvolutionStep, int DisLimit)
{
   TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
   while (CurPool!=NULL) // Проход по всем пулам
   {
      TPoolConnection* CurConnection; // Переменная текущей связи
      TPoolConnection* PrevConnection = NULL; // Переменная предыдущей связи
      CurConnection = CurPool->ConnectednessSet;
      while (CurConnection!=NULL) // Проход по всем входящим связям текущего пула
         if ((EvolutionStep - CurConnection->DisStep >= DisLimit) && (CurConnection->DisStep)) // Если эта связь была выключен и ее уже надо удалить из генома
            if (PrevConnection==NULL) // Если это первая связь
            {
               CurPool->ConnectednessSet = CurConnection->next; // то меняем указатель в постсинаптическом пуле
               delete CurConnection;
               CurConnection = CurPool->ConnectednessSet;
               KidPoolNetwork->ConnectionQuantity--;
            }
            else
            {
               PrevConnection->next = CurConnection->next; // иначе меняем указатель в предыдущей связи
               delete CurConnection;
               CurConnection = PrevConnection->next;
               KidPoolNetwork->ConnectionQuantity--;

            }
         else // Если связь не нужно удалять
         {
            PrevConnection = CurConnection;
            CurConnection = CurConnection->next;
         }
      CurPool = CurPool->next;
   }

   // Теперь мы выравниваем все ID связей чтобы максимальный ID совпадал с количеством связей(то что ID могут поменяться местами - это неважно)
   CurPool = KidPoolNetwork->PoolsStructure;
   int ConnectionCount=0;
   while (CurPool!=NULL)
   {
      TPoolConnection* CurConnection;
      CurConnection = CurPool->ConnectednessSet;
      while (CurConnection!=NULL)
      {
         CurConnection->ID = ++ConnectionCount;
         CurConnection = CurConnection->next;
      }
      CurPool = CurPool->next;
   }
}

// Процедура мутации - удаления из агента предикторных связей, которые выключены более некоторого количества поколений
void MutationDeletePredConPop(TPoolNetwork* KidPoolNetwork, int EvolutionStep, int DisLimit)
{
   TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
   while (CurPool!=NULL) // Проход по всем пулам
   {
      TPoolPredConnection* CurPredConnection; // Переменная текущей предикторной связи
      TPoolPredConnection* PrevPredConnection = NULL; // Переменная предыдущей предикторной связи
      CurPredConnection = CurPool->PredConnectednessSet;
      while (CurPredConnection!=NULL) // Проход по всем входящим предикторным связям текущего пула
         if ((EvolutionStep - CurPredConnection->DisStep >= DisLimit) && (CurPredConnection->DisStep)) // Если эта предикторная связь была выключена и ее уже надо удалить из генома
            if (PrevPredConnection==NULL) // Если это первая предикторная связь
            {
               CurPool->PredConnectednessSet = CurPredConnection->next; // то меняем указатель в постсинаптическом пуле
               delete CurPredConnection;
               CurPredConnection = CurPool->PredConnectednessSet;
               KidPoolNetwork->PredConnectionQuantity--;
            }
            else
            {
               PrevPredConnection->next = CurPredConnection->next; // иначе меняем указатель в предыдущей связи
               delete CurPredConnection;
               CurPredConnection = PrevPredConnection->next;
               KidPoolNetwork->PredConnectionQuantity--;

            }
         else // Если предикторную связь не нужно удалять
         {
            PrevPredConnection = CurPredConnection;
            CurPredConnection = CurPredConnection->next;
         }
      CurPool = CurPool->next;
   }

   // Теперь мы выравниваем все ID предикторных связей чтобы максимальный ID совпадал с количеством связей(то что ID могут поменяться местами - это неважно)
   CurPool = KidPoolNetwork->PoolsStructure;
   int PredConnectionCount=0;
   while (CurPool!=NULL)
   {
      TPoolPredConnection* CurPredConnection;
      CurPredConnection = CurPool->PredConnectednessSet;
      while (CurPredConnection!=NULL)
      {
         CurPredConnection->ID = ++PredConnectionCount;
         CurPredConnection = CurPredConnection->next;
      }
      CurPool = CurPool->next;
   }
}

// Процедура мутации - мутация весовых коэффициентов связи
void MutationConWeight(TPoolNetwork* KidPoolNetwork, double MutWeightProbability, double MutWeightMeanDisp, double MutWeightDispDisp)
{
   TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
   while (CurPool!=NULL) // Пробегаем по всем пулам
   {
      if (UniformDistribution(0,1) < MutWeightProbability)
      {
         CurPool->BiasMean += UniformDistribution(-MutWeightMeanDisp, MutWeightMeanDisp);// Мутируем смещение нейрона
			// Важно, чтобы дисперсия была не меньше нуля
         CurPool->BiasVariance = abs(CurPool->BiasVariance + UniformDistribution(-MutWeightDispDisp, MutWeightDispDisp));
      }
      //printf("%.2f\n",(rand() % ((int) (2*MutWeightDisp*1000) + 1) - MutWeightDisp*1000)/1000.0);
      TPoolConnection* CurConnection = CurPool->ConnectednessSet;
      while (CurConnection!=NULL) // Пробегаем по всем входящим связям текущего пула
      {
         if ( (UniformDistribution(0,1) < MutWeightProbability) && (CurConnection->Enabled) ) // Если мутация имеет место быть и связь включена
         {
            CurConnection->WeightMean += UniformDistribution(-MutWeightMeanDisp, MutWeightMeanDisp); //Проводим мутацию
				// Важно, чтобы дисперсия была не меньше нуля
            CurConnection->WeightVariance = abs(CurConnection->WeightVariance + UniformDistribution(-MutWeightDispDisp, MutWeightDispDisp));
         }
         CurConnection = CurConnection->next;
      }
      CurPool = CurPool->next;
   }
}

// Процедура мутации - включения/выключения связей
void MutationEnDisConnection(TPoolNetwork* KidPoolNetwork, double EnConProb, double DisConProb, int EvolutionStep)
{
   TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
   while (CurPool!=NULL) // Проходимся по всей пулам
   {
      TPoolConnection* CurConnection = CurPool->ConnectednessSet;
      while (CurConnection!=NULL) // Проходимся по всем входным связям текущего пула
      {
         if (CurConnection->Enabled) // Если связь включена
         {
            if (UniformDistribution(0, 1) < DisConProb) // Если мутация имеет место быть
            {
               CurConnection->Enabled = false;
               CurConnection->DisStep = EvolutionStep;
            }
         }
         else // Если связь выключена
            if (UniformDistribution(0, 1) < EnConProb)
            {
               CurConnection->Enabled = true;
               CurConnection->DisStep = 0;
            }
         CurConnection = CurConnection->next;
      }
      CurPool = CurPool->next;
   }
}

// Процедура мутации - включения/выключения предикторных связей
void MutationEnDisPredConnection(TPoolNetwork* KidPoolNetwork, double EnConProb, double DisConProb, int EvolutionStep)
{
   TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
   while (CurPool!=NULL) // Проходимся по всей пулам
   {
      TPoolPredConnection* CurPredConnection = CurPool->PredConnectednessSet;
      while (CurPredConnection!=NULL) // Проходимся по всем входным предикторным связям текущего пула
      {
         if (CurPredConnection->Enabled) // Если связь включена
         {
            if (UniformDistribution(0, 1) < DisConProb) // Если мутация имеет место быть
            {
               CurPredConnection->Enabled = false;
               CurPredConnection->DisStep = EvolutionStep;
            }
         }
         else // Если связь выключена
            if (UniformDistribution(0, 1) < EnConProb)
            {
               CurPredConnection->Enabled = true;
               CurPredConnection->DisStep = 0;
            }
         CurPredConnection = CurPredConnection->next;
      }
      CurPool = CurPool->next;
   }
}

// Процедура мутации - добавление связи
void MutationAddConnection(TPoolNetwork* KidPoolNetwork, double AddConnectionProb, bool EvolutionMode, double* CurrentInnovationNumber, double InitialDevelopSynapseProb)
{
   if (UniformDistribution(0, 1) < AddConnectionProb) // Если имеет место мутация
   {
      TNeuralPool* CurPool;
      TPoolConnection* CurConnection;

      // Нахождения значений размерности среды и размерности выходного вектора
      int EnResolution = 0;
      int OutResolution = 0;
      CurPool = KidPoolNetwork->PoolsStructure;
      while ((CurPool!=NULL) && (CurPool->Type != 1)) // Так как входные и выходные пулы в самом начале генома, то считываем не до конца
      {
         if (CurPool->Type == 0) EnResolution++;
         if (CurPool->Type == 2) OutResolution++;
         CurPool = CurPool->next;
      }

      int PrePoolID, PostPoolID; // Пресинаптический и постсинаптический пул потенциальной связи
      int OutCheck=0; // Переменная признак нахождения корректной связи
      int TryCount = 0; //Количество ложных генераций связи - введено, чтобы не было зацикливания
      do // Цикл нахождения корректной связи
      {
         PrePoolID = UniformDiscreteDistribution(1, KidPoolNetwork->PoolQuantity);
         PostPoolID = UniformDiscreteDistribution(1, KidPoolNetwork->PoolQuantity);

         CurConnection = GetPointer2PoolConnection(KidPoolNetwork, PrePoolID, PostPoolID);

         OutCheck=0;
         if (CurConnection==NULL) // Еcли нет такой связи в сети
            if (PostPoolID > EnResolution) // Если постсинаптический пул не сенсорный нейрон
               if ( (PostPoolID > EnResolution+OutResolution) || (PrePoolID <= EnResolution) || (PrePoolID > EnResolution+OutResolution) ) OutCheck=1;
                           //Если постсинаптический и пресинаптический пулы не являются одновременно выходными, то значит мы нашли нужную связь
         TryCount++;
           // или тоже самое - ( (FaultQt>=1001) || ( (tmpConGen==NULL) && ( (OutNode>(EnResolution+OutResolution)) || (InNode<=EnResolution) || (InNode>(EnResolution+OutResolution)) ) && (OutNode>EnResolution) ) )
      }while ((!OutCheck) && (TryCount<1000)); // Пока не найдем корректную связь или превысим допустимое кол-во попыток

       // Если найдена корректная связь для добавления
      if (OutCheck)
      {
         TNeuralPool* PrePool = GetPointer2Pool(KidPoolNetwork, PrePoolID);// Находим пресинаптический пул
         CurPool = GetPointer2Pool(KidPoolNetwork, PostPoolID);
         if (CurPool->ConnectednessSet==NULL) // Если у постсинаптического пула еще нет входных связей (В текущий момент в переменной текущего пула ссылка на постсинаптический пул)
         {
            // Создаем новую связь
				CurPool->ConnectednessSet = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, UniformDistribution(-1, 1), 0, 1, ++(*CurrentInnovationNumber), 0, InitialDevelopSynapseProb, PrePool, CurPool, NULL); //!! Инициализация мат.ожидания [-1;1], дисперсии [-0.1;0.1]
            CurConnection = CurPool->ConnectednessSet;
         }
         else
         {
            CurConnection = CurPool->ConnectednessSet;
            while (CurConnection->next!=NULL) CurConnection = CurConnection->next; // Находим последнюю связь
            // Создаем новую связь
				CurConnection->next = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, UniformDistribution(-1, 1), 0, 1, ++(*CurrentInnovationNumber), 0, InitialDevelopSynapseProb, PrePool, CurPool, NULL); //!! Инициализация мат.ожидания [-1;1], дисперсии [-0.1;0.1]
            CurConnection=CurConnection->next;
         }
         // Детекция необходимости сдвига постсинаптического пула в следующий слой, если появилась связь между пулами одного слоя (используется для модернизированного эволюционного алгоритма)
         if ( (EvolutionMode) && (CurConnection->PrePool->Layer == CurConnection->PostPool->Layer) && (CurConnection->PrePool->ID != CurConnection->PostPool->ID) )
         {
            CurPool = KidPoolNetwork->PoolsStructure;
            while (CurPool!=NULL) // Проходимся по всем пулам
            {
               if ( (CurPool->Layer > CurConnection->PrePool->Layer) || (CurPool->ID == CurConnection->PostPool->ID) ) // Сдвигаем постсинапчиеский пул и все пулы в более дальних слоях
                  CurPool->Layer++;
               CurPool = CurPool->next;
            }
         }
      }
   }
}

// Процедура мутации - добавление предикторной связи связи
void MutationAddPredConnection(TPoolNetwork* KidPoolNetwork, double AddPredConnectionProb, double* CurrentPredInnovationNumber, double InitialDevelopPredConProb)
{
   if (UniformDistribution(0, 1) < AddPredConnectionProb) // Если имеет место мутация
   {
      TNeuralPool* CurPool;
      TPoolPredConnection* CurPredConnection;

      // Нахождения значений размерности среды и размерности выходного вектора
      int EnResolution = 0;
      int OutResolution = 0;
      CurPool = KidPoolNetwork->PoolsStructure;
      while ((CurPool!=NULL) && (CurPool->Type != 1)) // Так как входные и выходные пулы в самом начале генома, то считываем не до конца
      {
         if (CurPool->Type == 0) EnResolution++;
         if (CurPool->Type == 2) OutResolution++;
         CurPool = CurPool->next;
      }

      int PrePoolID, PostPoolID; // Пресинаптический и постсинаптический пул потенциальной предикторной связи
      int OutCheck=0; // Переменная признак нахождения корректной предикторной связи
      int TryCount = 0; //Количество ложных генераций предикторной связи - введено, чтобы не было зацикливания
      do // Цикл нахождения корректной связи
      {
         PrePoolID = UniformDiscreteDistribution(1, KidPoolNetwork->PoolQuantity);
         PostPoolID = UniformDiscreteDistribution(1, KidPoolNetwork->PoolQuantity);

         CurPredConnection = GetPointer2PoolPredConnection(KidPoolNetwork, PrePoolID, PostPoolID);

         OutCheck=0;
         if (CurPredConnection==NULL) // Еcли нет такой предикторной связи в сети
            if (PostPoolID > EnResolution + OutResolution) // Если постсинаптический пул не сенсорный и не выходной нейрон
               OutCheck=1;

         TryCount++;
           // или тоже самое - ( (FaultQt>=1001) || ( (tmpConGen==NULL) && ( (OutNode>(EnResolution+OutResolution)) || (InNode<=EnResolution) || (InNode>(EnResolution+OutResolution)) ) && (OutNode>EnResolution) ) )
      }while ((!OutCheck) && (TryCount<1000)); // Пока не найдем корректную предикторную связь или превысим допустимое кол-во попыток

       // Если найдена корректная связь для добавления
      if (OutCheck)
      {
         TNeuralPool* PrePool = GetPointer2Pool(KidPoolNetwork, PrePoolID);// Находим пресинаптический пул
         CurPool = GetPointer2Pool(KidPoolNetwork, PostPoolID);

         if (CurPool->PredConnectednessSet==NULL) // Если у постсинаптического пула еще нет входных предикторных связей (В текущий момент в переменной текущего пула ссылка на постсинаптический пул)
            // Создаем новую предикторную связь
					CurPool->PredConnectednessSet = CreatePoolPredConnection(++KidPoolNetwork->PredConnectionQuantity, 1, ++(*CurrentPredInnovationNumber), 0, InitialDevelopPredConProb, PrePool, CurPool, NULL);
         else
         {
            CurPredConnection = CurPool->PredConnectednessSet;
            while (CurPredConnection->next!=NULL) CurPredConnection = CurPredConnection->next; // Находим последнюю связь
            // Создаем новую связь
				CurPredConnection->next = CreatePoolPredConnection(++KidPoolNetwork->PredConnectionQuantity, 1, ++(*CurrentPredInnovationNumber), 0, InitialDevelopPredConProb, PrePool, CurPool, NULL);
         }
      }
   }
}

// Процедура мутации - удаление связи
void MutationDeletePoolConnection(TPoolNetwork* KidNetwork, double DeleteConnectionProb)
{
   if (UniformDistribution(0, 1) < DeleteConnectionProb) // Если имеет место мутация
   {
      TNeuralPool* CurPool = KidNetwork->PoolsStructure;
      TPoolConnection* CurConnection;

      int EnabledConnectionQuantity = 0; // Кол-во включенных связей в сети
      // Находим кол-во включенных связей в сети
      while (CurPool!=NULL) // Пробегаем по всем пулам
      {
         CurConnection = CurPool->ConnectednessSet;
         while (CurConnection!=NULL) // Пробегаем по все входящим связям текущего пула
         {
            if (CurConnection->Enabled)
               ++EnabledConnectionQuantity;
            CurConnection = CurConnection->next;
         }
         CurPool = CurPool->next;
      }
      if (EnabledConnectionQuantity != 0) // Если есть хотя бы одна связь
      {
         int DeletedConnectionID = UniformDiscreteDistribution(1, EnabledConnectionQuantity); // Номер удаляемой связи

         CurPool = KidNetwork->PoolsStructure;
         // Нахождение удаляемой связи
         while ((DeletedConnectionID) && (CurPool!=NULL))
         {
            CurConnection = CurPool->ConnectednessSet;
            while ((DeletedConnectionID) && (CurConnection!=NULL))
            {
               if (CurConnection->Enabled)
                  --DeletedConnectionID;
               if (DeletedConnectionID) // Если мы еще не нашли эту связь, то переходим дальше
                  CurConnection = CurConnection->next;
            }
            if (DeletedConnectionID)
               CurPool = CurPool->next;
         }
         // Находим связь до нее
         TPoolConnection* CurPrevConnection = CurPool->ConnectednessSet;
         if (CurPrevConnection->ID == CurConnection->ID) // Если это первая связт у нейрона
         {
            CurPool->ConnectednessSet = CurConnection->next;
            delete CurConnection;
         }
         else
         {
            // Находим связь перед удаляемой
            while (CurPrevConnection->next->ID != CurConnection->ID)
               CurPrevConnection = CurPrevConnection->next;
            CurPrevConnection->next = CurConnection->next;
            delete CurConnection;
         }
         // Выравниваем номера
         --(KidNetwork->ConnectionQuantity);
         CurPool = KidNetwork->PoolsStructure;
         int ConnectionCount=0;
         while (CurPool!=NULL)
         {
            CurConnection = CurPool->ConnectednessSet;
            while (CurConnection!=NULL)
            {
               CurConnection->ID = ++ConnectionCount;
               CurConnection = CurConnection->next;
            }
            CurPool=CurPool->next;
         }
      }
   }
}

// Процедура мутации - удаление пред. связи
void MutationDeletePoolPredConnection(TPoolNetwork* KidNetwork, double DeletePredConnectionProb)
{
   if (UniformDistribution(0, 1) < DeletePredConnectionProb) // Если имеет место мутация
   {
      TNeuralPool* CurPool = KidNetwork->PoolsStructure;
      TPoolPredConnection* CurPredConnection;

      int EnabledPredConnectionQuantity = 0; // Кол-во включенных связей в сети
      // Находим кол-во включенных связей в сети
      while (CurPool!=NULL) // Пробегаем по всем пулам
      {
         CurPredConnection = CurPool->PredConnectednessSet;
         while (CurPredConnection!=NULL) // Пробегаем по все входящим связям текущего пула
         {
            if (CurPredConnection->Enabled)
               ++EnabledPredConnectionQuantity;
            CurPredConnection = CurPredConnection->next;
         }
         CurPool = CurPool->next;
      }
      if (EnabledPredConnectionQuantity != 0) // Если есть хотя бы одна связь
      {
         int DeletedPredConnectionID = UniformDiscreteDistribution(1, EnabledPredConnectionQuantity); // Номер удаляемой связи

         CurPool = KidNetwork->PoolsStructure;
         // Нахождение удаляемой связи
         while ((DeletedPredConnectionID) && (CurPool!=NULL))
         {
            CurPredConnection = CurPool->PredConnectednessSet;
            while ((DeletedPredConnectionID) && (CurPredConnection!=NULL))
            {
               if (CurPredConnection->Enabled)
                  --DeletedPredConnectionID;
               if (DeletedPredConnectionID) // Если мы еще не нашли эту связь, то переходим дальше
                  CurPredConnection = CurPredConnection->next;
            }
            if (DeletedPredConnectionID)
               CurPool = CurPool->next;
         }
         // Находим связь до нее
         TPoolPredConnection* CurPrevPredConnection = CurPool->PredConnectednessSet;
         if (CurPrevPredConnection->ID == CurPredConnection->ID) // Если это первая связт у нейрона
         {
            CurPool->PredConnectednessSet = CurPredConnection->next;
            delete CurPredConnection;
         }
         else
         {
            // Находим связь перед удаляемой
            while (CurPrevPredConnection->next->ID != CurPredConnection->ID)
               CurPrevPredConnection = CurPrevPredConnection->next;
            CurPrevPredConnection->next = CurPredConnection->next;
            delete CurPredConnection;
         }
         // Выравниваем номера
         --(KidNetwork->PredConnectionQuantity);
         CurPool = KidNetwork->PoolsStructure;
         int PredConnectionCount=0;
         while (CurPool!=NULL)
         {
            CurPredConnection = CurPool->PredConnectednessSet;
            while (CurPredConnection!=NULL)
            {
               CurPredConnection->ID = ++(PredConnectionCount);
               CurPredConnection = CurPredConnection->next;
            }
            CurPool=CurPool->next;
         }
      }
   }
}

// Процедура мутации - вставка пула (для NEAT, при этом невозможен первичный системогенез и обучение)
void MutationInsertPool(TPoolNetwork* KidPoolNetwork, double InsertPoolProb, int EvolutionStep, double* CurrentInnovationNumber)
{
   if (UniformDistribution(0, 1) < InsertPoolProb) // Если имеет место мутация
   {
      TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
      TPoolConnection* CurConnection;

      int EnabledConnectionQuantity = 0; // Кол-во включенных связей в сети
      // Находим кол-во включенных связей в сети
      while (CurPool!=NULL) // Пробегаем по всем пулам
      {
         CurConnection = CurPool->ConnectednessSet;
         while (CurConnection!=NULL) // Пробегаем по все входящим связям текущего пула
         {
            if (CurConnection->Enabled)
               EnabledConnectionQuantity++;
            CurConnection = CurConnection->next;
         }
         CurPool = CurPool->next;
      }

      int DividedConnectionID = UniformDiscreteDistribution(1, EnabledConnectionQuantity); // Номер разделяемой связи

      CurPool = KidPoolNetwork->PoolsStructure;
      // Нахождение разделяемой связи
      while ((DividedConnectionID) && (CurPool!=NULL))
      {
         CurConnection = CurPool->ConnectednessSet;
         while ((DividedConnectionID) && (CurConnection!=NULL))
         {
            if (CurConnection->Enabled)
               DividedConnectionID--;
            if (DividedConnectionID) // Если мы еще не нашли эту связь, то переходим дальше
               CurConnection = CurConnection->next;
         }
         CurPool = CurPool->next;
      }

      TPoolConnection* DividedConnection = CurConnection;
      int NewPoolLayer; // Слой нового пула
      int CheckLayerShift = 0; // Признак необходимости сдвига пулов в следующие слои

       // Определение слоя нового пула
      if ( DividedConnection->PrePool->Layer < DividedConnection->PostPool->Layer ) // Если постсинаптический пул находился в более высоком слое
         if ( DividedConnection->PostPool->Layer - DividedConnection->PrePool->Layer == 1) // Если пулы находились в смежных слоях
         {
            NewPoolLayer = DividedConnection->PostPool->Layer; // Слой нового пула равен слою постсинаптического пула
            CheckLayerShift = 1;
         }
         else
            NewPoolLayer = DividedConnection->PrePool->Layer + 1; // Новый пул помещается в следующий слой после пресинаптического пула
      else
         if( DividedConnection->PrePool->Layer > DividedConnection->PostPool->Layer ) // Если пресинаптический пула находился в более высоком слое
            if ( DividedConnection->PrePool->Layer - DividedConnection->PostPool->Layer == 1) // Если пулы находились в смежных слоях
            {
               NewPoolLayer = DividedConnection->PrePool->Layer;// Слой нового пула равено слою пресинаптического пула
               CheckLayerShift=1;
            }
            else
               NewPoolLayer = DividedConnection->PrePool->Layer - 1; // Новый пул помещается в слой перед пресинаптическим пулоа
         else // Если пул находились в одном слое
         {
            NewPoolLayer = DividedConnection->PrePool->Layer;
            CheckLayerShift=2; // Признак того, что пулы находились в одном слое
         }

      // Если нужно сдвигать пулы в слудующие слои, то проводим эту операцию
      if (CheckLayerShift)
      {
         CurPool = KidPoolNetwork->PoolsStructure;
         while (CurPool!=NULL) // Пробегаем по всем пулам
         {
            if (CurPool->Layer >= NewPoolLayer) // Если слой текущего пула больше либо равен слою вставляемого пула
            {
               if (CheckLayerShift==2) // Если разбиваемая связь была между пулами одного слоя
               {
                  if ( (CurPool->Layer > NewPoolLayer) || (CurPool->ID == DividedConnection->PostPool->ID) ) // Если слой текущего пула больше слоя вставляемого или это постсинаптический пул разбиваемой связи
                     CurPool->Layer = CurPool->Layer + 1;
               }
               else  // Если разбиваемая связь между смежными слоями
                  CurPool->Layer = CurPool->Layer + 1;
            }

            CurPool = CurPool->next;
         }
      }

      // Находим последний пул
      CurPool = KidPoolNetwork->PoolsStructure;
      while (CurPool->next!=NULL)
         CurPool = CurPool->next;
      // Создаем новый пул
      TNeuralPool* NewPool; // Новый пул
      NewPool = CreateNeuralPool(++KidPoolNetwork->PoolQuantity, 1, 1/*!Необходимо вписать начальный объем пула!*/, UniformDistribution(-1,1), 0, NewPoolLayer, NULL, NULL, NULL); //!! Инициализация мат.ожидания [-1;1], дисперсии [-0.1;0.1]
      CurPool->next = NewPool;

      // Выключаем разделяемую связь
      DividedConnection->Enabled = false;
      DividedConnection->DisStep = EvolutionStep;

      //Добавляем входящую связь в созданный пул
      NewPool->ConnectednessSet = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, UniformDistribution(-1, 1), 0, 1, ++(*CurrentInnovationNumber), 0, 1.0, DividedConnection->PrePool, NewPool, NULL); //!! Инициализация мат.ожидания [-1;1], дисперсии [-0.1;0.1]

      //Добавляем входящую связь в постсинаптический пул разделяемой связи
      CurConnection = DividedConnection->PostPool->ConnectednessSet;
      while (CurConnection->next != NULL)
         CurConnection = CurConnection->next;
      CurConnection->next = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, UniformDistribution(-1, 1), 0, 1, ++(*CurrentInnovationNumber) , 0, 1.0, NewPool, DividedConnection->PostPool, NULL); //!! Инициализация мат.ожидания [-1;1], дисперсии [-0.1;0.1]
   }
}

// Множитель, который используется в процедуре мутации - дупликация пула, чтобы уменьшить вероятность дупликации с ростом сети
double F_duplicate(int PoolQuantity, int ConnectionQuantity, int PoolStandartAmount, int ConnectionStandartAmount)
{
   //double F = (double) 2*max_i(1,NeuronsQuantity-NetworkStandartSize);
   double F = max( 1.0, static_cast<double>(0.5*ConnectionQuantity/(static_cast<double>(ConnectionStandartAmount)) + 0.5*(PoolQuantity - PoolStandartAmount)) );
   return F;
}

// Процедура мутации - дупликация пула
void MutationPoolDuplicate(TPoolNetwork* KidPoolNetwork, double DuplicatePoolProb, double PoolDivisionCoefficient, int PoolStandartAmount, int ConnectionStandartAmount, double* CurrentInnovationNumber, double* CurrentPredInnovationNumber)
{
   TNeuralPool* CurPool;
   TPoolConnection* CurConnection;

   TNeuralPool* LastPool = KidPoolNetwork->PoolsStructure; // Текущий последний пул в сети
   while (LastPool->next!=NULL) // Находим последний пул в сети
      LastPool = LastPool->next;

   int OldConnectionsQuantity = KidPoolNetwork->ConnectionQuantity; //Исходное кол-во связей в сети
   int OldPoolsQuantity = KidPoolNetwork->PoolQuantity; // Исхоное кол-во пулов в сети
   int OldPredConnectionQuantity = KidPoolNetwork->PredConnectionQuantity;
   CurPool = KidPoolNetwork->PoolsStructure;
   do       // До тех пор пока не пройдены все старые пулы, проверяем их на вероятность дуплицирования
   {
      if (CurPool->Type == 1) // Если текущий пул - внутренний
      {
         // Проверяем дуплицирует ли пул (с учетом введеной поправки, уменьшающей вероятность дупликации с ростом нейросетевой структуры в эволюции)
         bool Duplicate = (UniformDistribution(0, 1) < DuplicatePoolProb/F_duplicate(OldPoolsQuantity, OldConnectionsQuantity, PoolStandartAmount, ConnectionStandartAmount));
         if (Duplicate) // Если пул дуплицирует
         {
            if (CurPool->Capacity != 1) // Если в пуле не остался только один нейрон (иначе оставляем размерность = 1)
               CurPool->Capacity = static_cast<int>(CurPool->Capacity * PoolDivisionCoefficient + 0.5); //!!!!
            LastPool->next = CreateNeuralPool(++KidPoolNetwork->PoolQuantity, CurPool->Type, CurPool->Capacity, CurPool->BiasMean, CurPool->BiasVariance, CurPool->Layer, NULL, NULL, NULL);//!!!!!!!!! // Создание нового пула

            LastPool = LastPool->next;

            CurConnection = CurPool->ConnectednessSet;
            TPoolConnection* LastConnectionInPool = NULL; // Переменная последней входящей связи в новом (в дальнейшем не в новом) пуле
            while (CurConnection!=NULL) // Копируем все входящие связи из дуплицируещего пула в новый
            {
               if (CurConnection->ID<=OldConnectionsQuantity) // Если это старая связь (то есть до процедуры мутации)
               {
                  if (LastConnectionInPool == NULL) //Если связей еще нет
                  {
                     if (CurConnection->PrePool->ID == CurPool->ID) // Если это связь на самого себя
								LastPool->ConnectednessSet = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, CurConnection->WeightMean, CurConnection->WeightVariance, CurConnection->Enabled, ++(*CurrentInnovationNumber), CurConnection->DisStep, CurConnection->DevelopSynapseProb, LastPool, LastPool, NULL);
                     else
                        LastPool->ConnectednessSet = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, CurConnection->WeightMean, CurConnection->WeightVariance, CurConnection->Enabled, ++(*CurrentInnovationNumber), CurConnection->DisStep, CurConnection->DevelopSynapseProb, CurConnection->PrePool, LastPool, NULL);
                     LastConnectionInPool = LastPool->ConnectednessSet;
                  }
                  else
                  {
                     if (CurConnection->PrePool->ID == CurPool->ID) // Если это связь на самого себя
                        LastConnectionInPool->next =  CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, CurConnection->WeightMean, CurConnection->WeightVariance, CurConnection->Enabled, ++(*CurrentInnovationNumber), CurConnection->DisStep, CurConnection->DevelopSynapseProb, LastPool, LastPool, NULL);
                     else
                        LastConnectionInPool->next =  CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, CurConnection->WeightMean, CurConnection->WeightVariance, CurConnection->Enabled, ++(*CurrentInnovationNumber), CurConnection->DisStep, CurConnection->DevelopSynapseProb, CurConnection->PrePool, LastPool, NULL);
                     LastConnectionInPool = LastConnectionInPool->next;
                  }
               }
               CurConnection = CurConnection->next;
            }

            TNeuralPool* CurInputPool = KidPoolNetwork->PoolsStructure; // Переменная текущего потенциального постсинаптического пула для связей от нового пула
            // Копируем все выходящие связи из дуплицирующего пула (для этого надо пройтись по всей сети, так как эти связи разбросаны)
            while ((CurInputPool!=NULL)&&(CurInputPool->ID <= OldPoolsQuantity)) // Проходим по всем изначальным пулам
            {
               if (CurInputPool->ID != CurPool->ID)
               {
                  CurConnection = CurInputPool->ConnectednessSet;
                  while (CurConnection!=NULL)
                  {
                     if (CurConnection->ID<=OldConnectionsQuantity) // Если это старая связь (то есть до процедуры мутации)
                        if (CurConnection->PrePool->ID == CurPool->ID) // Если у текущего потенциального пула есть связь с текущим дублицирующим пулом
                        {
                           LastConnectionInPool = CurConnection;
                           while (LastConnectionInPool->next!=NULL) LastConnectionInPool = LastConnectionInPool->next; // Находим последнюю связь
                           LastConnectionInPool->next = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, CurConnection->WeightMean/2.0, CurConnection->WeightVariance, CurConnection->Enabled, ++*CurrentInnovationNumber, CurConnection->DisStep, CurConnection->DevelopSynapseProb, LastPool, CurConnection->PostPool, NULL);
                           CurConnection->WeightMean = CurConnection->WeightMean/2.0; // Выходные связи дуплицирующего нейрона мы делим пополам между новым и дуплицирующим
                        }
                     CurConnection = CurConnection->next;
                  }
               }
               CurInputPool = CurInputPool->next;
            }

            //!! Делаем тоже самое (копируем) со всеми предикторными связями текущего пула
            TPoolPredConnection* CurPredConnection = CurPool->PredConnectednessSet;
            TPoolPredConnection* LastPredConnectionInPool = NULL; // Переменная последней входящей связи в новом (в дальнейшем не в новом) пуле
            while (CurPredConnection!=NULL) // Копируем все входящие связи из дуплицируещего пула в новый
            {
               if (CurPredConnection->ID<=OldPredConnectionQuantity) // Если это старая связь (то есть до процедуры мутации)
               {
                  if (LastPredConnectionInPool == NULL) //Если связей еще нет
                  {
                     if (CurPredConnection->PrePool->ID == CurPool->ID)
								LastPool->PredConnectednessSet = CreatePoolPredConnection(++KidPoolNetwork->PredConnectionQuantity, CurPredConnection->Enabled, ++(*CurrentPredInnovationNumber), CurPredConnection->DisStep, CurPredConnection->DevelopPredConProb, LastPool, LastPool, NULL);
                     else
                        LastPool->PredConnectednessSet = CreatePoolPredConnection(++KidPoolNetwork->PredConnectionQuantity, CurPredConnection->Enabled, ++*CurrentPredInnovationNumber, CurPredConnection->DisStep, CurPredConnection->DevelopPredConProb, CurPredConnection->PrePool, LastPool, NULL);
                     LastPredConnectionInPool = LastPool->PredConnectednessSet;
                  }
                  else
                  {
                     if (CurPredConnection->PrePool->ID == CurPool->ID)
                        LastPredConnectionInPool->next =  CreatePoolPredConnection(++KidPoolNetwork->PredConnectionQuantity, CurPredConnection->Enabled, ++*CurrentPredInnovationNumber, CurPredConnection->DisStep, CurPredConnection->DevelopPredConProb, LastPool, LastPool, NULL);
                     else
                        LastPredConnectionInPool->next =  CreatePoolPredConnection(++KidPoolNetwork->PredConnectionQuantity, CurPredConnection->Enabled, ++*CurrentPredInnovationNumber, CurPredConnection->DisStep, CurPredConnection->DevelopPredConProb, CurPredConnection->PrePool, LastPool, NULL);
                     LastPredConnectionInPool = LastPredConnectionInPool->next;
                  }
               }
               CurPredConnection = CurPredConnection->next;
            }

            CurInputPool = KidPoolNetwork->PoolsStructure; // Переменная текущего потенциального постсинаптического пула для связей от нового пула
            // Копируем все выходящие связи из дуплицирующего пула (для этого надо пройтись по всей сети, так как эти связи разбросаны)
            while ((CurInputPool!=NULL)&&(CurInputPool->ID <= OldPoolsQuantity)) // Проходим по всем изначальным пулам
            {
               if (CurInputPool->ID != CurPool->ID)
               {
                  CurPredConnection = CurInputPool->PredConnectednessSet;
                  while (CurPredConnection!=NULL)
                  {
                     if (CurPredConnection->ID<=OldPredConnectionQuantity) // Если это старая связь (то есть до процедуры мутации)
                        if (CurPredConnection->PrePool->ID == CurPool->ID) // Если у текущего потенциального пула есть связь с текущим дублицирующим пулом
                        {
                           LastPredConnectionInPool = CurPredConnection;
                           while (LastPredConnectionInPool->next!=NULL) LastPredConnectionInPool = LastPredConnectionInPool->next; // Находим последнюю связь
                           LastPredConnectionInPool->next = CreatePoolPredConnection(++KidPoolNetwork->PredConnectionQuantity, CurPredConnection->Enabled, ++*CurrentPredInnovationNumber, CurPredConnection->DisStep, CurPredConnection->DevelopPredConProb, LastPool, CurPredConnection->PostPool, NULL);
                        }
                     CurPredConnection = CurPredConnection->next;
                  }
               }
               CurInputPool = CurInputPool->next;
            }
         } // Duplicate
      }
      CurPool=CurPool->next;

   } while ( (CurPool!=NULL)&&(CurPool->ID<=OldPoolsQuantity) );
}

// Процедура мутации вероятности развития синапса по связи между пулами
void MutationDevelopSynapseProb(TPoolNetwork* KidPoolNetwork, double MutDevelopSynapseProbProb, double MutDevelopSynapseProbDisp){
	TNeuralPool* CurrentPool = KidPoolNetwork->PoolsStructure;
	while (NULL != CurrentPool){
		TPoolConnection* CurrentConnection = CurrentPool->ConnectednessSet;
		while (NULL != CurrentConnection){
			if (UniformDistribution(0, 1) < MutDevelopSynapseProbProb)
				// Важно, чтобы вероятность была между нулем и единицей
				CurrentConnection->DevelopSynapseProb = min(1.0, max(0.0, CurrentConnection->DevelopSynapseProb + UniformDistribution(-MutDevelopSynapseProbDisp, MutDevelopSynapseProbDisp)));
			CurrentConnection = CurrentConnection->next;
		}
		CurrentPool = CurrentPool->next;
	}
}

// Процедура мутации вероятности развития предикторной связи по предикторной связи между пулами
void MutationDevelopPredConProb(TPoolNetwork* KidPoolNetwork, double MutDevelopPredConProbProb, double MutDevelopPredConProbDisp){
	TNeuralPool* CurrentPool = KidPoolNetwork->PoolsStructure;
	while (NULL != CurrentPool){
		TPoolPredConnection* CurrentPredConnection = CurrentPool->PredConnectednessSet;
		while (NULL != CurrentPredConnection){
			if (UniformDistribution(0, 1) < MutDevelopPredConProbProb)
				// Важно, чтобы вероятность была между нулем и единицей
				CurrentPredConnection->DevelopPredConProb = min(1.0, max(0.0, CurrentPredConnection->DevelopPredConProb + UniformDistribution(-MutDevelopPredConProbDisp, MutDevelopPredConProbDisp)));
			CurrentPredConnection = CurrentPredConnection->next;
		}
		CurrentPool = CurrentPool->next;
	}
}

// Процедура генерации потомка от двух родителей
void ComposeOffspringFromParents(TPoolNetwork* KidPoolNetwork, TPoolNetwork* FirstParentNetwork, TPoolNetwork* SecondParentNetwork)
{

   TPoolNetwork* MoreFitness;
   TPoolNetwork* LessFitness;
   // Находим более и менее приспособленного родителя
   if (FirstParentNetwork->Reward >= SecondParentNetwork->Reward)
   {
      MoreFitness = FirstParentNetwork;
      LessFitness = SecondParentNetwork;
   }
   else
   {
      MoreFitness = SecondParentNetwork;
      LessFitness = FirstParentNetwork;
   }
   CopyPoolNetwork(KidPoolNetwork, MoreFitness);

   // Пробегаем по все генам потомка и смотрим есть ли во втором родителе такие гены
  /* TNeuron* FirstTempNeuron = KidNetwork->Neurons;
   while (FirstTempNeuron!=NULL)
   {
      TSynapse* FirstTempSynapse = FirstTempNeuron->InSynapses;
      while (FirstTempSynapse!=NULL)
      {
         TSynapse* MatchSynapse = NULL; // Совпадающий ген (если найден)
         // Ищем совпадающий ген
         TNeuron* SecondTempNeuron = LessFitness->Neurons;
         while ((SecondTempNeuron!=NULL) && (MatchSynapse == NULL))
         {
            TSynapse* SecondTempSynapse = SecondTempNeuron->InSynapses;
            while ((SecondTempSynapse!=NULL) && (MatchSynapse == NULL))
            {
               if (FirstTempSynapse->InnNumber == SecondTempSynapse->InnNumber)
                  MatchSynapse = SecondTempSynapse;
               SecondTempSynapse = SecondTempSynapse->next;
            }
            SecondTempNeuron = SecondTempNeuron->next;
         }

         if (MatchSynapse != NULL) // Если найден совпадающий ген, то скрещиваем
         {
            if ((FirstTempSynapse->Enabled && !MatchSynapse->Enabled) || (!FirstTempSynapse->Enabled && MatchSynapse->Enabled)) // Если в одном из родителей этот ген выключен, а в другом включен
            {
               FirstTempSynapse->Enabled = (rand()%1000 > 500); // Случайно определяем включен ли ген
               if (FirstTempSynapse->Enabled) // Если в итоге ген включен, то просто обнуляем поле, в котором записан номер эв. такта выключения
                  FirstTempSynapse->DisStep = 0;
               else // иначе
                  if (!FirstTempSynapse->DisStep) // Если в более приспособленном родителе этот ген был включен
                     FirstTempSynapse->DisStep = MatchSynapse->DisStep; // То переписываем номер такта из второго родителя (иначе номер такта уже и так записан)
            }

            // Усредняем вес
            FirstTempSynapse->Weight += MatchSynapse->Weight;
            FirstTempSynapse->Weight = FirstTempSynapse->Weight / 2.0;
            //if (rand()%1000 > 500) // С равной вероятностью выбираем весовой коэффициент одного из родителей
            //   FirstTempSynapse->Weight = MatchSynapse->Weight;

         }

         FirstTempSynapse = FirstTempSynapse->next;
      }

      FirstTempNeuron = FirstTempNeuron->next;
   }*/
}

// Процедура генерации одного потомка
void GenerateOffspring(TPoolNetwork* KidPoolNetwork, TPoolNetwork* FirstParentNetwork, TPoolNetwork* SecondParentNetwork, int EvolutionStep, TMutationSettings* Settings, bool EvolutionMode, double* CurrentInnovationNumber, double* CurrentPredInnovationNumber, double InitialDevelopSynapseProb, double InitialDevelopPredConProb)
{
   ComposeOffspringFromParents(KidPoolNetwork, FirstParentNetwork, SecondParentNetwork); // Создаем первиченую сеть потомка после скрещивания родителей

   MutationDeleteConPop(KidPoolNetwork, EvolutionStep, Settings->DisLimit); // Мутация - удаление устаревших генов
   MutationDeletePredConPop(KidPoolNetwork, EvolutionStep, Settings->DisLimit); // Мутация - удаление устаревших генов

   if (!EvolutionMode) // Если эволюционный алгоритм - NEAT
      MutationInsertPool(KidPoolNetwork, Settings->InsertPoolProb, EvolutionStep, CurrentInnovationNumber); // Мутация - вставка пула
   else
      MutationPoolDuplicate(KidPoolNetwork, Settings->DuplicatePoolProb, Settings->PoolDivisionCoef, Settings->PoolStandartAmount, Settings->ConnectionStandartAmount, CurrentInnovationNumber, CurrentPredInnovationNumber); // Мутация дупликация пулов

   MutationEnDisConnection(KidPoolNetwork, Settings->EnConProb, Settings->DisConProb, EvolutionStep); // Мутация - включение/выключение связей

	MutationAddConnection(KidPoolNetwork, Settings->AddConnectionProb, EvolutionMode, CurrentInnovationNumber, InitialDevelopSynapseProb);

	MutationAddPredConnection(KidPoolNetwork, Settings->AddPredConnectionProb, CurrentPredInnovationNumber, InitialDevelopPredConProb);

   MutationDeletePoolConnection(KidPoolNetwork, Settings->DeleteConnectionProb);

   MutationDeletePoolPredConnection(KidPoolNetwork, Settings->DeletePredConnectionProb);

   MutationConWeight(KidPoolNetwork, Settings->MutWeightProbability, Settings->MutWeightMeanDisp, Settings->MutWeightDispDisp); // Мутация весов связкй

	MutationDevelopSynapseProb(KidPoolNetwork, Settings->MutDevelopConProbProb, Settings->MutDevelopConProbDisp);

	MutationDevelopPredConProb(KidPoolNetwork, Settings->MutDevelopConProbProb, Settings->MutDevelopConProbDisp);
}

//Процедура получения номера агента, используемая в рулеточном алгоритме
int RouletteWheelSelection(double* PopulationFitness, int PopulationQuantity)
{
   double TotalFitness = 0; // Полная награда популяции
   for (int i=0; i<PopulationQuantity; i++) TotalFitness += PopulationFitness[i];

   double CurrentRandom = UniformDistribution(0, TotalFitness, false, false);
   double CurrentSum = 0.0;
   int CurrentNumber = 0;
   while (CurrentRandom > CurrentSum)
   {
      CurrentSum += PopulationFitness[CurrentNumber];
      CurrentNumber++;
   }
   //printf("k%.5fu%.5fp%ip",TotalFitness, CurrentRandom, CurrentNumber);
   return CurrentNumber;
}

// Штраф из награды за размер генома агента
double RewardPenalty(TPoolNetwork* PoolNetwork, int PenaltyRewardLimit)
{
   TNeuralPool* CurPool;
   TPoolConnection* CurConnection;
   int EnabledConnectionQuantity=0; // Кол-во включенных связей в сети

   //Подсчитываем кол-во включенных связей в сети
   CurPool = PoolNetwork->PoolsStructure;
   while (CurPool!=NULL)
   {
      CurConnection = CurPool->ConnectednessSet;
      while (CurConnection!=NULL)
      {
         if (CurConnection->Enabled) EnabledConnectionQuantity++;
         CurConnection = CurConnection->next;
      }
      CurPool = CurPool->next;
   }

    if (EnabledConnectionQuantity < PenaltyRewardLimit)
      return 0;
    else
      return (EnabledConnectionQuantity - PenaltyRewardLimit) * (EnabledConnectionQuantity - PenaltyRewardLimit) * 1.0F / 3.0F;
}

// Процедура генерации нового поколения и замены старого на новое
void GenerateNextPopulation(TAgentGenomePopulation* AgentGenomePopulation, TMutationSettings* MutationSettings, int EvolutionStep, bool EvolutionMode, bool RewardMode, double* CurrentInnovationNumber, double* CurrentPredInnovationNumber, double InitialDevelopSynapseProb, double InitialDevelopPredConProb)
{
   //double PopulationReward[AgentGenomePopulation->PopulationAgentQuantity]; // Массив со всеми наградами популяци
   double* PopulationReward = new double[AgentGenomePopulation->PopulationAgentQuantity]; // Массив со всеми наградами популяци
   double RewardSum = 0.0; // Сумма всех наград агентов
   for (int i=0; i<AgentGenomePopulation->PopulationAgentQuantity; i++) // Заполняем массив с наградами популяции
      if (RewardMode) // Если используется штрафная функция
      {
         PopulationReward[i] = max( 0.0, AgentGenomePopulation->AgentGenome[i]->Reward - RewardPenalty(AgentGenomePopulation->AgentGenome[i], MutationSettings->PenaltyRewardLimit) ); // Учитываем штраф за размер агента
         RewardSum += PopulationReward[i];
      }
      else
      {
         PopulationReward[i] = AgentGenomePopulation->AgentGenome[i]->Reward;
         RewardSum += PopulationReward[i];
      }


   TAgentGenomePopulation *NewGenomePopulation;// Новая популяции
   NewGenomePopulation = new TAgentGenomePopulation;
   NewGenomePopulation->PopulationAgentQuantity = AgentGenomePopulation->PopulationAgentQuantity;
   for (int i=0;i<AgentGenomePopulation->PopulationAgentQuantity;i++) // Генерируем новую популяцию
   {
      int FirstParentAgent;
      int SecondParentAgent;
      // Определяем номер родительского агента
      if (RewardSum) // Если есть хоть один агент, который набрал хоть сколько-то награды
      {
         FirstParentAgent = RouletteWheelSelection(PopulationReward, AgentGenomePopulation->PopulationAgentQuantity) - 1;
         int CheckCount = 0;
         do
         {
            SecondParentAgent =  RouletteWheelSelection(PopulationReward, AgentGenomePopulation->PopulationAgentQuantity) - 1;
            CheckCount++;
         } while ((FirstParentAgent == SecondParentAgent)&&(CheckCount<200));

         if (FirstParentAgent == SecondParentAgent) //Если не удается найти другого агента (потому что у всех остальных очень маленькие награды)
            SecondParentAgent = UniformDiscreteDistribution(0, AgentGenomePopulation->PopulationAgentQuantity - 1);
      }
      else
      {
         FirstParentAgent = UniformDiscreteDistribution(0, AgentGenomePopulation->PopulationAgentQuantity - 1);
         do
         {
            SecondParentAgent =  UniformDiscreteDistribution(0, AgentGenomePopulation->PopulationAgentQuantity - 1);

         } while (FirstParentAgent == SecondParentAgent);
      }
      // Пусть более приспособленный родитель всегда будет на первом месте
      //printf("_%i_%i_", FirstParentAgent, SecondParentAgent);
      if (AgentGenomePopulation->AgentGenome[FirstParentAgent]->Reward < AgentGenomePopulation->AgentGenome[SecondParentAgent]->Reward)
      {
         int tmp = FirstParentAgent;
         FirstParentAgent = SecondParentAgent;
         SecondParentAgent = tmp;
      }
      NewGenomePopulation->AgentGenome[i] = CreatePoolNetwork(); // Инициализируем агента
		GenerateOffspring(NewGenomePopulation->AgentGenome[i], AgentGenomePopulation->AgentGenome[FirstParentAgent], AgentGenomePopulation->AgentGenome[SecondParentAgent], EvolutionStep, MutationSettings, EvolutionMode, CurrentInnovationNumber, CurrentPredInnovationNumber, InitialDevelopSynapseProb, InitialDevelopPredConProb);
      NewGenomePopulation->AgentGenome[i]->Parents[0] = FirstParentAgent;
      NewGenomePopulation->AgentGenome[i]->Parents[1] = SecondParentAgent;
   }
   delete []PopulationReward;


   // Теперь перепишем новую популяцию в старую (но не физически, а только ссылки)
   for (int i=0; i<AgentGenomePopulation->PopulationAgentQuantity; i++)
   {
      AgentGenomePopulation->AgentGenome[i] = DeletePoolNetwork(AgentGenomePopulation->AgentGenome[i]);
      AgentGenomePopulation->AgentGenome[i] = NewGenomePopulation->AgentGenome[i];
   }

   delete NewGenomePopulation;
}
