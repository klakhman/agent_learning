#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "types.h"
#include "service.h"

#include <time.h>

#include "environment.h"

// ��������� �������� ����� �� �����
void InitEnvironment(TEnvironmentAims* EnvironmentAims, char* EnvironmentFilename)
{
   FILE* f_EnvironmentFile = fopen(EnvironmentFilename,"r"); // ��������� ���� �����
   char StrFromFile[255];

   fscanf(f_EnvironmentFile, "%s", StrFromFile); // ��������� ����������� �����
   EnvironmentAims->EnvironmentResolution = atoi(StrFromFile);

   //����������� ����������� ��������� �������
   int OutResolution=-1;
   double ActionQuantity; // ���-�� ��������� ��� ����������� �������� ��� ������������ ���-�� �������� ��������
   do
   {
      OutResolution++;
      ActionQuantity = 1;
      // ������������ ���-�� ���������� (�������)
      for (int i=0; i<OutResolution; i++)
         ActionQuantity = ActionQuantity * (i+1);
      for (int i=0; i<OutResolution - 2; i++)
         ActionQuantity = ActionQuantity / (i+1);
      ActionQuantity = ActionQuantity / 2;
      //----------------------------------------
   } while (ActionQuantity < (2 * EnvironmentAims->EnvironmentResolution)); // ���� ���������� �������� �� ����� ����� ��� ������ ����������
   EnvironmentAims->OutputResolution = OutResolution;

   fscanf(f_EnvironmentFile, "%s", StrFromFile); // ��������� ���-�� �����
   EnvironmentAims->AimQuantity=atoi(StrFromFile);

   for (int CurrentAim=0; CurrentAim<EnvironmentAims->AimQuantity; CurrentAim++) // ��������� ��������� �����
   {
      fscanf(f_EnvironmentFile, "%s", StrFromFile); // ��������� ��������� �����
      EnvironmentAims->Aim[CurrentAim].Complexity=atoi(StrFromFile);
      fscanf(f_EnvironmentFile, "%s", StrFromFile); // ��������� ������� �� ����
      EnvironmentAims->Aim[CurrentAim].Reward=atof(StrFromFile);
      EnvironmentAims->Aim[CurrentAim].VisitTime = 0;

      for (int CurrentBit=0; CurrentBit<EnvironmentAims->Aim[CurrentAim].Complexity; CurrentBit++) // ��������� ��� ���� ����
      {
         fscanf(f_EnvironmentFile, "%s", StrFromFile);
         EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Number = atoi(StrFromFile);
         fscanf(f_EnvironmentFile, "%s", StrFromFile);
         EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Value = atoi(StrFromFile);
      }
    }

    fclose(f_EnvironmentFile);
}

//��������� ����������� �����
void ResetEnviroment(TEnvironmentAims* EnvironmentAims)
{
   for (int i=0; i<EnvironmentAims->AimQuantity; i++)
      EnvironmentAims->Aim[i].VisitTime = -100;
}

// ��������� ���������� ������������ ������������� �����
double CoefFullEnvironment(TEnvironmentAims* EnvironmentAims)
{
   //bool k_en[EnvironmentAims->EnvironmentResolution]; // ������ ��������� ������-���� ���� � ����
   bool* k_en = new bool[EnvironmentAims->EnvironmentResolution];
   double CoefFull=0.0; // ����������� ������������� �����

   for (int CurrentAim=0; CurrentAim<EnvironmentAims->AimQuantity; CurrentAim++)
   {
        for (int i=0; i<EnvironmentAims->EnvironmentResolution; i++) k_en[i] = false; // ������� ������ ���������

        for (int CurrentBit=0; CurrentBit<EnvironmentAims->Aim[CurrentAim].Complexity; CurrentBit++) // ����������� �����, �������� � ���� (���������� ������� ���������)
            k_en[EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Number] = true;

        double CoefFullAim = 1; // ��������� ����
        int k_enc = 0; // ���������� ��������� ����� �������� � ����

        // ���������� ��������� ����
        for (int k=0; k<EnvironmentAims->Aim[CurrentAim].Complexity; k++) CoefFullAim = CoefFullAim * ( 1.0 / (2.0F * EnvironmentAims->EnvironmentResolution)); // ������� ����������� ���������� k ���������������� ��������
        for (int k=0; k<EnvironmentAims->EnvironmentResolution; k++) k_enc += k_en[k]; // ������� ���-�� ����� �������� � ����
        for (int k=0; k<(EnvironmentAims->Aim[CurrentAim].Complexity - k_enc); k++) CoefFullAim=CoefFullAim*2; // ������� ����������� ���������� ����� � ����������� ���������� ���������
        //----------------------
        CoefFull+=CoefFullAim;
    }
    delete []k_en;

    return CoefFull;
}

