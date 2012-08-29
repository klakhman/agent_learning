#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "types.h"
#include "service.h"
#include "service_network.h"
#include "environment.h"
#include "agentlife.h"
#include "parallel.h"
#include "primary_systemogenesis.h"

#include "analysis.h"

extern const int RunAgentNumber;

// Процедура прогона популяции
void PopulationRun(double* AverageReward, double* MaxReward, TAgentPopulation* AgentPopulation, TEnvironmentAims* EnvironmentAims, TTimeSettings* TimeSettings, TLearningSettings* LearningSettings, double EnVariableProb, bool NetworkMode)
{
   // Прогоняем всех агентов популяции
   //double AgentsRewardMean[AgentPopulation->PopulationAgentQuantity]; // Среднее после прогона агента
   //double AgentsRewardVariance[AgentPopulation->PopulationAgentQuantity]; // Дисперсия прогона агента

   double* AgentsRewardMean = new double[AgentPopulation->PopulationAgentQuantity];
   double* AgentsRewardVariance = new double[AgentPopulation->PopulationAgentQuantity];

   for (int CurrentAgent=0; CurrentAgent < AgentPopulation->PopulationAgentQuantity; CurrentAgent++)
   {
      double CurrentAgentReward[RunAgentNumber]; // Массив, в который записываются результаты прогона одного агента
      for (int RunNumber=0; RunNumber<RunAgentNumber; RunNumber++)
      {
         AgentLife(AgentPopulation->Agent[CurrentAgent], EnvironmentAims, TimeSettings->RewardRecoveryTime, EnVariableProb, TimeSettings->AgentLifetime, NetworkMode, false, LearningSettings);
         CurrentAgentReward[RunNumber] = AgentPopulation->Agent[CurrentAgent]->Reward;
      }

      AgentsRewardMean[CurrentAgent] = 0;
      AgentsRewardVariance[CurrentAgent] = 0;

      // Считаем среднее
      for (int i=0; i<RunAgentNumber; i++) AgentsRewardMean[CurrentAgent] += CurrentAgentReward[i] / ((double) RunAgentNumber);

      // Нахождение среднеквадратичного отклонения
      for (int i=0; i<RunAgentNumber; i++)
         AgentsRewardVariance[CurrentAgent] += (AgentsRewardMean[CurrentAgent] - CurrentAgentReward[i])*(AgentsRewardMean[CurrentAgent] - CurrentAgentReward[i]);
      AgentsRewardVariance[CurrentAgent]= sqrt(AgentsRewardVariance[CurrentAgent]/((double) RunAgentNumber - 1));
   }

   // Считаем среднее и максимальное по всей популяции
   double _MaxReward = 0.0; // Временная переменная максимальной награды
   double _AverageReward = 0.0; // Временная переменная средней награды
   for (int CurrentAgent=0; CurrentAgent<AgentPopulation->PopulationAgentQuantity; CurrentAgent++)
   {
      if (AgentsRewardMean[CurrentAgent] > _MaxReward) _MaxReward = AgentsRewardMean[CurrentAgent];
      _AverageReward += AgentsRewardMean[CurrentAgent];
   }
   _AverageReward = _AverageReward / ((double) AgentPopulation->PopulationAgentQuantity);

   *MaxReward = _MaxReward;
   *AverageReward = _AverageReward;

   delete []AgentsRewardMean;
   delete []AgentsRewardVariance;
   /*
   // Ниже закомментировано построение гистограмм распределения награды и среднеквадратичного отклонения по популяции
   for (int i=0;i<10;i++)
   {
      GistMean[i]=0;
      GistVariance[i]=0;
   }
   double MaxMean=0;
   double MaxVariance=0;
   for (int i=0;i<AgentPopulation;i++)
   {
      if (AgentRewardMean[i]>MaxMean) MaxMean=AgentRewardMean[i];
      if (AgentRewardVariance[i]>MaxVariance) MaxVariance=AgentRewardVariance[i];
   }

   for (int i=0;i<AgentPopulation;i++)
   {
      int p=0;
      do
      {
         p++;
      } while ((p*trunc((MaxMean+10)/10))<AgentRewardMean[i]);
      GistMean[p-1] = GistMean[p-1] + 1;

      p=0;
      do
      {
         p++;
      } while ((p*trunc((MaxVariance+10)/10))<AgentRewardVariance[i]);
      GistVariance[p-1] = GistVariance[p-1] + 1;
   }*/
}

