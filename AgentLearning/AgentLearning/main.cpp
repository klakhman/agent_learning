#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <new>

#include "parallel.h"
#include "single_process.h"
#include "analysis.h"
#include "single_process.h"
#include "service.h"
#include "service_network.h"
#include "primary_systemogenesis.h"


#include "agentlife.h"
#include "environment.h"

//#include "windows.h"
int count;
void out_of_memory(){
	printf("\nOUT OF MEMORY\n");
	exit(1);
}

int main(int argc, char **argv)
{
	std::set_new_handler(out_of_memory);
	//printf("%i %i", sizeof(long double), sizeof(float));
	//char c= getchar();
	//srand(static_cast<unsigned int>(time(0)));
	/*int scale = 10000;
	printf("%.10f", 5148.498047 - 1.0F/static_cast<double>(scale));
	for (int i=0; i<100000000; ++i){
		double r = UniformDistribution(0.0,5148.498047, false, false);
		if (r >5148) printf("%.12f\n", r);
	}
		printf("%.10f", 5148.498047 - 1.0F/10000.0F);

	printf("Done!");
	char c= getchar();*/
   SerialTasks(argc, argv);
   /*char tmp_str[255];
   sprintf(tmp_str, "ghk");
   int i = atoi(tmp_str);
   printf("%i", i);
   printf("++");
   char c=getchar();*/
   /*FILE* hRecordFile = fopen("C:/Temp/input.txt", "r");
   char tmp_str[255];
   for (int i=0; i<5; ++i)
   {
      sprintf(tmp_str, "-");
      double j=0;
      int i = fscanf(hRecordFile, "%f", &j);
      if (i == EOF) printf("!!!!");
      printf("%.3f\t%i\n", j, i);
   }
   fscanf(hRecordFile, "%s", tmp_str);
   printf("%s\n", tmp_str);

   printf("+\n");
   char c = getchar();*/
   /*srand((unsigned int) time(0));
   TPoolNetwork* PoolNetwork;
   PoolNetwork = CreatePoolNetwork();
   FillSpecificPoolNetwork(PoolNetwork, "C:/Temp/TestPoolNetwork.txt", 1, 3, 2, 0);
   PoolNetwork2Dot(PoolNetwork, "C:/Temp/TestPoolNetwork.dot");
   TNeuralNetwork* NeuralNetwork;
   NeuralNetwork = CreateNeuralNetwork();

   TPrimarySystemogenesisSettings PrimSysSettings;
   PrimSysSettings.DevelopSynapseProbability = 0.5;
   PrimSysSettings.PrimarySystemogenesiTime = 300;
   PrimSysSettings.SpontaneousActivityProb = 0.2;
   PrimSysSettings.ActiveNeuronsPercent = 50.0;
   PrimSysSettings.SynapsesActivityTreshold = 0;
   PrimSysSettings.SignificanceTreshold = 0.5;
   AgentPrimarySystemogenesis(NeuralNetwork, PoolNetwork, &PrimSysSettings);
   //BuildPrimaryNetwork(NeuralNetwork, PoolNetwork, 0.5);
   NeuralNetwork2Dot(NeuralNetwork, "C:/Temp/TestNeuralNetwork.dot");
   FILE* hNetworkFile = fopen("C:/Temp/TempSysNeuralNetwork.txt","w");
   RecordNeuralNetwork(NeuralNetwork, 1, hNetworkFile);
   fclose(hNetworkFile);
   PoolNetwork = DeletePoolNetwork(PoolNetwork);
   NeuralNetwork = DeleteNeuralNetwork(NeuralNetwork);
   printf("+");
   char c=getchar();*/
   /*double step=0.01;
   double sigma = 1.0;
   double mean = 0;
   int Hist[900];
   for (int i=0; i<900; ++i)
      Hist[i] =0;
   int repeat = 100000000;
   for (int i=0; i<repeat; ++i)
   {
      double tmp = NormalDistribution(mean, sigma);
      for (int j=-451; j <= 450; ++j)
         if ((tmp>mean + (j-1)*step) && (tmp<=mean + j*step))
            Hist[j+450] += 1;
   }
   FILE* hOUT = fopen("C:/Temp/Hist.txt", "w");
   for (int i=0; i<900; ++i)
      fprintf(hOUT,"%i\n", Hist[i]);
   fclose(hOUT);
   printf("OK");
   char c= getchar();*/
   /*int repeat = 1000000000;
   int before=0;
   int after =0;
   int middle = 0;
   for (int i=0; i<repeat; ++i)
   {
      double tmp = UniformDistribution(0,1);
      if (tmp>0.5)
         ++after;
      if (tmp<0.5)
         ++before;
      if (tmp==0.5)
         ++middle;
   }
   printf("%.7f\t%.7f\t%.10f\t%i", ((double)before)/repeat, ((double)after)/repeat, ((double)middle)/repeat, middle);
   char c= getchar();*/
   /*srand((unsigned int) time(NULL));
   int max = -100000;
   int min = 1000000;
   int A = 0;
   int B = 10000;
   int repeat = 1000000000;
   int freq[B-A+1];
   for (int i=0; i<B-A+1; ++i)
      freq[i]=0;
   int sum = 0;
   for (int i=0; i < repeat; ++i)
   {
      ++sum;
      int tmp = UniformDiscreteDistribution(A,B);
      if (max < tmp) max = tmp;
      if (min > tmp) min = tmp;
      freq[tmp-A] += 1;
   }
   printf("%i\n%i\t%i\n",sum,min, max);
   for (int i=B-A; i>=0; --i)
      printf("%i:\t%.5f\n", i+A , freq[i]/((double)repeat));*/
   //printf("%i", sizeof(int));
   //char c=getchar();
   /*int ProgramMode = 0; // Режим работы комплекса: 0 - эволюция; 1 - анализ
   int CurrentArgNumber = 2; // Текущий номер параметра (в первом записан путь к файлу настроек)
   while (CurrentArgNumber < argc) // Пока мы не прошли все параметры
   {
      if (argv[CurrentArgNumber][1] == 'm') // Расшифровываем параметр и заполняем (по второму полю, так как в первом "-")
         ProgramMode = atoi(argv[++CurrentArgNumber]);
      ++CurrentArgNumber;
   }
   if (!ProgramMode)
      ParallelTasks(argc, argv);
   else
      if (ProgramMode==1)
         ParallelAnalysis(argc, argv);
      else
         if (ProgramMode==2)
            ;//MakeNetworkParametersSummary(argc, argv);
         else
            if (ProgramMode==3)
               TransEnv(argc, argv);//MakeEnvironmentBlockSummary(argc, argv);
            else
               printf("Error: Unknown program mode...");*/

   //CheckExternalPairsStatSignificance("C:/Temp/BestPopAnalysis_en1001-1360_0.0000_608_mod.txt", "C:/Temp/BestPopAnalysis_en1001-1360_0.0000_100_mod.txt", "C:/Temp/result.txt", "", 0.05, 18, 20, 10);

   //CheckInternalStatSignificance("C:/Temp/BestPopAnalysis_en1001-1360_0.0000_108_mod.txt", "C:/Temp/result1.txt", "", 0.05, 18, 20, 10);

   //PrintLifeRecord(argc, argv);


   //GenerateEnvironmentsPool(0.7, 0.7, 0.05, 5, 96, "D:/temp/", 8, 0.001, 5, 3, 2);
   //GenerateEnvironmentsPool(0.25, 0.61, 0.05, 20, 1201, "D:/temp/", 8, 0.001, 5, 3, 2);
   /*for (int i=1001; i<=1360; i++)
   {
      char str[255];
      sprintf(str,"C:/temp2/En%i",i);
      CreateDirectory(str, NULL);
   }*/

   /*TNetwork AgentNetwork;
   AgentNetwork.Neurons = NULL;
   FillSpecificAgent(&AgentNetwork, "D:/Results/En20/EvolutionWithPenalty/En20_0(2)_maxaverage_pen.txt", 130, 8, 7, 0);
   TEnvironmentAims EnvironmentAims;
   InitEnvironment(&EnvironmentAims, "D:/Environments/Environment20.txt");

   srand((unsigned) time(0));
   rand()%1000;
   rand()%1000;
   rand()%1000;

   AgentLifeView(&AgentNetwork, &EnvironmentAims, 30, 0, 250, 0);*/


   //SingleAnalysis(argc,argv);
   //SerialTasks(argc, argv);
   return 0;
}