// ��������� ���� ������ (!!!) ����� ������ �����
bool CompareDifLentghAim(TAim* FirstAim, TAim* SecondAim)
{
   // �������� ���� ������ ��������� - ������ ����� �� ����������� ������� ���� �����
   bool CheckStatus = true;
   // ���� ����� ����������� ������� � ������ ���� ������
   if (FirstAim->VisitTime > SecondAim->VisitTime)
      // ���� ����� ����������� ������� ������ ���� ������, ��� ����� ����� ������ ����, �� ��� �� ����� ���� ���������
      if (FirstAim->VisitTime > SecondAim->Complexity)
         CheckStatus = false;
      else
      {
         // ���������� �� ���� ����� �� ����������� ����� ������ ����
         for (int i=0; i<FirstAim->VisitTime; i++)
            // ���� ���� �� ���� �������� �������, �� ���� �������
            if ( (FirstAim->AimStructure[i].Value != SecondAim->AimStructure[i].Value) || (FirstAim->AimStructure[i].Number != SecondAim->AimStructure[i].Number) )
               CheckStatus = false;
      }
   else
      if (SecondAim->VisitTime > FirstAim->Complexity) //!! ���� ���������� FirstAim->VisitTime �� FirstAim->Complexity
         CheckStatus=false;
      else
      {
         for (int i=0; i<SecondAim->VisitTime; i++)
            if ( (FirstAim->AimStructure[i].Value != SecondAim->AimStructure[i].Value) || (FirstAim->AimStructure[i].Number != SecondAim->AimStructure[i].Number) )
               CheckStatus = false;
      }
   return CheckStatus;

}