// Процедра анализа показателей лучшей популяции агентов
void BestPopulationAnalysis(double* AverageReward, double* MaxReward, TRunSettings* RunSettings, TDirectorySettings* DirectorySettings, TTimeSettings* TimeSettings, TModeSettings* ModeSettings, TLearningSettings* LearningSettings, double EnVariableProb, int NoEnVariableCheck)
{
   // Инициализация генератора случайных чисел и тестовые запуски
   srand(RunSettings->RandomizeRank);
   int tmp_rand = rand()%1000;
   tmp_rand = rand()%1000;
   tmp_rand = rand()%1000;

   char EnvironmentFilename[255];
   GetEnvironmentFilename(EnvironmentFilename, DirectorySettings->EnvironmentDirectory, RunSettings->EnvironmentNumber);

   // Загружаем среду
   TEnvironmentAims EnvironmentAims;
   InitEnvironment(&EnvironmentAims, EnvironmentFilename);

   // Загружаем популяцию
   char BestPopulationFilename[255];
   GetBestPopulationFilename(BestPopulationFilename, RunSettings, ModeSettings, DirectorySettings->ResultDirectory, NoEnVariableCheck);
   FILE* hBestPopulationFile = fopen(BestPopulationFilename, "r");
   TAgentGenomePopulation AgentGenomePopulation;
   AgentGenomePopulation.PopulationAgentQuantity = TimeSettings->AgentPopulationQuantity;
   TAgentPopulation AgentPopulation;
   AgentPopulation.PopulationAgentQuantity = TimeSettings->AgentPopulationQuantity;
   for (int CurrentAgent=0; CurrentAgent<AgentGenomePopulation.PopulationAgentQuantity; CurrentAgent++)
   {
      AgentGenomePopulation.AgentGenome[CurrentAgent] = CreatePoolNetwork();
      FillNextPoolNetwork(AgentGenomePopulation.AgentGenome[CurrentAgent], hBestPopulationFile, EnvironmentAims.EnvironmentResolution, EnvironmentAims.OutputResolution, ModeSettings->NetworkMode);
      AgentPopulation.Agent[CurrentAgent] = CreateNeuralNetwork();
      AgentLinearPrimarySystemogenesis(AgentPopulation.Agent[CurrentAgent], AgentGenomePopulation.AgentGenome[CurrentAgent]); //!!!
   }
   fclose(hBestPopulationFile);

   // Запускаем прогон популяции
   PopulationRun(AverageReward, MaxReward, &AgentPopulation, &EnvironmentAims, TimeSettings, LearningSettings, EnVariableProb, ModeSettings->NetworkMode);

   for (int CurrentAgent=0; CurrentAgent<AgentPopulation.PopulationAgentQuantity; CurrentAgent++)
   {
      AgentPopulation.Agent[CurrentAgent] = DeleteNeuralNetwork(AgentPopulation.Agent[CurrentAgent]);
      AgentGenomePopulation.AgentGenome[CurrentAgent] = DeletePoolNetwork(AgentGenomePopulation.AgentGenome[CurrentAgent]);
   }

   //printf("Environment: %i\tTry: %i\n",RunSettings->EnvironmentNumber, RunSettings->TryNumber);
}

// Пропуск заголовка файла результатов
void SkipResultFileHead(FILE* hResultFile, int RewardMode)
{
   char tmp_str[255];

   for (int CurrentNote = 0; CurrentNote<13; CurrentNote++) // Пропуск списка параметров моделирования
      fscanf(hResultFile, "%s", tmp_str);

   for (int CurrentNote = 0; CurrentNote<3; CurrentNote++) // Пропуск номера такта и наград
      fscanf(hResultFile, "%s", tmp_str);

   if (RewardMode)
      for (int CurrentNote = 0; CurrentNote<2; CurrentNote++) // Пропуск чистых наград
         fscanf(hResultFile, "%s", tmp_str);

   for (int CurrentNote = 0; CurrentNote<2; CurrentNote++) // Пропуск записей о нейронах и синапсах
         fscanf(hResultFile, "%s", tmp_str);

}

