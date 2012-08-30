#include <cmath>
#include <cstring>

#include "types.h"
#include "service.h"
#include "service_network.h"

#include "primary_systemogenesis.h"

// Построение ПОЛНОЙ (без отбора) первичной нейронной сети из генотипа (структуры пулов)
void BuildPrimaryNetwork(TNeuralNetwork* PrimaryNetwork, TPoolNetwork* PoolNetwork)
{
   int NeuronsQuantity = 0; // Количество нейронов в первичной сети
   TNeuralPool* CurrentPool= PoolNetwork->PoolsStructure; // Ссылка на текущий пул
   TNeuron* CurrentNeuron = PrimaryNetwork->NetworkStructure; // Ссылка на текущий нейрон
   int MaxCapacity = 0; // Максимальная размерность пула
   // Находим максимальную размерность пула
   for (int PoolNumber = 0; PoolNumber < PoolNetwork->PoolQuantity; PoolNumber++)
   {
      if (CurrentPool->Capacity > MaxCapacity)
         MaxCapacity = CurrentPool->Capacity;
      CurrentPool = CurrentPool->next;
   }
   // Создаем двумерный массив ссылок на все нейроны первичной сети, разбитых на принадлежность к пулу (чтобы потом их не искать, когда добавляем связи)
	//printf("%i ", MaxCapacity);
   //TNeuron* PoolAndNetworkStructure[PoolNetwork->PoolQuantity][MaxCapacity];
   TNeuron*** PoolAndNetworkStructure = new TNeuron**[PoolNetwork->PoolQuantity];
   for (int i=0; i<PoolNetwork->PoolQuantity; ++i)
      PoolAndNetworkStructure[i] = new TNeuron*[MaxCapacity];

   CurrentPool = PoolNetwork->PoolsStructure;
   // Проходим по всем пулам и создаем нейроны
   for (int PoolNumber = 0; PoolNumber < PoolNetwork->PoolQuantity; PoolNumber++)
   {
      // Создаем все нейроны пула
      for (int NeuronNumber = 0; NeuronNumber < CurrentPool->Capacity; NeuronNumber++)
      {
         if (CurrentNeuron == NULL) // Если в сети нет еще ни одного нейрона
         {
            PrimaryNetwork->NetworkStructure = CreateNeuron(++NeuronsQuantity, CurrentPool->Type, NormalDistribution(CurrentPool->BiasMean, CurrentPool->BiasVariance), CurrentPool->Layer, true, CurrentPool, CurrentPool->ID, NULL, NULL, NULL);
            CurrentNeuron = PrimaryNetwork->NetworkStructure;
         }
         else
         {
            CurrentNeuron->next = CreateNeuron(++NeuronsQuantity, CurrentPool->Type, NormalDistribution(CurrentPool->BiasMean, CurrentPool->BiasVariance), CurrentPool->Layer, true, CurrentPool, CurrentPool->ID, NULL, NULL, NULL);
            CurrentNeuron = CurrentNeuron->next;
         }
         PoolAndNetworkStructure[PoolNumber][NeuronNumber] = CurrentNeuron;
      }

      CurrentPool = CurrentPool->next;
   }
   CurrentPool = PoolNetwork->PoolsStructure;
   CurrentNeuron = PrimaryNetwork->NetworkStructure;
   int SynapsesQuantity = 0; // Количество синапсов сети
   int PredConnectionQuantity = 0; // Количество предикторных связей
   // Проходимсся по всем пулам и создаем синапсы
   for (int PoolNumber = 0; PoolNumber < PoolNetwork->PoolQuantity; ++PoolNumber)
   {
      for (int NeuronNumber = 0; NeuronNumber < CurrentPool->Capacity; ++NeuronNumber)
      {
         // Создаем синапсы
         TPoolConnection* CurrentConnection = CurrentPool->ConnectednessSet;
         TSynapse* CurrentSynapse = CurrentNeuron->InSynapses;
         // Пока мы не прошли все входные связи текущего пула
         while (CurrentConnection != NULL)
         {
            /*// Определяем количество входных связей для текущего нейрона от нейронов пресинаптического пула
            int CurrentSynapsesQuantity = rand() % 0.2*CurrentConnection->PrePool->Capacity;
            int PreNeuronNumber[CurrentSynapsesQuantity]; // Номера пресинаптических нейронов из текущего пресинаптического пула
            GenerateRandomNumbers(1, CurrentConnection->PrePool->Capacity, CurrentSynapsesQuantity, PreNeuronNumber);
            BubbleSort(PreNeuronNumber, CurrentSynapsesQuantity);

            for (int i=0; i<CurrentSynapsesQuantity; i++) // Создаем все текущие синапсы
            {
               if (CurrentSynapse = NULL) // Если нет еще ни одного синапса у текущего нейрона
               {
                  CurrentNeuron->InSynapses = CreateSynapse(++SynapsesQuantity, NormalDistribution(CurrentConnection->WeightMean, CurrentConnection->WeightVariance), CurrentConnection->Enabled, PoolAndNetworkStructure[CurrentConnection->PrePool->ID-1][PreNeuronNumber[i]-1], CurrentNeuron, NULL);
                  CurrentSynapse = CurrentNeuron->InSynapses;
               }
               else
               {
                  CurrentSynapse->next = CreateSynapse(++SynapsesQuantity, NormalDistribution(CurrentConnection->WeightMean, CurrentConnection->WeightVariance), CurrentConnection->Enabled, PoolAndNetworkStructure[CurrentConnection->PrePool->ID-1][PreNeuronNumber[i]-1], CurrentNeuron, NULL);
                  CurrentSynapse = CurrentSynapse->next;
               }
            }*/
            // Проходимся по всем потенциальным пресинаптическим нейронам и определяем развивается ли связь
            if (CurrentConnection->Enabled)
                  for (int CurrentPreNeuronNumber = 0; CurrentPreNeuronNumber < CurrentConnection->PrePool->Capacity; ++CurrentPreNeuronNumber )
                  {
							if (UniformDistribution(0, 1) < CurrentConnection->DevelopSynapseProb)
                     {
                        if (CurrentSynapse == NULL) // Если нет еще ни одного синапса у текущего нейрона
                        {
                           CurrentNeuron->InSynapses = CreateSynapse(++SynapsesQuantity, NormalDistribution(CurrentConnection->WeightMean, CurrentConnection->WeightVariance), CurrentConnection->Enabled, PoolAndNetworkStructure[CurrentConnection->PrePool->ID-1][CurrentPreNeuronNumber], CurrentNeuron, NULL);
                           CurrentSynapse = CurrentNeuron->InSynapses;
                        }
                        else
                        {
                           CurrentSynapse->next = CreateSynapse(++SynapsesQuantity, NormalDistribution(CurrentConnection->WeightMean, CurrentConnection->WeightVariance), CurrentConnection->Enabled, PoolAndNetworkStructure[CurrentConnection->PrePool->ID-1][CurrentPreNeuronNumber], CurrentNeuron, NULL);
                           CurrentSynapse = CurrentSynapse->next;
                        }
                     }
                  }

            CurrentConnection = CurrentConnection->next;
         }

         //Создаем предикторные связи
         TPoolPredConnection* CurrentPredConnection = CurrentPool->PredConnectednessSet;
         TPredConnection* CurrentPredSynapse = CurrentNeuron->PredConnections;
         // Пока мы не прошли все входные предикторные связи текущего пула
         while (CurrentPredConnection != NULL)
         {
            /*// Определяем количество входных предикторных связей для текущего нейрона от нейронов пресинаптического пула
            int CurrentPredSynapsesQuantity = rand() % 0.2*CurrentPredConnection->PrePool->Capacity;
            int PreNeuronNumber[CurrentPredSynapsesQuantity]; // Номера пресинаптических нейронов из текущего пресинаптического пула
            GenerateRandomNumbers(1, CurrentPredConnection->PrePool->Capacity, CurrentPredSynapsesQuantity, PreNeuronNumber);
            BubbleSort(PreNeuronNumber, CurrentPredSynapsesQuantity);

            for (int i=0; i<CurrentPredSynapsesQuantity; i++) // Создаем все текущие предикторные связи
            {
               if (CurrentPredSynapse = NULL) // Если нет еще ни одной предикторной связи у текущего нейрона
               {
                  CurrentNeuron->PredConnections = CreatePredConnection(++PredConnectionQuantity, CurrentPredConnection->Enabled, PoolAndNetworkStructure[CurrentPredConnection->PrePool->ID-1][PreNeuronNumber[i]-1], CurrentNeuron, NULL);
                  CurrentPredSynapse = CurrentNeuron->PredConnections;
               }
               else
               {
                  CurrentPredSynapse->next = CreatePredConnection(++PredConnectionQuantity, CurrentPredConnection->Enabled, PoolAndNetworkStructure[CurrentPredConnection->PrePool->ID-1][PreNeuronNumber[i]-1], CurrentNeuron, NULL);
                  CurrentPredSynapse = CurrentPredSynapse->next;
               }
            }*/
            // Проходимся по всем потенциальным пресинаптическим нейронам для текущей предикторной связи и определяем развивается ли связь
            if (CurrentPredConnection->Enabled)
               for (int CurrentPreNeuronNumber = 0; CurrentPreNeuronNumber < CurrentPredConnection->PrePool->Capacity; ++CurrentPreNeuronNumber)
               {
						if (UniformDistribution(0, 1) < CurrentPredConnection->DevelopPredConProb)
                  {
                     if (CurrentPredSynapse == NULL) // Если нет еще ни одного синапса у текущего нейрона
                     {
                        CurrentNeuron->PredConnections = CreatePredConnection(++PredConnectionQuantity, CurrentPredConnection->Enabled, PoolAndNetworkStructure[CurrentPredConnection->PrePool->ID-1][CurrentPreNeuronNumber], CurrentNeuron, NULL);
                        CurrentPredSynapse = CurrentNeuron->PredConnections;
                     }
                     else
                     {
                        CurrentPredSynapse->next = CreatePredConnection(++PredConnectionQuantity, CurrentPredConnection->Enabled, PoolAndNetworkStructure[CurrentPredConnection->PrePool->ID-1][CurrentPreNeuronNumber], CurrentNeuron, NULL);
                        CurrentPredSynapse = CurrentPredSynapse->next;
                     }
                  }
               }

            CurrentPredConnection = CurrentPredConnection->next;
         }

         CurrentNeuron = CurrentNeuron->next;
      }
      CurrentPool = CurrentPool->next;
   }

   for (int i=0; i<PoolNetwork->PoolQuantity; ++i)
      delete [](PoolAndNetworkStructure[i]);
   delete []PoolAndNetworkStructure;

   // Заполняем данные про построенную сеть
   PrimaryNetwork->NeuronQuantity = NeuronsQuantity;
   PrimaryNetwork->SynapseQuantity = SynapsesQuantity;
   PrimaryNetwork->PredConnectionQuantity = PredConnectionQuantity;
}