// ��������� ��������� �����
double GenerateEnvironment(TEnvironmentAims* EnvironmentAims, int EnvironmentResolution, double RequiredEnvCoefFull, double Eps, int MaxComplexity, int MinMaxComplexity, int MinComplexity)
{
   int CoefOK = 0; // ���������� ������� ����, ��� �������� ����������� ������������� � �������� �������� ��������
   double RealEnvCoefFull;
   //int ReplyOK = 0;
   // ���������� ����������� �����
   EnvironmentAims->EnvironmentResolution = EnvironmentResolution;
   //����������� ����������� ��������� �������
   int OutResolution=-1;
   double ActionQuantity; // ���-�� ��������� ��� ����������� �������� ��� ������������ ���-�� �������� ��������
   do
   {
      OutResolution++;
      ActionQuantity = 1;
      // ������������ ���-�� ���������� (�������)
      for (int i=0; i<OutResolution; i++)
         ActionQuantity = ActionQuantity * (i+1);
      for (int i=0; i<OutResolution - 2; i++)
         ActionQuantity = ActionQuantity / (i+1);
      ActionQuantity = ActionQuantity / 2;
      //----------------------------------------
   } while (ActionQuantity < (2 * EnvironmentAims->EnvironmentResolution)); // ���� ���������� �������� �� ����� ����� ��� ������ ����������

   EnvironmentAims->OutputResolution = OutResolution;

   // ���������� �����, ���� �� �������� ������� ������������ �������������
   while (!CoefOK)
   {
      //!!!
      int FullAimQuantity = rand()%1000; //1100 ���������� ���-�� ������ ����� (��� ����� ��������)
      //FullAimQuantity = 100; //!!!!
      // ���������� ���� ����
      TEnvironmentAims FullAims;
      for (int CurrentAim=0; CurrentAim<FullAimQuantity; CurrentAim++)
      {
         // ������� ����� ������ ����
         FullAims.Aim[CurrentAim].Complexity = rand()%(MaxComplexity - MinMaxComplexity + 1) + MinMaxComplexity;
         // ������� ����� ����������� ������� (��� ���� �������� ���������� �� � ���������� VisitTime)
         FullAims.Aim[CurrentAim].VisitTime = rand()%(FullAims.Aim[CurrentAim].Complexity - MinComplexity + 1) + MinComplexity;
         int Fault = 0; // ������� ��������� ���� (��������� ����� ��������������)
         // ���������� ����� (������������ ������) ����
         do
         {
            Fault = 0;
            // ���������� ���������������� ����
            for (int CurrentBit=0; CurrentBit<FullAims.Aim[CurrentAim].Complexity; CurrentBit++)
            {
               int CurrentPosition;
			   bool CurrentValue; // ��������������� �������� ������� ��������� � �����������
               int Stop = 0; // ������� ����, ��� �������������� ��� ����������� ����������
               do //���������� ���������������� ��������
               {
                  Stop = 0;
                  // ���������� �������������� ��������
                  CurrentPosition = rand()%EnvironmentResolution;
                  CurrentValue = rand()%2;
                  int k = CurrentBit - 1;
                  // ����������� �� ���������� ��������� �� ���������� �������� � ��������� ���� �� ����
                  while ((!Stop) && (k!=-1))
                  {
                     if (FullAims.Aim[CurrentAim].AimStructure[k].Number == CurrentPosition) Stop = 1;
                     k--;
                  }
                  if (Stop == 1) // ���� ������� ����� ��������
                     if (FullAims.Aim[CurrentAim].AimStructure[k+1].Value != CurrentValue) // ���� ���������� ������� ��� ���������������, �� �������� ���������������
                        Stop = 0;

               }while (Stop);

               FullAims.Aim[CurrentAim].AimStructure[CurrentBit].Value = CurrentValue;
               FullAims.Aim[CurrentAim].AimStructure[CurrentBit].Number = CurrentPosition;
            }
            // ����� ��������� ���������������� ���� ���������� �������� �� �� ����� ����������� �� ������� ������������
            int CurrentCompareAim=0;
            while ((CurrentCompareAim < CurrentAim) && (!Fault))
            {
               // ���������� ��� ����
               Fault = CompareDifLentghAim(&(FullAims.Aim[CurrentAim]), &(FullAims.Aim[CurrentCompareAim]));
               // ���� ��������� ����������, ��� ��� ���������, �� �������� ��������� ����������� ��������� ������� ���� ���� ������ �� ���������
               while ((Fault) && (FullAims.Aim[CurrentAim].VisitTime<FullAims.Aim[CurrentAim].Complexity))
               {
                  FullAims.Aim[CurrentAim].VisitTime++;
                  Fault = CompareDifLentghAim(&(FullAims.Aim[CurrentAim]), &(FullAims.Aim[CurrentCompareAim]));
               }
               CurrentCompareAim++;
            }
         } while (Fault); // ������������� ���������� ����
      }

      // ����� ��������� ���� ������ ����� ���������� ������������� �������� ����� � ���������
      EnvironmentAims->AimQuantity = 0;
      // ������� ���-�� ���� �����
      for (int i=0; i<FullAimQuantity; i++)
         EnvironmentAims->AimQuantity += FullAims.Aim[i].Complexity - FullAims.Aim[i].VisitTime + 1;
      int CurrentAim=0; // ������� ����� � �������� �����
      for (int CurrentFullAim = 0; CurrentFullAim<FullAimQuantity; CurrentFullAim++)
         for (int CurrentSubAim = 0; CurrentSubAim<FullAims.Aim[CurrentFullAim].Complexity-FullAims.Aim[CurrentFullAim].VisitTime+1; CurrentSubAim++)
         {
            int Complexity = FullAims.Aim[CurrentFullAim].VisitTime + CurrentSubAim;
            double Reward = 0;
            switch (Complexity){
               case 2:
                  Reward = 20;
                  break;
               case 3:
                  Reward = 30;
                  break;
               case 4:
                  Reward = 40;
                  break;
               case 5:
                  Reward = 50;
                  break;
               case 6:
                  Reward = 60;
                  break;
            }

            EnvironmentAims->Aim[CurrentAim].Complexity = Complexity;
            EnvironmentAims->Aim[CurrentAim].Reward = Reward;
            for (int CurrentBit = 0; CurrentBit<Complexity; CurrentBit++)
            {
               EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Value = FullAims.Aim[CurrentFullAim].AimStructure[CurrentBit].Value;
               EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Number = FullAims.Aim[CurrentFullAim].AimStructure[CurrentBit].Number;
            }
            CurrentAim++;
         }

      RealEnvCoefFull = CoefFullEnvironment(EnvironmentAims);
      // ��������� �������� ����������� ������������� �� ��������� � �������� ��������
      CoefOK = (RealEnvCoefFull >= RequiredEnvCoefFull-Eps) && (RealEnvCoefFull <= RequiredEnvCoefFull+Eps);
      //CoefOK = (EnvironmentAims->AimQuantity == 100); //!!!
   }

   return RealEnvCoefFull;
}