// Процедура композиции общего отчета результатов эволюции по средам
void MakeFullResultFile(TGeneralRunSettings* GeneralRunSettings, TModeSettings* ModeSettings, TDirectorySettings* DirectorySettings, int EvolutionTime, double EnvironmentVariableProb)
{
   for (int EnvironmentNumber=GeneralRunSettings->FirstEnvironmentNumber; EnvironmentNumber<=GeneralRunSettings->LastEnvironmentNumber; EnvironmentNumber++)
   {
      // Массив файлов с результатами для текущей среды
      //FILE* ResultFileArray[GeneralRunSettings->LastTryNumber - GeneralRunSettings->FirstTryNumber + 1];
      FILE** ResultFileArray = new FILE*[GeneralRunSettings->LastTryNumber - GeneralRunSettings->FirstTryNumber + 1];
      bool EnvironmentStability = (EnvironmentVariableProb == 0);

      for (int TryNumber=GeneralRunSettings->FirstTryNumber; TryNumber<=GeneralRunSettings->LastTryNumber; TryNumber++)
      {
         // Заполнение структуры параметров текущего запуска
         TRunSettings RunSettings;
         RunSettings.TryNumber = TryNumber;
         RunSettings.EnvironmentNumber = EnvironmentNumber;
         RunSettings.VariableNumber = GeneralRunSettings->VariableNumber;

         char ResultFilename[255];

         // Получение полного пути к файлу результатов
         GetResultsFilename(ResultFilename, &RunSettings, ModeSettings, DirectorySettings->ResultDirectory, EnvironmentStability);

         // Открытие файла и пропуск головы
         ResultFileArray[TryNumber - GeneralRunSettings->FirstTryNumber] = fopen(ResultFilename, "r");
         SkipResultFileHead(ResultFileArray[TryNumber - GeneralRunSettings->FirstTryNumber], ModeSettings->RewardMode);
      }

      // Получение полного пути к файлу общего отчета по средам и его открытие
      TRunSettings RunSettings;
      RunSettings.EnvironmentNumber = EnvironmentNumber;
      RunSettings.VariableNumber = GeneralRunSettings->VariableNumber;

      char ResultFullFilename[255];
      GetFullResultFilename(ResultFullFilename, &RunSettings, GeneralRunSettings->FirstTryNumber, GeneralRunSettings->LastTryNumber, ModeSettings, DirectorySettings->ResultDirectory, EnvironmentStability);
      FILE* hResultFullFile = fopen(ResultFullFilename, "w");

      char tmp_str[255];
      // Перепись результатов
      for (int EvolutionStep=0; EvolutionStep<EvolutionTime; EvolutionStep++)
      {
         fprintf(hResultFullFile,"%i\t", EvolutionStep); // Запись номера такта

         for (int CurrentFile=0; CurrentFile < GeneralRunSettings->LastTryNumber - GeneralRunSettings->FirstTryNumber + 1; CurrentFile++)
         {
            fscanf(ResultFileArray[CurrentFile],"%s",tmp_str); // Считываем номер такта
            if (ModeSettings->RewardMode) // Если режим со штрафом, то считываем чистые награды
            {
               fscanf(ResultFileArray[CurrentFile],"%s",tmp_str);
               fscanf(ResultFileArray[CurrentFile],"%s",tmp_str);
            }

            // Считываем среднюю награду и записываем в общий файл
            fscanf(ResultFileArray[CurrentFile],"%s", tmp_str);
            fprintf(hResultFullFile,"%s\t", tmp_str);

            // Считываем максимальную награду и данные он нейронах и синапсах
            fscanf(ResultFileArray[CurrentFile],"%s",tmp_str);
            fscanf(ResultFileArray[CurrentFile],"%s",tmp_str);
            fscanf(ResultFileArray[CurrentFile],"%s",tmp_str);
         }
         fprintf(hResultFullFile,"\n");
      }

      // Закрываем файлы
      for (int CurrentFile=0; CurrentFile < GeneralRunSettings->LastTryNumber - GeneralRunSettings->FirstTryNumber + 1; CurrentFile++)
         fclose(ResultFileArray[CurrentFile]);
      fclose(hResultFullFile);

      delete []ResultFileArray;
   }
}