// Функция отбора активирующихся нейронов
void NeuronsSelection(TNeuralNetwork* PrimaryNetwork, double* const _SummaryPotential, int ActivationNeuronsQuantity)
{
	double* const SummaryPotential = new double[PrimaryNetwork->NeuronQuantity];
	memcpy(SummaryPotential, _SummaryPotential, sizeof(double) * PrimaryNetwork->NeuronQuantity);

	int InputOuputNeuronsQuantity = 0;
   TNeuron* CurrentNeuron = PrimaryNetwork->NetworkStructure;
   // Считаем кол-во входных и выходных нейронов
   while (CurrentNeuron != NULL)
   {
      if (CurrentNeuron->Type != 1)
         ++InputOuputNeuronsQuantity;
      CurrentNeuron = CurrentNeuron->next;
   }

   // Вариант с использованием рулеточного алгоритма
   // Работаем только с той частью списка, в которой записаны интер-нейроны
   // Сначала нормируем все значения - так чтобы у нейронов с отрицательным потенциалом была возможность отобраться
   double MinPotential = 0;
   // Ищем минимальный отрицательный суммарный потенциал
   for (int CurrentNeuron = InputOuputNeuronsQuantity; CurrentNeuron<PrimaryNetwork->NeuronQuantity; CurrentNeuron++)
      if (MinPotential > SummaryPotential[CurrentNeuron])
         MinPotential = SummaryPotential[CurrentNeuron];
   // Инвертируем оставшиеся потенциалы
   for (int CurrentNeuron = InputOuputNeuronsQuantity; CurrentNeuron<PrimaryNetwork->NeuronQuantity; CurrentNeuron++)
      SummaryPotential[CurrentNeuron] -= MinPotential - 1; // Делаем так, чтобы ни у одного нейрона не было нулевого суммарного потенциала
   
   double TotalPotential = 0;
   double CurrentNeuronsQuantity = 0;
   for (int CurrentNeuron = InputOuputNeuronsQuantity; CurrentNeuron < PrimaryNetwork->NeuronQuantity; CurrentNeuron++)
      TotalPotential += SummaryPotential[CurrentNeuron];
   const double CheckEmpty = -5000.0F; // Признак выбранного элемента

   // Пока есть хотя бы один нейрон с ненулевым суммарным потенциалом и не набрано достаточно кол-во нейронов - прогоняем рулеточный алгоритм
   while (CurrentNeuronsQuantity<ActivationNeuronsQuantity)
   {
      double RandomPotential = UniformDistribution(0, TotalPotential, false, false);//(rand()%1000/1000.0) * TotalPotential;
      double CurrentPotential = 0;
      int CurrentNeuronNum = InputOuputNeuronsQuantity;
		//char tmp_str[10000];
		//strcpy(tmp_str, "");
      while (RandomPotential > CurrentPotential)
		{
         CurrentPotential += max(SummaryPotential[CurrentNeuronNum++], 0.0);
			//sprintf(tmp_str, "%s%i: %.4f : %.4f |\t", tmp_str, CurrentNeuronNum-1, max(SummaryPotential[CurrentNeuronNum-1], 0.0), CurrentPotential);
		}
		if (CurrentNeuronNum > PrimaryNetwork->NeuronQuantity){
			printf("\n!! Error !! - %i (%i) - %.6f , %.6f , %.6f\n", CurrentNeuronNum, PrimaryNetwork->NeuronQuantity, RandomPotential, TotalPotential, CurrentPotential);
			//char c= getchar();
		}
		//printf("%i(%i) ", CurrentNeuronNum, PrimaryNetwork->NeuronQuantity);
      // Уменьшаем суммарный потенциал на величину выбранного нейрона
      TotalPotential -= SummaryPotential[CurrentNeuronNum-1];

      SummaryPotential[CurrentNeuronNum - 1] = CheckEmpty; //!!!
      CurrentNeuronsQuantity++;
   }
   /*Детерминированный вариант
   for (int i=0; i<ActivationNeuronsQuantity; i++)
   {
      double CurrentMax=-999;
      int CurrentMaxNumber;
      for (int j=InputOuputNeuronsQuantity; j<PrimaryNetwork->NeuronsQuantity; j++)
         if (CurrentMax < SummaryPotential[j])
         {
            CurrentMax = SummaryPotential[j];
            CurrentMaxNumber = j;
         }
      SummaryPotential[j] = -1000;
   }*/

   //Изменяем информацию о нейронах (разделяем их на активные и молчащие)
   CurrentNeuron = PrimaryNetwork->NetworkStructure;
   while (CurrentNeuron!=NULL)
   {
      if (CurrentNeuron->Type == 1)
         if (SummaryPotential[CurrentNeuron->ID-1] == CheckEmpty)
            CurrentNeuron->Active = 1;
         else CurrentNeuron->Active = 0;
      else
         CurrentNeuron->Active=1;

      CurrentNeuron = CurrentNeuron->next;
   }

	delete []SummaryPotential;
}