// ������ ����� � ����
void WriteEnvironmentToFile(TEnvironmentAims* EnvironmentAims, char* EnvironmentFilename)
{
   FILE* hEnvironmentFile = fopen(EnvironmentFilename, "w");

   fprintf(hEnvironmentFile, "%i\n%i\n", EnvironmentAims->EnvironmentResolution, EnvironmentAims->AimQuantity);
   for (int CurrentAim=0; CurrentAim<EnvironmentAims->AimQuantity; CurrentAim++)
   {
      fprintf(hEnvironmentFile, "%i\t%.1f\n", EnvironmentAims->Aim[CurrentAim].Complexity,  EnvironmentAims->Aim[CurrentAim].Reward);
      for (int CurrentBit=0; CurrentBit <  EnvironmentAims->Aim[CurrentAim].Complexity; CurrentBit++)
         fprintf(hEnvironmentFile, "%i\t%i\t",  EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Number, EnvironmentAims->Aim[CurrentAim].AimStructure[CurrentBit].Value);
      fprintf(hEnvironmentFile, "\n");
   }

   fclose(hEnvironmentFile);
}

// ��������� ���� ����
void GenerateEnvironmentsPool(double StartCoefficient, double EndCoefficient, double Step, int ReplyEnvQuanity, int FirstEnvironmentID, char* EnvironmentDirectory, int EnvironmentResolution, double Eps, int MaxComplexity, int MinMaxComplexity, int MinComplexity)
{
   srand((unsigned) time(0)); // ������������� ���������� ��������� �����
   // ��� �������� ������� ���������� (����� �������� � ������� ��������)
   int tmp_rand = rand()%1000; // �������� ������ ���������� ��������� �����
   tmp_rand = rand()%1000; // �������� ������ ���������� ��������� �����
   tmp_rand = rand()%1000; // �������� ������ ���������� ��������� �����


   int EnvironmentsQuantity = static_cast<int>((EndCoefficient - StartCoefficient)/Step) + 1;

   char ListOfEnvFilename[255];
   sprintf(ListOfEnvFilename,"%s/ListOfEnvironments%i-%i.txt", EnvironmentDirectory, FirstEnvironmentID, FirstEnvironmentID + EnvironmentsQuantity*ReplyEnvQuanity - 1);
   FILE* hListOfEnvFile = fopen(ListOfEnvFilename, "w");

   for (int i=0; i<EnvironmentsQuantity; i++)
   {
      double RequiredCoefficient = StartCoefficient + i*Step;
      for (int j=0; j < ReplyEnvQuanity; j++)
      {
         TEnvironmentAims EnvironmentAims;
         double RealCoefficient = GenerateEnvironment(&EnvironmentAims, EnvironmentResolution, RequiredCoefficient, Eps, MaxComplexity, MinMaxComplexity, MinComplexity);

         char EnvironmentFilename[255];
         GetEnvironmentFilename(EnvironmentFilename, EnvironmentDirectory, FirstEnvironmentID + i*ReplyEnvQuanity + j);
         WriteEnvironmentToFile(&EnvironmentAims,EnvironmentFilename);
         printf("%i:\tAimQuantity = %i\t Coefficient = %.4f\n", FirstEnvironmentID + i*ReplyEnvQuanity+j, EnvironmentAims.AimQuantity, RealCoefficient);
         fprintf(hListOfEnvFile, "%i\t%.4f\t%.4f\t%i\n", FirstEnvironmentID + i*ReplyEnvQuanity+j, RealCoefficient, 1.0/RealCoefficient, EnvironmentAims.AimQuantity);
      }
   }

   fclose(hListOfEnvFile);
}