// Проверка целостности данных перед анализом
void CheckIntegrityOfData(TGeneralRunSettings* GeneralRunSettings, TModeSettings* ModeSettings, TDirectorySettings* DirectorySettings, bool NoEnVariableCheck, int AgentPopulationQuantity)
{
   // Структура, в которую будут загружаться все агента перед всеми запусками
   TPoolNetwork* CurrentAgentGenome = CreatePoolNetwork();

   char LogFilename[255];
   sprintf(LogFilename, "%s/AnalysisIntegrityLog_En%i-%i_Var%i_Noen%i.txt", DirectorySettings->WorkDirectory, GeneralRunSettings->FirstEnvironmentNumber, GeneralRunSettings->LastEnvironmentNumber, GeneralRunSettings->VariableNumber, NoEnVariableCheck);
   FILE* hLogFile = fopen(LogFilename, "w");

   for (int CurrentVarNumber=GeneralRunSettings->VariableNumber; CurrentVarNumber<=GeneralRunSettings->LastVariableNumber; CurrentVarNumber++)
      for (int CurrentEnNumber=GeneralRunSettings->FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings->LastEnvironmentNumber; CurrentEnNumber++)
         for (int CurrentTryNumber=GeneralRunSettings->FirstTryNumber; CurrentTryNumber<=GeneralRunSettings->LastTryNumber; CurrentTryNumber++)
         {
            TRunSettings RunSettings;
            RunSettings.EnvironmentNumber = CurrentEnNumber;
            RunSettings.TryNumber = CurrentTryNumber;
            RunSettings.VariableNumber = CurrentVarNumber;

            char EnvironmentFilename[255];
            GetEnvironmentFilename(EnvironmentFilename, DirectorySettings->EnvironmentDirectory, RunSettings.EnvironmentNumber);

            // Загружаем среду
            TEnvironmentAims EnvironmentAims;
            InitEnvironment(&EnvironmentAims, EnvironmentFilename);

            char BestPopulationFilename[255];
            GetBestPopulationFilename(BestPopulationFilename, &RunSettings, ModeSettings, DirectorySettings->ResultDirectory, NoEnVariableCheck);
            FILE* hBestPopulationFile = fopen(BestPopulationFilename, "r");

            for (int CurrentAgentGenomeNumber=0; CurrentAgentGenomeNumber<AgentPopulationQuantity; ++CurrentAgentGenomeNumber)
               FillNextPoolNetwork(CurrentAgentGenome, hBestPopulationFile, EnvironmentAims.EnvironmentResolution, EnvironmentAims.OutputResolution, ModeSettings->NetworkMode);
            fprintf(hLogFile, "Environment: %i Try: %i - OK\n", CurrentEnNumber, CurrentTryNumber);

            fclose(hBestPopulationFile);
         }

   CurrentAgentGenome = DeletePoolNetwork(CurrentAgentGenome);

   fclose(hLogFile);
}

double StudentCriterion(const char CriterionFilename[], double SignificanceLevel, int FreedomDegrees)
{
   return 1.98F;//2.01;
}