// Функция отбора наиболее активных синапсов
void SynapsesSelection(TNeuralNetwork* PrimaryNetwork, double* const SummaryPotential, double SynapsesActivityTreshold)
{
	TNeuron* CurrentNeuron = PrimaryNetwork->NetworkStructure;
   TSynapse* CurrentSynapse;
   // Переписываем все данные о сумарных потенциалах в сами синапсы, чтобы потом эффективно их отсортировать, а значения весов переписываем в отдельный массив
   //double WeightDump[PrimaryNetwork->SynapseQuantity]; // Сохраненные веса всех синапсов
   double* WeightDump = new double[PrimaryNetwork->SynapseQuantity];
   //TSynapse* SelectionSynapseDump[PrimaryNetwork->SynapseQuantity]; // Массив связей для сортировки
   TSynapse** SelectionSynapseDump = new TSynapse*[PrimaryNetwork->SynapseQuantity]; // Массив связей для сортировки

   int SelectionSynapseQuantity = 0;
   while (CurrentNeuron != NULL)
   {
      CurrentSynapse = CurrentNeuron->InSynapses;
      while (CurrentSynapse != NULL)
      {
         WeightDump[CurrentSynapse->ID - 1] = CurrentSynapse->Weight;
         CurrentSynapse->Weight = SummaryPotential[CurrentSynapse->ID - 1];
         // Если пресинаптический нейрон не входной и постсинаптический нейрон не выходной (так как мы почему-то не отбираем такие связи, а оставляем их)
         if ((CurrentSynapse->PreNeuron->Type != 0) && (CurrentSynapse->PostNeuron->Type != 2))
            SelectionSynapseDump[++SelectionSynapseQuantity - 1] = CurrentSynapse;
         CurrentSynapse = CurrentSynapse->next;
      }
      CurrentNeuron = CurrentNeuron->next;
   }
   // Теперь отсортируем массив синапсов, чтобы потом найти квантиль распределения (или просто отобрать нужное кол-во синапсов)
   for (int i=0; i<SelectionSynapseQuantity; ++i)
      for (int j=0; j<SelectionSynapseQuantity-i-1; ++j)
         if (SelectionSynapseDump[j]->Weight > SelectionSynapseDump[j+1]->Weight)
         {
            TSynapse* tmpSynapse = SelectionSynapseDump[j+1];
            SelectionSynapseDump[j+1] = SelectionSynapseDump[j];
            SelectionSynapseDump[j] = tmpSynapse;
         }

   double PercentileValue = 0; // Значение нужной персентили
   if (SelectionSynapseQuantity) // Если вообще есть синапсы, которые надо отбирать
      PercentileValue = SelectionSynapseDump[static_cast<int>((100 - SynapsesActivityTreshold)/100.0*SelectionSynapseQuantity)]->Weight;

   CurrentNeuron = PrimaryNetwork->NetworkStructure;
   while (CurrentNeuron!=NULL)
   {
      // Отбор активных связей (только между активными нейронами)
      if ((CurrentNeuron->Active) && (CurrentNeuron->Type != 2)) // Если текущий нейрон активен и он не выходной
      {
         CurrentSynapse = CurrentNeuron->InSynapses;
         if (CurrentSynapse != NULL) // Если у нейрона вообще есть входные синапсы
         {
            while (CurrentSynapse->next != NULL) // Проходимся по всем кроме первой
            {
               if ((CurrentSynapse->next->PreNeuron->Active) && (CurrentSynapse->next->PreNeuron->Type)) // Если пресинаптический нейрон активен и он не входной
                  if (CurrentSynapse->Weight < PercentileValue) // Если активность ниже порога, то удаляем связь
                  {
                     TSynapse* TmpSynapse = CurrentSynapse->next;
                     CurrentSynapse->next = TmpSynapse->next;
                     delete TmpSynapse;
                     PrimaryNetwork->SynapseQuantity--;
                  }
                  else
                     CurrentSynapse = CurrentSynapse->next;
               else
                  CurrentSynapse = CurrentSynapse->next;
            }
            if ((CurrentNeuron->InSynapses->PreNeuron->Active) && (CurrentNeuron->InSynapses->PreNeuron->Type)) // Если пресинаптический нейрон первой связи активен и он не входной
               if (CurrentSynapse->Weight < PercentileValue) // Если активность ниже порога, то удаляем связь
               {
                  TSynapse* TmpSynapse = CurrentNeuron->InSynapses;
                  CurrentNeuron->InSynapses = TmpSynapse->next;
                  delete TmpSynapse;
                  PrimaryNetwork->SynapseQuantity--;
               }
         }
      }
      CurrentNeuron = CurrentNeuron->next;
   }

   // Возвращаем все веса на нужное место
   CurrentNeuron = PrimaryNetwork->NetworkStructure;
   while (CurrentNeuron != NULL)
   {
      CurrentSynapse = CurrentNeuron->InSynapses;
      while (CurrentSynapse != NULL)
      {
         CurrentSynapse->Weight = WeightDump[CurrentSynapse->ID - 1];
         CurrentSynapse=CurrentSynapse->next;
      }
      CurrentNeuron = CurrentNeuron->next;
   }
   // Выравниваем идентификаторы синапсов
   fix_synapses_id(PrimaryNetwork);
   delete []WeightDump;
   delete []SelectionSynapseDump;
}

