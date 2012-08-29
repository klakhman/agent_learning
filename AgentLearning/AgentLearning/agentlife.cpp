#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "service.h"
#include "service_network.h"
#include "environment.h"
#include "learning.h"

#include "agentlife.h"

//Процедура определения общей награды, полученной агентом в течение жизни
double CalcAgentReward(int* LifeRecord, int AgentLifetime, TEnvironmentAims* EnvironmentAims, int RewardRecoveryTime)
{
   double AgentReward = 0.0; // Награда агента
   for (int CurStep=0; CurStep<AgentLifetime; CurStep++) // Проходимся по всей ленте жизни
   {
      for (int CurAim=0; CurAim<EnvironmentAims->AimQuantity; CurAim++) // Проверяем все цели
         if ( EnvironmentAims->Aim[CurAim].Complexity <= (CurStep + 1) ) // Если агент успел бы достигнуть этой цели
         {
            int ErrorCheck = 0; //Признак того, что проверяемая цель не достигнута
            int CurBit = 0; // Текущий бит цели
            do // Проходимся по ленте и проверяем достижение текущей цели
            {
               // Примечание: если агент ничего не сделал то в ленте записано 0, а значит всегда будет выход из цикла
               // Если текущий бит цели совпадает с изменяемым битом в ленте (c учетом отматывания времени назад)
               if ( EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Number == (abs(LifeRecord[CurStep - EnvironmentAims->Aim[CurAim].Complexity + 1 + CurBit ]) - 1) )
                  if (LifeRecord[CurStep - EnvironmentAims->Aim[CurAim].Complexity + CurBit + 1] < 0) // Если перевод был из единицы в ноль
                  {
                     if (EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Value == 1) ErrorCheck = 1; // то если в цели надо было перевести бит в единицу то выходим
                  }
                  else // Иначе
                  {
                     if (EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Value == 0) ErrorCheck = 1; // если надо было перевести бит в ноль, то выходим
                  }
               else // Если номер бита не совпадает то выходим
                  ErrorCheck = 1;
               CurBit++;
            } while ( (!ErrorCheck) && (CurBit < EnvironmentAims->Aim[CurAim].Complexity) ); // Пока нет выхода или пока проверка цели не закончена
            if (!ErrorCheck) // Если не было преждевременного выхода, значит цель достигнута
            {
               //AimStatistic[j]+=1;
               //AimTemp[j]+=1;
               // Прибавляем текущую награду за цель (минимум из стандартной награды и востанавливающейся)
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

//Процедура действия агента в один такт времени
void AgentStep(TNeuralNetwork* AgentNetwork, int* EnvironmentVector, TEnvironmentAims* EnvironmentAims, int* LifeRecord, int AgentLifeStep, bool NetworkMode)
{
   int OutputResolution = EnvironmentAims->OutputResolution; // Размерность выходного пространства
   int EnvironmentResolution = EnvironmentAims->EnvironmentResolution; // Размерность среды
	//!!! Изменения связанные с представлением входных и выходных пулов
	// Определение реальных размеров выходного и входного пространства (для случая когда входные и выходные нейроны также представлены пулами)
	int* distr_input_neurons = new int[EnvironmentResolution]; // Количество входных нейронов в рамках каждого входного пула
	memset(distr_input_neurons, 0, sizeof(int)*EnvironmentResolution);
	int network_EnvironmentResolution = 0; // Реальное кол-во входных нейронов в сети
	TNeuron* CurrentNeuron = AgentNetwork->NetworkStructure;
	while (CurrentNeuron->Type == 0){ // Подсчитываем распределение входных нейронов
		++distr_input_neurons[CurrentNeuron->ParentPoolID - 1];
		++network_EnvironmentResolution;
		CurrentNeuron = CurrentNeuron->next;
	}
	int* distr_output_neurons = new int[OutputResolution]; // Количество выходных нейронов в рамках каждого выходного пула
	memset(distr_output_neurons, 0, sizeof(int)*OutputResolution);
	int network_OutputResolution = 0; // Реальное кол-во выходных нейронов в сети
	while (CurrentNeuron->Type == 2){ // Подсчитываем распределение выходных нейронов
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
   if (!NetworkMode) // Если режим одновременный
      NetworkOutput_standart(AgentNetwork, _EnvironmentVector, _CurrentNetworkOut); // Определяем текущий выходной вектор нейросети агента
   else // Если режим синхронный
      NetworkOutput_synch(AgentNetwork, _EnvironmentVector, _CurrentNetworkOut);
	
	double* CurrentNetworkOut = new double[OutputResolution]; 
	memset(CurrentNetworkOut, 0, sizeof(double) * OutputResolution);
	//!!!
	// Считаем среднее выхода каждого выходного пула
	CurrentBit = 0;
	for (int i=0; i < OutputResolution; ++i)
		for (int j=0; j < distr_output_neurons[i]; ++j)
			CurrentNetworkOut[i] += _CurrentNetworkOut[CurrentBit++] / distr_output_neurons[i];	

	delete []distr_input_neurons;
	delete []distr_output_neurons;
	delete []_EnvironmentVector;
	delete []_CurrentNetworkOut;
	//!!!

   double MaxOutput_1=-1; // Значение первого по величине выхода
   double MaxOutput_2=-1; // Значение второго по величине выхода
   int MaxNeuronNumber_1=0; // Номер нейрона с первым по величине выходом
   int MaxNeuronNumber_2=0; // Номер нейрона со вторым по величине выходом

   // Определение двух максимальных выходов нейронов
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
   // Если нейроны не по порядку, то меняем местами номера (нам неважно кто из пары больше)
   if (MaxNeuronNumber_1 > MaxNeuronNumber_2)
   {
      MaxNeuronNumber_1 = MaxNeuronNumber_1 + MaxNeuronNumber_2;
      MaxNeuronNumber_2 = MaxNeuronNumber_1 - MaxNeuronNumber_2;
      MaxNeuronNumber_1 = MaxNeuronNumber_1 - MaxNeuronNumber_2;
   }

   //Процедура расшифровки выходов сети
   int CurNeuronNumber_1 = 1; // Номер первого текущего нейрона в проверяемой паре
   int CurNeuronNumber_2; // Номер второго текущего нейрона в паре
   int OutCheck = 0; // Признак того, что нужная пара найдена
   int PairCount = 0; // Кол-во проверенных пар
   int ActionNumber = 0;// Итоговый номер совершаемого дейтсвия
   do
   {
      CurNeuronNumber_2 = CurNeuronNumber_1 + 1;
      do
      {
         // Проверяем нужная ли это пара
         if ( (MaxNeuronNumber_1 == CurNeuronNumber_1) && (MaxNeuronNumber_2 == CurNeuronNumber_2) )
         {
            ActionNumber = PairCount % (2 * EnvironmentResolution); // Определяем номер действия
            OutCheck = 1;
         }
         PairCount++;
         CurNeuronNumber_2++;
      }while ( (CurNeuronNumber_2 <= OutputResolution) && (!OutCheck) );
      CurNeuronNumber_1++;
   }while ( (CurNeuronNumber_1 < OutputResolution) && (!OutCheck) );

   // Изменение вектора среды и запись действия в летопись жизни агента
   if (ActionNumber % 2) // Если вычисленный номер бита нечетное число - значит это перевод из 1 в 0
   {
      if (EnvironmentVector[(ActionNumber - 1) / 2]) // Если бит вектора был единицей
      {
         EnvironmentVector[(ActionNumber - 1) / 2] = 0;
         LifeRecord[AgentLifeStep - 1] = - ((ActionNumber - 1) / 2 + 1);
      }
      else
         LifeRecord[AgentLifeStep - 1] = 0; // Иначе агент ничего не делает
   }
   else // Иначе из 0 в 1
   {
      if (!EnvironmentVector[ActionNumber / 2]) // Если бит вектора был нулем
      {
         EnvironmentVector[ActionNumber / 2] = 1;
         LifeRecord[AgentLifeStep - 1] = ActionNumber / 2 + 1;
      }
      else
         LifeRecord[AgentLifeStep - 1] = 0;
   }

   delete []CurrentNetworkOut;
}

//Процедура моделирования жизни агента
void AgentLife(TNeuralNetwork* AgentNetwork, TEnvironmentAims* EnvironmentAims, int RewardRecoveryTime, double EnVarProb, int AgentLifetime, bool NetworkMode /* = false*/, bool LearningMode /* = false*/, TLearningSettings* LearningSettings /* = NULL*/)
{
   ResetEnviroment(EnvironmentAims); // Перезапускаем среду
   ResetNeuralNetwork(AgentNetwork);

   //int EnvironmentVector[EnvironmentAims->EnvironmentResolution]; // Вектор состояния среды
   int* EnvironmentVector = new int[EnvironmentAims->EnvironmentResolution];
   for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++) // Генерируем случайное первоначальное состояние
      EnvironmentVector[i] =  rand() % 2;

   //int LifeRecord[AgentLifetime]; // Запись жизни агента
   int* LifeRecord = new int[AgentLifetime];
   // Запускаем процесс жизни
   for (int AgentLifeStep=1; AgentLifeStep<=AgentLifetime; AgentLifeStep++)
   {
      AgentStep(AgentNetwork, EnvironmentVector, EnvironmentAims, LifeRecord, AgentLifeStep, NetworkMode);
      if (LearningMode) AgentLearning(AgentNetwork, LearningSettings);
      // Случайное изменение вектора состояния (для квазистационарной среды)
      for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++)
         if (rand()%10001 < EnVarProb*10001) // Проверяем надо ли изменит бит
         {
            if (EnvironmentVector[i] == 1) // Если бит был единицей
               EnvironmentVector[i] = 0; // Изменяем на ноль
            else
               EnvironmentVector[i] = 1; // Иначе изменяем на единицу
         }
   }
   delete []EnvironmentVector;
   // Подсчитываем набранную агентом награду
   AgentNetwork->Reward = CalcAgentReward(LifeRecord, AgentLifetime, EnvironmentAims, RewardRecoveryTime);
   delete []LifeRecord;
}

// Проверка достижения целей и отображение информации в реальном времени
double CheckRewardInRealTime(int* LifeRecord, int AgentLifeStep, TEnvironmentAims* EnvironmentAims, int RewardRecoveryTime)
{
   AgentLifeStep--; // Уменьшаем на единицу текущий шаг (чтобы начало жизни было с нуля)

   double AgentReward = 0.0; // Награда агента
   for (int CurAim=0; CurAim<EnvironmentAims->AimQuantity; CurAim++) // Проверяем все цели
      if ( EnvironmentAims->Aim[CurAim].Complexity <= (AgentLifeStep + 1) ) // Если агент успел бы достигнуть этой цели
      {
         int ErrorCheck = 0; //Признак того, что проверяемая цель не достигнута
         int CurBit = 0; // Текущий бит цели
         do // Проходимся по ленте и проверяем достижение текущей цели
         {
            // Примечание: если агент ничего не сделал то в ленте записано 0, а значит всегда будет выход из цикла
            // Если текущий бит цели совпадает с изменяемым битом в ленте (c учетом отматывания времени назад)
            if ( EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Number == (abs(LifeRecord[AgentLifeStep - EnvironmentAims->Aim[CurAim].Complexity + 1 + CurBit ]) - 1) )
               if (LifeRecord[AgentLifeStep - EnvironmentAims->Aim[CurAim].Complexity + CurBit + 1] < 0) // Если перевод был из единицы в ноль
               {
                  if (EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Value == 1) ErrorCheck = 1; // то если в цели надо было перевести бит в единицу то выходим
               }
               else // Иначе
               {
                  if (EnvironmentAims->Aim[CurAim].AimStructure[CurBit].Value == 0) ErrorCheck = 1; // если надо было перевести бит в ноль, то выходим
               }
            else // Если номер бита не совпадает то выходим
               ErrorCheck = 1;
            CurBit++;
         } while ( (!ErrorCheck) && (CurBit < EnvironmentAims->Aim[CurAim].Complexity) ); // Пока нет выхода или пока проверка цели не закончена
         if (!ErrorCheck) // Если не было преждевременного выхода, значит цель достигнута
         {
            // Прибавляем текущую награду за цель (минимум из стандартной награды и востанавливающейся)
            double LocalAgentReward = min((double) EnvironmentAims->Aim[CurAim].Reward, (AgentLifeStep + 1 - EnvironmentAims->Aim[CurAim].VisitTime) * EnvironmentAims->Aim[CurAim].Reward / ((double) RewardRecoveryTime) );
            EnvironmentAims->Aim[CurAim].VisitTime=AgentLifeStep + 1;
            AgentReward += LocalAgentReward;

            printf("Agent have reached aim #%i - local reward: %.3f\n", CurAim, LocalAgentReward);
         }
      }
    return AgentReward;
}

// Отображение жизни агента в текстовом режиме
void AgentLifeView(TNeuralNetwork* AgentNetwork, TEnvironmentAims*  EnvironmentAims, int RewardRecoveryTime, double EnVarProb, int AgentLifetime, bool NetworkMode)
{
   ResetEnviroment(EnvironmentAims); // Перезапускаем среду
   ResetNeuralNetwork(AgentNetwork);

   AgentNetwork->Reward = 0;
   //int EnvironmentVector[EnvironmentAims->EnvironmentResolution]; // Вектор состояния среды
   int* EnvironmentVector = new int[EnvironmentAims->EnvironmentResolution];
   for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++) // Генерируем случайное первоначальное состояние
      EnvironmentVector[i] =  rand() % 2;

   //int LifeRecord[AgentLifetime]; // Запись жизни агента
   int* LifeRecord = new int[AgentLifetime];
   // Запускаем процесс жизни
   for (int AgentLifeStep=1; AgentLifeStep<=AgentLifetime; AgentLifeStep++)
   {
      // Печатаем состояние до текущего шага
      printf("%i:\t%.3f\t", AgentLifeStep, AgentNetwork->Reward);
      for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++)
         printf("%i ", EnvironmentVector[i]);
      printf("\t->\t");

      AgentStep(AgentNetwork, EnvironmentVector, EnvironmentAims, LifeRecord, AgentLifeStep, NetworkMode);

      // Печатаем состояние после текущего шага
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

      AgentNetwork->Reward += CheckRewardInRealTime(LifeRecord, AgentLifeStep, EnvironmentAims, RewardRecoveryTime); // Считаем награду агента на текущий шаг, а также отображаем информацию о достигнутых целях

      char c = getchar();

      // Случайное изменение вектора состояния (для квазистационарной среды)
      for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++)
         if (rand()%10001 < EnVarProb*10001) // Проверяем надо ли изменит бит
         {
            if (EnvironmentVector[i] == 1) // Если бит был единицей
               EnvironmentVector[i] = 0; // Изменяем на ноль
            else
               EnvironmentVector[i] = 1; // Иначе изменяем на единицу
         }
   }
   delete []LifeRecord;
   delete []EnvironmentVector;
}