// Проверка статистической значимости различия пар значений двух последовательностей в зависимости от параметра
void CheckExternalPairsStatSignificance(const char FirstAnalysisFilename[], const char SecondAnalysisFilename[], const char ResultAnalysisFilename[], const char CriterionFilename[], double SignificanceLevel, int PairQuantity, int MemberQuantity, int TryQuantity)
{
   FILE* hFirstAnalysis = fopen(FirstAnalysisFilename, "r");
   FILE* hSecondAnalysis = fopen(SecondAnalysisFilename, "r");
   FILE* hResultAnalysis = fopen(ResultAnalysisFilename, "w");

   for (int PairNumber=0; PairNumber<PairQuantity; PairNumber++)
   {
      char tmp_str[255];
      //double FirstSequence[MemberQuantity*TryQuantity];
      //double SecondSequence[MemberQuantity*TryQuantity];
      double* FirstSequence = new double[MemberQuantity*TryQuantity];
      double* SecondSequence = new double[MemberQuantity*TryQuantity];
      double FirstMean=0;
      double SecondMean=0;

      for (int MemberNumber=0; MemberNumber<MemberQuantity; MemberNumber++)
         for (int TryNumber=0; TryNumber<TryQuantity; TryNumber++)
         {
            // Считываем номер среды, кол-во целей, заполненность среды, сложность среды
            fscanf(hFirstAnalysis,"%s",tmp_str); fscanf(hFirstAnalysis,"%s",tmp_str); fscanf(hFirstAnalysis,"%s",tmp_str); fscanf(hFirstAnalysis,"%s",tmp_str);
            // Считываем нужное нам среднее значение
            fscanf(hFirstAnalysis,"%s",tmp_str);
            FirstSequence[MemberNumber*TryQuantity + TryNumber] = atof(tmp_str);
            FirstMean += FirstSequence[MemberNumber*TryQuantity + TryNumber];
            // Считываем ненужное максимальное значение
            fscanf(hFirstAnalysis,"%s",tmp_str);

             // Считываем номер среды, кол-во целей, заполненность среды, сложность среды
            fscanf(hSecondAnalysis,"%s",tmp_str); fscanf(hSecondAnalysis,"%s",tmp_str); fscanf(hSecondAnalysis,"%s",tmp_str); fscanf(hSecondAnalysis,"%s",tmp_str);
            // Считываем нужное нам среднее значение
            fscanf(hSecondAnalysis,"%s",tmp_str);
            SecondSequence[MemberNumber*TryQuantity + TryNumber] = atof(tmp_str);
            SecondMean += SecondSequence[MemberNumber*TryQuantity + TryNumber];
            // Считываем ненужное максимальное значение
            fscanf(hSecondAnalysis,"%s",tmp_str);
         }

      FirstMean = FirstMean / (MemberQuantity*TryQuantity);
      SecondMean = SecondMean / (MemberQuantity*TryQuantity);

      double FirstVariance=0;
      double SecondVariance=0;
      for (int i=0; i<MemberQuantity*TryQuantity; i++)
      {
         FirstVariance += (FirstSequence[i] - FirstMean)*(FirstSequence[i] - FirstMean) / (MemberQuantity*TryQuantity - 1);
         SecondVariance += (SecondSequence[i] - SecondMean)*(SecondSequence[i] - SecondMean) / (MemberQuantity*TryQuantity - 1);
      }

      double ComplexVariance = FirstVariance / (MemberQuantity*TryQuantity) + SecondVariance / (MemberQuantity*TryQuantity);
      double TestStatistics = (FirstMean - SecondMean) / sqrt(ComplexVariance);

      double Criterion = StudentCriterion(CriterionFilename, SignificanceLevel/2, MemberQuantity*TryQuantity - 1);

      fprintf(hResultAnalysis,"%.2f\t%.2f\t", FirstMean, SecondMean);
      if (abs(TestStatistics) > Criterion)
         fprintf(hResultAnalysis, "*\n");
      else
         fprintf(hResultAnalysis, "\n");

      delete []FirstSequence;
      delete []SecondSequence;
   }

   fclose(hFirstAnalysis);
   fclose(hSecondAnalysis);
   fclose(hResultAnalysis);
}