// Функция отбора предикторных связей
void PredConnectionSelection(TNeuralNetwork* PrimaryNetwork, double* PredictorSignificance, double SignificanceTreshold)
{
   TNeuron* CurrentNeuron = PrimaryNetwork->NetworkStructure;
   TPredConnection* CurrentPredConnection;
   while (CurrentNeuron!=NULL)
   {
      if (CurrentNeuron->Active) // Если текущий нейрон активен
      {
         CurrentPredConnection = CurrentNeuron->PredConnections;
         if (CurrentPredConnection != NULL) // Если у нейрона вообще есть предикторные связи
         {
            while (CurrentPredConnection->next != NULL) // Проходимся по всем кроме первой
            {
               if ((CurrentPredConnection->next->PreNeuron->Active)) // Если пресинаптический нейрон активен
                  if (PredictorSignificance[CurrentPredConnection->next->ID - 1] <= SignificanceTreshold) // Если статистическая значимость предсказания ниже требуемой, то удаляем связь
                  {
                     TPredConnection* TmpPredConnection = CurrentPredConnection->next;
                     CurrentPredConnection->next = TmpPredConnection->next;
                     delete TmpPredConnection;
                     PrimaryNetwork->PredConnectionQuantity--;
                  }
                  else
                     CurrentPredConnection = CurrentPredConnection->next;
               else
                  CurrentPredConnection = CurrentPredConnection->next;
            }
            if ((CurrentNeuron->PredConnections->PreNeuron->Active)) // Если пресинаптический нейрон первой связи активен
               if (PredictorSignificance[CurrentNeuron->PredConnections->ID - 1] <= SignificanceTreshold) // Если статистическая значимость предсказания ниже требуемой, то удаляем связь
               {
                  TPredConnection* TmpPredConnection = CurrentNeuron->PredConnections;
                  CurrentNeuron->PredConnections = TmpPredConnection->next;
                  delete TmpPredConnection;
                  PrimaryNetwork->PredConnectionQuantity--;
               }
         }
      }
      CurrentNeuron = CurrentNeuron->next;
   }

   // Выравниваем идентификаторы связей
   fix_pred_connections_id(PrimaryNetwork);
}