// Проверка статистической значимости динамики значений последовательности в зависимости от параметра
void CheckInternalStatSignificance(const char AnalysisFilename[], const char ResultAnalysisFilename[], const char CriterionFilename[], double SignificanceLevel, int ParameterQuantity, int MemberQuantity, int TryQuantity)
{
   FILE* hAnalysis = fopen(AnalysisFilename, "r");
   FILE* hResultAnalysis = fopen(ResultAnalysisFilename, "w");

   //double MeanSequence[ParameterQuantity];
   //double VarianceSequence[ParameterQuantity];
   double* MeanSequence = new double[ParameterQuantity];
   double* VarianceSequence = new double[ParameterQuantity];

   for (int ParameterNumber=0; ParameterNumber<ParameterQuantity; ParameterNumber++)
   {
      char tmp_str[255];
      //double Sequence[MemberQuantity*TryQuantity];
      double* Sequence = new double[MemberQuantity*TryQuantity];
      double Mean=0;

      for (int MemberNumber=0; MemberNumber<MemberQuantity; MemberNumber++)
         for (int TryNumber=0; TryNumber<TryQuantity; TryNumber++)
         {
            // Считываем номер среды, кол-во целей, заполненность среды, сложность среды
            fscanf(hAnalysis,"%s",tmp_str); fscanf(hAnalysis,"%s",tmp_str); fscanf(hAnalysis,"%s",tmp_str); fscanf(hAnalysis,"%s",tmp_str);
            // Считываем нужное нам среднее значение
            fscanf(hAnalysis,"%s",tmp_str);
            Sequence[MemberNumber*TryQuantity + TryNumber] = atof(tmp_str);
            Mean += Sequence[MemberNumber*TryQuantity + TryNumber];
            // Считываем ненужное максимальное значение
            fscanf(hAnalysis,"%s",tmp_str);
         }

      Mean = Mean / (MemberQuantity*TryQuantity);

      double Variance=0;
      for (int i=0; i<MemberQuantity*TryQuantity; i++)
         Variance += (Sequence[i] - Mean)*(Sequence[i] - Mean) / (MemberQuantity*TryQuantity - 1);

      MeanSequence[ParameterNumber] = Mean;
      VarianceSequence[ParameterNumber] = Variance;
      fprintf(hResultAnalysis, "%.2f\t%.2f\n", Mean, sqrt(Variance));
      delete []Sequence;
   }
   fclose(hAnalysis);

   for (int i=0; i<=ParameterQuantity; i++)
      fprintf(hResultAnalysis, "%i\t", i);
   fprintf(hResultAnalysis, "\n");


   for (int ParameterNumber = 1; ParameterNumber<=ParameterQuantity; ParameterNumber++)
   {
      fprintf(hResultAnalysis,"%i\t",ParameterNumber);
      for (int CompareParameterNumber = 1; CompareParameterNumber<=ParameterQuantity; CompareParameterNumber++)
      {
         if (ParameterNumber != CompareParameterNumber)
         {
            double ComplexVariance = VarianceSequence[ParameterNumber-1] / (MemberQuantity*TryQuantity) + VarianceSequence[CompareParameterNumber-1] / (MemberQuantity*TryQuantity);
            double TestStatistics = (MeanSequence[ParameterNumber-1] - MeanSequence[CompareParameterNumber-1]) / sqrt(ComplexVariance);
            double Criterion = StudentCriterion(CriterionFilename, SignificanceLevel/2, MemberQuantity*TryQuantity - 1);
            if (abs(TestStatistics) > Criterion)
               fprintf(hResultAnalysis, "*\t");
            else
               fprintf(hResultAnalysis, "\t");
         }
         else
            fprintf(hResultAnalysis, "-\t");
      }
      fprintf(hResultAnalysis,"\n");
   }
   delete []MeanSequence;
   delete []VarianceSequence;
   fclose(hResultAnalysis);
}