// Процедура проведения первичного системогенеза агента
void AgentPrimarySystemogenesis(TNeuralNetwork* PrimaryNetwork, TPoolNetwork* PoolNetwork, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings)
{
   EraseNeuralNetwork(PrimaryNetwork);

   BuildPrimaryNetwork(PrimaryNetwork, PoolNetwork);

   // Массив суммарных потенциалов на нейроне за все время первичного системогенеза
   /*double NeuronsPotential[PrimaryNetwork->NeuronQuantity];
   for (int i=0; i<PrimaryNetwork->NeuronQuantity; ++i)
      NeuronsPotential[i] = 0;*/
   double* const NeuronsPotential = new double[PrimaryNetwork->NeuronQuantity];
   memset(NeuronsPotential, 0, sizeof(double) * PrimaryNetwork->NeuronQuantity);
   // Массив суммарных потенциалов прошедших через синапсы за все время первичного системогенеза
   /*double SynapsesPotential[PrimaryNetwork->SynapseQuantity];
   for (int i=0; i<PrimaryNetwork->SynapseQuantity; ++i)
      SynapsesPotential[i] = 0;*/
   double* const SynapsesPotential = new double[PrimaryNetwork->SynapseQuantity];
   memset(SynapsesPotential, 0, sizeof(double) * PrimaryNetwork->SynapseQuantity);
   // Массив степени статистической значимости предсказания предикторной связи
   /*double PredictorSignificance[PrimaryNetwork->PredConnectionQuantity];
   for (int i=0; i<PrimaryNetwork->PredConnectionQuantity; ++i)
      PredictorSignificance[i] = 0;*/
   double* const PredictorSignificance = new double[PrimaryNetwork->PredConnectionQuantity];
   memset(PredictorSignificance, 0, sizeof(double) * PrimaryNetwork->PredConnectionQuantity);

   // Проводим прогон первичного системогенеза и подсчет суммарных характеристик
   ResetNeuralNetwork(PrimaryNetwork);
   for (int TimeStep=0; TimeStep<PrimarySystemogenesisSettings->PrimarySystemogenesisTime; TimeStep++)
   {
      NetworkOutput_primarysys(PrimaryNetwork, PrimarySystemogenesisSettings->SpontaneousActivityProb);
      TNeuron* CurrentNeuron = PrimaryNetwork->NetworkStructure;
      // Проходимся по всем нейронам и обновляем суммарные характеристики всех сущностей
      while (CurrentNeuron != NULL)
      {
         NeuronsPotential[CurrentNeuron->ID - 1] += CurrentNeuron->Potential;

         TSynapse* CurrentSynapse = CurrentNeuron->InSynapses;
         while (CurrentSynapse != NULL)
         {
            // Если связь прямая
            if (CurrentSynapse->PostNeuron->Layer > CurrentSynapse->PreNeuron->Layer)
               SynapsesPotential[CurrentSynapse->ID - 1] += abs(CurrentSynapse->Weight * CurrentSynapse->PreNeuron->CurrentOut);
            // Обратная связь
            else
               SynapsesPotential[CurrentSynapse->ID - 1] += abs(CurrentSynapse->Weight * CurrentSynapse->PreNeuron->PreviousOut);
            CurrentSynapse = CurrentSynapse->next;
         }

         TPredConnection* CurrentPredConnection = CurrentNeuron->PredConnections;
         while (CurrentPredConnection != NULL)
         {
            // Если пресинаптический нейрон правильно предсказал активацию / молчание постсинаптического
            if (((CurrentNeuron->CurrentOut > 0.5) && (CurrentPredConnection->PreNeuron->PreviousOut > 0.5)) || ((CurrentNeuron->CurrentOut <= 0.5) && (CurrentPredConnection->PreNeuron->PreviousOut <= 0.5)))
               PredictorSignificance[CurrentPredConnection->ID - 1] += 1;
            CurrentPredConnection = CurrentPredConnection->next;
         }

         CurrentNeuron = CurrentNeuron->next;
      }
   }
   // Нормировка показателей статистической значимости предикторных связей
   for (int i=0; i<PrimaryNetwork->PredConnectionQuantity; i++)
      PredictorSignificance[i] /= PrimarySystemogenesisSettings->PrimarySystemogenesisTime; //! -1;

   // Определение кол-ва активирующихся нейронов
   int InputOuputNeuronsQuantity = 0;
   TNeuron* CurrentNeuron = PrimaryNetwork->NetworkStructure;
   // Считаем кол-во входных и выходных нейронов
   while (CurrentNeuron!=NULL)
   {
      if (CurrentNeuron->Type != 1)
         ++InputOuputNeuronsQuantity;
      CurrentNeuron = CurrentNeuron->next;
   }

   int ActivationNeuronsQuantity = static_cast<int>(PrimarySystemogenesisSettings->ActiveNeuronsPercent/100.0*(PrimaryNetwork->NeuronQuantity-InputOuputNeuronsQuantity) + 0.5);
   //!!!
   /*printf("Activation neurons : %i\n", ActivationNeuronsQuantity);
   NeuralNetwork2Dot(PrimaryNetwork, "C:/EvolutionImages/TestNeuralNetworkBeforeSys.dot");
   FILE* hTestOut = fopen("C:/EvolutionImages/SecSys.txt", "w");
   double _NeuronsPotential[PrimaryNetwork->NeuronQuantity];
   for (int i=0; i<PrimaryNetwork->NeuronQuantity; ++i)
      _NeuronsPotential[i] = NeuronsPotential[i];
   //BubbleSortdouble(_NeuronsPotential, PrimaryNetwork->NeuronQuantity);
   for (int i=0; i<PrimaryNetwork->NeuronQuantity; ++i)
      fprintf(hTestOut, "%i:\t%.4f\n", i, _NeuronsPotential[i]);
   fprintf(hTestOut, "\n\n");
   double _SynapsesPotential[PrimaryNetwork->SynapseQuantity];
   for (int i=0; i<PrimaryNetwork->SynapseQuantity; ++i)
      _SynapsesPotential[i] = SynapsesPotential[i];
   BubbleSortdouble(_SynapsesPotential, PrimaryNetwork->SynapseQuantity);
   for (int i=0; i<PrimaryNetwork->SynapseQuantity; ++i)
      fprintf(hTestOut, "%i:\t%.4f\n", i, _SynapsesPotential[i]);
   fprintf(hTestOut, "\n\n");
   double _PredictorSignificance[PrimaryNetwork->PredConnectionQuantity];
   for (int i=0; i<PrimaryNetwork->PredConnectionQuantity; ++i)
      _PredictorSignificance[i] = PredictorSignificance[i];
   //BubbleSortdouble(_PredictorSignificance, PrimaryNetwork->PredConnectionQuantity);
   for (int i=0; i<PrimaryNetwork->PredConnectionQuantity; ++i)
      fprintf(hTestOut, "%i:\t%.4f\n", i, _PredictorSignificance[i]);
   fclose(hTestOut);*/
   //!!!
   NeuronsSelection(PrimaryNetwork, NeuronsPotential, ActivationNeuronsQuantity);
	//printf("%.2f ", _NeuronsPotential[2]);
   SynapsesSelection(PrimaryNetwork, SynapsesPotential, PrimarySystemogenesisSettings->SynapsesActivityTreshold);
   PredConnectionSelection(PrimaryNetwork, PredictorSignificance, PrimarySystemogenesisSettings->SignificanceTreshold);
   //printf("%i ", NeuronsPotential[1]);
   delete []NeuronsPotential;
	delete []SynapsesPotential;
   delete []PredictorSignificance;
}

// Процедура линейного системогенеза, которая преобразует сеть из пулов в сеть из нейронов, как будто это одно и тоже (эволюция нейронных сетей, а не пулов)
void AgentLinearPrimarySystemogenesis(TNeuralNetwork* PrimaryNetwork, TPoolNetwork* PoolNetwork)
{
   EraseNeuralNetwork(PrimaryNetwork);

   TNeuralPool* CurrentPool = PoolNetwork->PoolsStructure;
   TNeuron* CurrentNeuron = PrimaryNetwork->NetworkStructure;
   int NeuronsQuantity = 0;
   // Проходим по всем пулам и создаем нейроны
   while (CurrentPool != NULL)
   {
      if (CurrentNeuron == NULL)
      {
         PrimaryNetwork->NetworkStructure = CreateNeuron(++NeuronsQuantity, CurrentPool->Type, CurrentPool->BiasMean, CurrentPool->Layer, true, CurrentPool, CurrentPool->ID, NULL, NULL, NULL);
         CurrentNeuron = PrimaryNetwork->NetworkStructure;
      }
      else
      {
         CurrentNeuron->next = CreateNeuron(++NeuronsQuantity, CurrentPool->Type, CurrentPool->BiasMean, CurrentPool->Layer, true, CurrentPool,  CurrentPool->ID, NULL, NULL, NULL);
         CurrentNeuron = CurrentNeuron->next;
      }
      CurrentPool = CurrentPool->next;
   }
   CurrentPool = PoolNetwork->PoolsStructure;
   CurrentNeuron = PrimaryNetwork->NetworkStructure;
   int SynapsesQuantity = 0; // Количество синапсов сети
   int PredConnectionQuantity = 0; // Количество предикторных связей

   while (CurrentPool != NULL)
   {
      // Создаем синапсы
      TPoolConnection* CurrentConnection = CurrentPool->ConnectednessSet;
      TSynapse* CurrentSynapse = CurrentNeuron->InSynapses;
      // Пока мы не прошли все входные связи текущего пула
      while (CurrentConnection != NULL)
      {
         if (CurrentConnection->Enabled)
         {
            if (CurrentSynapse == NULL) // Если нет еще ни одного синапса у текущего нейрона
            {
               CurrentNeuron->InSynapses = CreateSynapse(++SynapsesQuantity, CurrentConnection->WeightMean, CurrentConnection->Enabled, GetPointer2Neuron(PrimaryNetwork, CurrentConnection->PrePool->ID), CurrentNeuron, NULL);
               CurrentSynapse = CurrentNeuron->InSynapses;
            }
            else
            {
               CurrentSynapse->next = CreateSynapse(++SynapsesQuantity, CurrentConnection->WeightMean, CurrentConnection->Enabled, GetPointer2Neuron(PrimaryNetwork, CurrentConnection->PrePool->ID), CurrentNeuron, NULL);
               CurrentSynapse = CurrentSynapse->next;
            }
         }
         CurrentConnection = CurrentConnection->next;
      }
      // Создаем все предикторные связи
      TPoolPredConnection* CurrentPoolPredConnection = CurrentPool->PredConnectednessSet;
      TPredConnection* CurrentPredConnection= CurrentNeuron->PredConnections;
      // Пока мы не прошли все входные связи текущего пула
      while (CurrentPoolPredConnection != NULL)
      {
         if (CurrentPoolPredConnection->Enabled)
         {
            if (CurrentPredConnection == NULL) // Если нет еще ни одной связи у текущего нейрона
            {
               CurrentNeuron->PredConnections = CreatePredConnection(++PredConnectionQuantity, CurrentPoolPredConnection->Enabled, GetPointer2Neuron(PrimaryNetwork, CurrentPoolPredConnection->PrePool->ID), CurrentNeuron, NULL);
               CurrentPredConnection = CurrentNeuron->PredConnections;
            }
            else
            {
               CurrentPredConnection->next = CreatePredConnection(++PredConnectionQuantity, CurrentPoolPredConnection->Enabled, GetPointer2Neuron(PrimaryNetwork, CurrentPoolPredConnection->PrePool->ID), CurrentNeuron, NULL);
               CurrentPredConnection = CurrentPredConnection->next;
            }
         }

         CurrentPoolPredConnection = CurrentPoolPredConnection->next;
      }
      CurrentPool = CurrentPool->next;
      CurrentNeuron = CurrentNeuron->next;
   }
   // Заполняем данные про построенную сеть
   PrimaryNetwork->NeuronQuantity = NeuronsQuantity;
   PrimaryNetwork->SynapseQuantity = SynapsesQuantity;
   PrimaryNetwork->PredConnectionQuantity = PredConnectionQuantity;
}