/*// Составление отчета о среднем количестве нейронов и связей в зависимости от сложности среды
void MakeNetworkParametersSummary(int argc, char **argv)
{
   TTimeSettings TimeSettings;
   TMutationSettings MutationSettings;
   TModeSettings ModeSettings;
   TDirectorySettings DirectorySettings;
   TGeneralRunSettings GeneralRunSettings;
   double EnVariableProbability = 0;
   int ProgramMode = 0;
   bool NoEnVariableCheck = 0;
   // Сначала заполняем все настройки из файла
   FillSettingsFromFile(argv[1], &TimeSettings, &MutationSettings, &ModeSettings, &DirectorySettings, &GeneralRunSettings, &EnVariableProbability);
   // Заполняем настройки из командной строки (это только для родительского процесса)
   FillCommandParametrs(argc, argv, &GeneralRunSettings, &ModeSettings, &(MutationSettings.EnConProb), &(MutationSettings.DisConProb), &EnVariableProbability, &ProgramMode, &NoEnVariableCheck);

   char NoEnVariableString[10]; // Признак того, что прогонялись агенты, обучавшиеся на стационарной среде
   if (NoEnVariableCheck)
      strcpy(NoEnVariableString, "_noenvar");
   else
      strcpy(NoEnVariableString, "");

   char ResultAnalysisFilename[255];
   sprintf(ResultAnalysisFilename, "%s/BestPopulationAnalysis/BestPopAnalysisNetworkParam_en%i-%i%s_%.4f_%i", DirectorySettings.ResultDirectory, GeneralRunSettings.FirstEnvironmentNumber, GeneralRunSettings.LastEnvironmentNumber, NoEnVariableString, EnVariableProbability, GeneralRunSettings.VariableNumber);
   if (ModeSettings.RewardMode)
      strcat(ResultAnalysisFilename, "_pen");
   if (ModeSettings.EvolutionMode)
      strcat(ResultAnalysisFilename, "_mod");
   strcat(ResultAnalysisFilename, ".txt");

   FILE* hResultAnalysisFile = fopen(ResultAnalysisFilename, "w");

   // Структура, в которую будут загружаться все агенты перед всеми подсчетами
   TNetwork CurrentAgent;
   CurrentAgent.Neurons = NULL;

   for (int CurrentVarNumber=GeneralRunSettings.VariableNumber; CurrentVarNumber<=GeneralRunSettings.LastVariableNumber; CurrentVarNumber++)
      for (int CurrentEnNumber=GeneralRunSettings.FirstEnvironmentNumber; CurrentEnNumber<=GeneralRunSettings.LastEnvironmentNumber; CurrentEnNumber++)
      {
         double AverageNeuronQuantity = 0;
         double AverageSynapsesQuantity = 0;
         double AverageEnabledSynapsesQuantity = 0;
         for (int CurrentTryNumber=GeneralRunSettings.FirstTryNumber; CurrentTryNumber<=GeneralRunSettings.LastTryNumber; CurrentTryNumber++)
         {
            TRunSettings RunSettings;
            RunSettings.EnvironmentNumber = CurrentEnNumber;
            RunSettings.TryNumber = CurrentTryNumber;
            RunSettings.VariableNumber = CurrentVarNumber;

            char EnvironmentFilename[255];
            GetEnvironmentFilename(EnvironmentFilename, DirectorySettings.EnvironmentDirectory, RunSettings.EnvironmentNumber);

            // Загружаем среду
            TEnvironmentAims EnvironmentAims;
            InitEnvironment(&EnvironmentAims, EnvironmentFilename);

            char BestPopulationFilename[255];
            GetBestPopulationFilename(BestPopulationFilename, &RunSettings, &ModeSettings, DirectorySettings.ResultDirectory, NoEnVariableCheck);
            FILE* hBestPopulationFile = fopen(BestPopulationFilename, "r");

            for (int CurrentAgentNumber=0; CurrentAgentNumber<TimeSettings.AgentPopulationQuantity; CurrentAgentNumber++)
            {
               FillNextAgent(&CurrentAgent, hBestPopulationFile, EnvironmentAims.EnvironmentResolution, EnvironmentAims.OutputResolution, ModeSettings.NetworkMode);
               int CurrentEnabledSynapsesQuantity = 0;
               TNeuron* CurrentNeuron = CurrentAgent.Neurons;
               while (CurrentNeuron!=NULL)
               {
                  TSynapse* CurrentSynapse = CurrentNeuron->InSynapses;
                  while (CurrentSynapse!=NULL)
                  {
                     if (CurrentSynapse->Enabled)
                        CurrentEnabledSynapsesQuantity++;
                     CurrentSynapse = CurrentSynapse->next;
                  }
                  CurrentNeuron = CurrentNeuron->next;
               }
               AverageNeuronQuantity+= CurrentAgent.NeuronsQuantity / (double) TimeSettings.AgentPopulationQuantity;
               AverageSynapsesQuantity += CurrentAgent.SynapsesQuantity / (double) TimeSettings.AgentPopulationQuantity;
               AverageEnabledSynapsesQuantity += CurrentEnabledSynapsesQuantity / (double) TimeSettings.AgentPopulationQuantity;
            }
            fclose(hBestPopulationFile);
         }

         AverageNeuronQuantity /= GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1;
         AverageSynapsesQuantity /= GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1;
         AverageEnabledSynapsesQuantity /= GeneralRunSettings.LastTryNumber - GeneralRunSettings.FirstTryNumber + 1;
         fprintf(hResultAnalysisFile, "%i\t%.2f\t%.2f\t%.2f\n", CurrentEnNumber, AverageNeuronQuantity, AverageEnabledSynapsesQuantity, AverageSynapsesQuantity);
      }

   fclose(hResultAnalysisFile);
}*/

void MakeEnvironmentSummary(TEnvironmentAims* EnvironmentAims, int* AimsDistribution)
{
   for (int i=0; i<6; i++)
      AimsDistribution[i] = 0;
   for (int CurrentAim=0; CurrentAim<EnvironmentAims->AimQuantity; CurrentAim++)
      AimsDistribution[EnvironmentAims->Aim[CurrentAim].Complexity - 1] ++;
}

void MakeEnvironmentBlockSummary(int argc, char** argv)
{

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
   // Сначала заполняем все настройки из файла
   FillSettingsFromFile(argv[1], &TimeSettings, &MutationSettings, &ModeSettings, &DirectorySettings, &GeneralRunSettings, &PrimarySystemogenesisSettings, &LearningSettings, &EnVariableProbability);
   // Заполняем настройки из командной строки (это только для родительского процесса)
   FillCommandParametrs(argc, argv, &GeneralRunSettings, &ModeSettings, &(MutationSettings.EnConProb), &(MutationSettings.DisConProb), &EnVariableProbability, &ProgramMode, &NoEnVariableCheck);

   for (int EnvironmentNumber = GeneralRunSettings.FirstEnvironmentNumber; EnvironmentNumber <= GeneralRunSettings.LastEnvironmentNumber; EnvironmentNumber++)
   {
      char EnvironmentFilename[255];
      GetEnvironmentFilename(EnvironmentFilename, DirectorySettings.EnvironmentDirectory, EnvironmentNumber);
      TEnvironmentAims EnvironmentAims;
      InitEnvironment(&EnvironmentAims, EnvironmentFilename);

      int AimsDistribution[6];
      MakeEnvironmentSummary(&EnvironmentAims, AimsDistribution);

      char SummaryFilename[255];
      sprintf(SummaryFilename, "%s/SummaryEnvironment%i.txt", DirectorySettings.WorkDirectory, EnvironmentNumber);
      FILE* hSummaryFile = fopen(SummaryFilename, "w");
      double Mean = 0;
      for (int i=2; i<=5; i++)
         Mean += AimsDistribution[i-1];
      Mean /= 4;
      double Variance = 0;
      for (int i=2; i<=5; i++)
         Variance += (AimsDistribution[i-1]-Mean)*(AimsDistribution[i-1]-Mean)/3;


      for (int i=2; i<=5; i++)
         fprintf(hSummaryFile, "%i\t%.2f\n", AimsDistribution[i-1], (AimsDistribution[i-1]-Mean)/sqrt(Variance));

      fclose(hSummaryFile);
   }
}

void TransEnv(int argc, char** argv)
{
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
   // Сначала заполняем все настройки из файла
   FillSettingsFromFile(argv[1], &TimeSettings, &MutationSettings, &ModeSettings, &DirectorySettings, &GeneralRunSettings, &PrimarySystemogenesisSettings, &LearningSettings, &EnVariableProbability);
   // Заполняем настройки из командной строки (это только для родительского процесса)
   FillCommandParametrs(argc, argv, &GeneralRunSettings, &ModeSettings, &(MutationSettings.EnConProb), &(MutationSettings.DisConProb), &EnVariableProbability, &ProgramMode, &NoEnVariableCheck);

   for (int EnvironmentNumber = GeneralRunSettings.FirstEnvironmentNumber; EnvironmentNumber <= GeneralRunSettings.LastEnvironmentNumber; EnvironmentNumber++)
   {
      char EnvironmentFilename[255];
      GetEnvironmentFilename(EnvironmentFilename, DirectorySettings.EnvironmentDirectory, EnvironmentNumber);
      TEnvironmentAims EnvironmentAims;
      InitEnvironment(&EnvironmentAims, EnvironmentFilename);

      int AimsDistribution[6];
      MakeEnvironmentSummary(&EnvironmentAims, AimsDistribution);

      char SummaryFilename[255];
      sprintf(SummaryFilename, "%s/SummaryAimEnvironment%i.txt", DirectorySettings.WorkDirectory, EnvironmentNumber);
      FILE* hSummaryFile = fopen(SummaryFilename, "w");
      for (int i=1; i<=5; i++)
      {
         fprintf(hSummaryFile, "%i\t", i);
         for (int j=0; j < EnvironmentAims.AimQuantity; j++)
         {
            if (EnvironmentAims.Aim[j].Complexity < i)
               fprintf(hSummaryFile, "\t");
            else
               fprintf(hSummaryFile, "%i\t", 2 * EnvironmentAims.Aim[j].AimStructure[i-1].Number + EnvironmentAims.Aim[j].AimStructure[i-1].Value + 1);
         }
         fprintf(hSummaryFile, "\n");
      }

      fclose(hSummaryFile);
   }

}




