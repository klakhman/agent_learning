#include <math.h>
#include <stdlib.h>

#include "types.h"
#include "service.h"
#include "service_network.h"

#include "mutation.h"


// ��������� ������� - �������� �� ������ ������, ������� ��������� ����� ���������� ���������� ���������
void MutationDeleteConPop(TPoolNetwork* KidPoolNetwork, int EvolutionStep, int DisLimit)
{
   TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
   while (CurPool!=NULL) // ������ �� ���� �����
   {
      TPoolConnection* CurConnection; // ���������� ������� �����
      TPoolConnection* PrevConnection = NULL; // ���������� ���������� �����
      CurConnection = CurPool->ConnectednessSet;
      while (CurConnection!=NULL) // ������ �� ���� �������� ������ �������� ����
         if ((EvolutionStep - CurConnection->DisStep >= DisLimit) && (CurConnection->DisStep)) // ���� ��� ����� ���� �������� � �� ��� ���� ������� �� ������
            if (PrevConnection==NULL) // ���� ��� ������ �����
            {
               CurPool->ConnectednessSet = CurConnection->next; // �� ������ ��������� � ����������������� ����
               delete CurConnection;
               CurConnection = CurPool->ConnectednessSet;
               KidPoolNetwork->ConnectionQuantity--;
            }
            else
            {
               PrevConnection->next = CurConnection->next; // ����� ������ ��������� � ���������� �����
               delete CurConnection;
               CurConnection = PrevConnection->next;
               KidPoolNetwork->ConnectionQuantity--;

            }
         else // ���� ����� �� ����� �������
         {
            PrevConnection = CurConnection;
            CurConnection = CurConnection->next;
         }
      CurPool = CurPool->next;
   }

   // ������ �� ����������� ��� ID ������ ����� ������������ ID �������� � ����������� ������(�� ��� ID ����� ���������� ������� - ��� �������)
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

// ��������� ������� - �������� �� ������ ������������ ������, ������� ��������� ����� ���������� ���������� ���������
void MutationDeletePredConPop(TPoolNetwork* KidPoolNetwork, int EvolutionStep, int DisLimit)
{
   TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
   while (CurPool!=NULL) // ������ �� ���� �����
   {
      TPoolPredConnection* CurPredConnection; // ���������� ������� ������������ �����
      TPoolPredConnection* PrevPredConnection = NULL; // ���������� ���������� ������������ �����
      CurPredConnection = CurPool->PredConnectednessSet;
      while (CurPredConnection!=NULL) // ������ �� ���� �������� ������������ ������ �������� ����
         if ((EvolutionStep - CurPredConnection->DisStep >= DisLimit) && (CurPredConnection->DisStep)) // ���� ��� ������������ ����� ���� ��������� � �� ��� ���� ������� �� ������
            if (PrevPredConnection==NULL) // ���� ��� ������ ������������ �����
            {
               CurPool->PredConnectednessSet = CurPredConnection->next; // �� ������ ��������� � ����������������� ����
               delete CurPredConnection;
               CurPredConnection = CurPool->PredConnectednessSet;
               KidPoolNetwork->PredConnectionQuantity--;
            }
            else
            {
               PrevPredConnection->next = CurPredConnection->next; // ����� ������ ��������� � ���������� �����
               delete CurPredConnection;
               CurPredConnection = PrevPredConnection->next;
               KidPoolNetwork->PredConnectionQuantity--;

            }
         else // ���� ������������ ����� �� ����� �������
         {
            PrevPredConnection = CurPredConnection;
            CurPredConnection = CurPredConnection->next;
         }
      CurPool = CurPool->next;
   }

   // ������ �� ����������� ��� ID ������������ ������ ����� ������������ ID �������� � ����������� ������(�� ��� ID ����� ���������� ������� - ��� �������)
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

// ��������� ������� - ������� ������� ������������� �����
void MutationConWeight(TPoolNetwork* KidPoolNetwork, double MutWeightProbability, double MutWeightMeanDisp, double MutWeightDispDisp)
{
   TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
   while (CurPool!=NULL) // ��������� �� ���� �����
   {
      if (UniformDistribution(0,1) < MutWeightProbability)
      {
         CurPool->BiasMean += UniformDistribution(-MutWeightMeanDisp, MutWeightMeanDisp);// �������� �������� �������
			// �����, ����� ��������� ���� �� ������ ����
         CurPool->BiasVariance = abs(CurPool->BiasVariance + UniformDistribution(-MutWeightDispDisp, MutWeightDispDisp));
      }
      //printf("%.2f\n",(rand() % ((int) (2*MutWeightDisp*1000) + 1) - MutWeightDisp*1000)/1000.0);
      TPoolConnection* CurConnection = CurPool->ConnectednessSet;
      while (CurConnection!=NULL) // ��������� �� ���� �������� ������ �������� ����
      {
         if ( (UniformDistribution(0,1) < MutWeightProbability) && (CurConnection->Enabled) ) // ���� ������� ����� ����� ���� � ����� ��������
         {
            CurConnection->WeightMean += UniformDistribution(-MutWeightMeanDisp, MutWeightMeanDisp); //�������� �������
				// �����, ����� ��������� ���� �� ������ ����
            CurConnection->WeightVariance = abs(CurConnection->WeightVariance + UniformDistribution(-MutWeightDispDisp, MutWeightDispDisp));
         }
         CurConnection = CurConnection->next;
      }
      CurPool = CurPool->next;
   }
}

// ��������� ������� - ���������/���������� ������
void MutationEnDisConnection(TPoolNetwork* KidPoolNetwork, double EnConProb, double DisConProb, int EvolutionStep)
{
   TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
   while (CurPool!=NULL) // ���������� �� ���� �����
   {
      TPoolConnection* CurConnection = CurPool->ConnectednessSet;
      while (CurConnection!=NULL) // ���������� �� ���� ������� ������ �������� ����
      {
         if (CurConnection->Enabled) // ���� ����� ��������
         {
            if (UniformDistribution(0, 1) < DisConProb) // ���� ������� ����� ����� ����
            {
               CurConnection->Enabled = false;
               CurConnection->DisStep = EvolutionStep;
            }
         }
         else // ���� ����� ���������
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

// ��������� ������� - ���������/���������� ������������ ������
void MutationEnDisPredConnection(TPoolNetwork* KidPoolNetwork, double EnConProb, double DisConProb, int EvolutionStep)
{
   TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
   while (CurPool!=NULL) // ���������� �� ���� �����
   {
      TPoolPredConnection* CurPredConnection = CurPool->PredConnectednessSet;
      while (CurPredConnection!=NULL) // ���������� �� ���� ������� ������������ ������ �������� ����
      {
         if (CurPredConnection->Enabled) // ���� ����� ��������
         {
            if (UniformDistribution(0, 1) < DisConProb) // ���� ������� ����� ����� ����
            {
               CurPredConnection->Enabled = false;
               CurPredConnection->DisStep = EvolutionStep;
            }
         }
         else // ���� ����� ���������
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

// ��������� ������� - ���������� �����
void MutationAddConnection(TPoolNetwork* KidPoolNetwork, double AddConnectionProb, bool EvolutionMode, double* CurrentInnovationNumber, double InitialDevelopSynapseProb)
{
   if (UniformDistribution(0, 1) < AddConnectionProb) // ���� ����� ����� �������
   {
      TNeuralPool* CurPool;
      TPoolConnection* CurConnection;

      // ���������� �������� ����������� ����� � ����������� ��������� �������
      int EnResolution = 0;
      int OutResolution = 0;
      CurPool = KidPoolNetwork->PoolsStructure;
      while ((CurPool!=NULL) && (CurPool->Type != 1)) // ��� ��� ������� � �������� ���� � ����� ������ ������, �� ��������� �� �� �����
      {
         if (CurPool->Type == 0) EnResolution++;
         if (CurPool->Type == 2) OutResolution++;
         CurPool = CurPool->next;
      }

      int PrePoolID, PostPoolID; // ���������������� � ����������������� ��� ������������� �����
      int OutCheck=0; // ���������� ������� ���������� ���������� �����
      int TryCount = 0; //���������� ������ ��������� ����� - �������, ����� �� ���� ������������
      do // ���� ���������� ���������� �����
      {
         PrePoolID = UniformDiscreteDistribution(1, KidPoolNetwork->PoolQuantity);
         PostPoolID = UniformDiscreteDistribution(1, KidPoolNetwork->PoolQuantity);

         CurConnection = GetPointer2PoolConnection(KidPoolNetwork, PrePoolID, PostPoolID);

         OutCheck=0;
         if (CurConnection==NULL) // �c�� ��� ����� ����� � ����
            if (PostPoolID > EnResolution) // ���� ����������������� ��� �� ��������� ������
               if ( (PostPoolID > EnResolution+OutResolution) || (PrePoolID <= EnResolution) || (PrePoolID > EnResolution+OutResolution) ) OutCheck=1;
                           //���� ����������������� � ���������������� ���� �� �������� ������������ ���������, �� ������ �� ����� ������ �����
         TryCount++;
           // ��� ���� ����� - ( (FaultQt>=1001) || ( (tmpConGen==NULL) && ( (OutNode>(EnResolution+OutResolution)) || (InNode<=EnResolution) || (InNode>(EnResolution+OutResolution)) ) && (OutNode>EnResolution) ) )
      }while ((!OutCheck) && (TryCount<1000)); // ���� �� ������ ���������� ����� ��� �������� ���������� ���-�� �������

       // ���� ������� ���������� ����� ��� ����������
      if (OutCheck)
      {
         TNeuralPool* PrePool = GetPointer2Pool(KidPoolNetwork, PrePoolID);// ������� ���������������� ���
         CurPool = GetPointer2Pool(KidPoolNetwork, PostPoolID);
         if (CurPool->ConnectednessSet==NULL) // ���� � ������������������ ���� ��� ��� ������� ������ (� ������� ������ � ���������� �������� ���� ������ �� ����������������� ���)
         {
            // ������� ����� �����
				CurPool->ConnectednessSet = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, UniformDistribution(-1, 1), 0, 1, ++(*CurrentInnovationNumber), 0, InitialDevelopSynapseProb, PrePool, CurPool, NULL); //!! ������������� ���.�������� [-1;1], ��������� [-0.1;0.1]
            CurConnection = CurPool->ConnectednessSet;
         }
         else
         {
            CurConnection = CurPool->ConnectednessSet;
            while (CurConnection->next!=NULL) CurConnection = CurConnection->next; // ������� ��������� �����
            // ������� ����� �����
				CurConnection->next = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, UniformDistribution(-1, 1), 0, 1, ++(*CurrentInnovationNumber), 0, InitialDevelopSynapseProb, PrePool, CurPool, NULL); //!! ������������� ���.�������� [-1;1], ��������� [-0.1;0.1]
            CurConnection=CurConnection->next;
         }
         // �������� ������������� ������ ������������������ ���� � ��������� ����, ���� ��������� ����� ����� ������ ������ ���� (������������ ��� ������������������ ������������� ���������)
         if ( (EvolutionMode) && (CurConnection->PrePool->Layer == CurConnection->PostPool->Layer) && (CurConnection->PrePool->ID != CurConnection->PostPool->ID) )
         {
            CurPool = KidPoolNetwork->PoolsStructure;
            while (CurPool!=NULL) // ���������� �� ���� �����
            {
               if ( (CurPool->Layer > CurConnection->PrePool->Layer) || (CurPool->ID == CurConnection->PostPool->ID) ) // �������� ���������������� ��� � ��� ���� � ����� ������� �����
                  CurPool->Layer++;
               CurPool = CurPool->next;
            }
         }
      }
   }
}

// ��������� ������� - ���������� ������������ ����� �����
void MutationAddPredConnection(TPoolNetwork* KidPoolNetwork, double AddPredConnectionProb, double* CurrentPredInnovationNumber, double InitialDevelopPredConProb)
{
   if (UniformDistribution(0, 1) < AddPredConnectionProb) // ���� ����� ����� �������
   {
      TNeuralPool* CurPool;
      TPoolPredConnection* CurPredConnection;

      // ���������� �������� ����������� ����� � ����������� ��������� �������
      int EnResolution = 0;
      int OutResolution = 0;
      CurPool = KidPoolNetwork->PoolsStructure;
      while ((CurPool!=NULL) && (CurPool->Type != 1)) // ��� ��� ������� � �������� ���� � ����� ������ ������, �� ��������� �� �� �����
      {
         if (CurPool->Type == 0) EnResolution++;
         if (CurPool->Type == 2) OutResolution++;
         CurPool = CurPool->next;
      }

      int PrePoolID, PostPoolID; // ���������������� � ����������������� ��� ������������� ������������ �����
      int OutCheck=0; // ���������� ������� ���������� ���������� ������������ �����
      int TryCount = 0; //���������� ������ ��������� ������������ ����� - �������, ����� �� ���� ������������
      do // ���� ���������� ���������� �����
      {
         PrePoolID = UniformDiscreteDistribution(1, KidPoolNetwork->PoolQuantity);
         PostPoolID = UniformDiscreteDistribution(1, KidPoolNetwork->PoolQuantity);

         CurPredConnection = GetPointer2PoolPredConnection(KidPoolNetwork, PrePoolID, PostPoolID);

         OutCheck=0;
         if (CurPredConnection==NULL) // �c�� ��� ����� ������������ ����� � ����
            if (PostPoolID > EnResolution + OutResolution) // ���� ����������������� ��� �� ��������� � �� �������� ������
               OutCheck=1;

         TryCount++;
           // ��� ���� ����� - ( (FaultQt>=1001) || ( (tmpConGen==NULL) && ( (OutNode>(EnResolution+OutResolution)) || (InNode<=EnResolution) || (InNode>(EnResolution+OutResolution)) ) && (OutNode>EnResolution) ) )
      }while ((!OutCheck) && (TryCount<1000)); // ���� �� ������ ���������� ������������ ����� ��� �������� ���������� ���-�� �������

       // ���� ������� ���������� ����� ��� ����������
      if (OutCheck)
      {
         TNeuralPool* PrePool = GetPointer2Pool(KidPoolNetwork, PrePoolID);// ������� ���������������� ���
         CurPool = GetPointer2Pool(KidPoolNetwork, PostPoolID);

         if (CurPool->PredConnectednessSet==NULL) // ���� � ������������������ ���� ��� ��� ������� ������������ ������ (� ������� ������ � ���������� �������� ���� ������ �� ����������������� ���)
            // ������� ����� ������������ �����
					CurPool->PredConnectednessSet = CreatePoolPredConnection(++KidPoolNetwork->PredConnectionQuantity, 1, ++(*CurrentPredInnovationNumber), 0, InitialDevelopPredConProb, PrePool, CurPool, NULL);
         else
         {
            CurPredConnection = CurPool->PredConnectednessSet;
            while (CurPredConnection->next!=NULL) CurPredConnection = CurPredConnection->next; // ������� ��������� �����
            // ������� ����� �����
				CurPredConnection->next = CreatePoolPredConnection(++KidPoolNetwork->PredConnectionQuantity, 1, ++(*CurrentPredInnovationNumber), 0, InitialDevelopPredConProb, PrePool, CurPool, NULL);
         }
      }
   }
}

// ��������� ������� - �������� �����
void MutationDeletePoolConnection(TPoolNetwork* KidNetwork, double DeleteConnectionProb)
{
   if (UniformDistribution(0, 1) < DeleteConnectionProb) // ���� ����� ����� �������
   {
      TNeuralPool* CurPool = KidNetwork->PoolsStructure;
      TPoolConnection* CurConnection;

      int EnabledConnectionQuantity = 0; // ���-�� ���������� ������ � ����
      // ������� ���-�� ���������� ������ � ����
      while (CurPool!=NULL) // ��������� �� ���� �����
      {
         CurConnection = CurPool->ConnectednessSet;
         while (CurConnection!=NULL) // ��������� �� ��� �������� ������ �������� ����
         {
            if (CurConnection->Enabled)
               ++EnabledConnectionQuantity;
            CurConnection = CurConnection->next;
         }
         CurPool = CurPool->next;
      }
      if (EnabledConnectionQuantity != 0) // ���� ���� ���� �� ���� �����
      {
         int DeletedConnectionID = UniformDiscreteDistribution(1, EnabledConnectionQuantity); // ����� ��������� �����

         CurPool = KidNetwork->PoolsStructure;
         // ���������� ��������� �����
         while ((DeletedConnectionID) && (CurPool!=NULL))
         {
            CurConnection = CurPool->ConnectednessSet;
            while ((DeletedConnectionID) && (CurConnection!=NULL))
            {
               if (CurConnection->Enabled)
                  --DeletedConnectionID;
               if (DeletedConnectionID) // ���� �� ��� �� ����� ��� �����, �� ��������� ������
                  CurConnection = CurConnection->next;
            }
            if (DeletedConnectionID)
               CurPool = CurPool->next;
         }
         // ������� ����� �� ���
         TPoolConnection* CurPrevConnection = CurPool->ConnectednessSet;
         if (CurPrevConnection->ID == CurConnection->ID) // ���� ��� ������ ����� � �������
         {
            CurPool->ConnectednessSet = CurConnection->next;
            delete CurConnection;
         }
         else
         {
            // ������� ����� ����� ���������
            while (CurPrevConnection->next->ID != CurConnection->ID)
               CurPrevConnection = CurPrevConnection->next;
            CurPrevConnection->next = CurConnection->next;
            delete CurConnection;
         }
         // ����������� ������
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

// ��������� ������� - �������� ����. �����
void MutationDeletePoolPredConnection(TPoolNetwork* KidNetwork, double DeletePredConnectionProb)
{
   if (UniformDistribution(0, 1) < DeletePredConnectionProb) // ���� ����� ����� �������
   {
      TNeuralPool* CurPool = KidNetwork->PoolsStructure;
      TPoolPredConnection* CurPredConnection;

      int EnabledPredConnectionQuantity = 0; // ���-�� ���������� ������ � ����
      // ������� ���-�� ���������� ������ � ����
      while (CurPool!=NULL) // ��������� �� ���� �����
      {
         CurPredConnection = CurPool->PredConnectednessSet;
         while (CurPredConnection!=NULL) // ��������� �� ��� �������� ������ �������� ����
         {
            if (CurPredConnection->Enabled)
               ++EnabledPredConnectionQuantity;
            CurPredConnection = CurPredConnection->next;
         }
         CurPool = CurPool->next;
      }
      if (EnabledPredConnectionQuantity != 0) // ���� ���� ���� �� ���� �����
      {
         int DeletedPredConnectionID = UniformDiscreteDistribution(1, EnabledPredConnectionQuantity); // ����� ��������� �����

         CurPool = KidNetwork->PoolsStructure;
         // ���������� ��������� �����
         while ((DeletedPredConnectionID) && (CurPool!=NULL))
         {
            CurPredConnection = CurPool->PredConnectednessSet;
            while ((DeletedPredConnectionID) && (CurPredConnection!=NULL))
            {
               if (CurPredConnection->Enabled)
                  --DeletedPredConnectionID;
               if (DeletedPredConnectionID) // ���� �� ��� �� ����� ��� �����, �� ��������� ������
                  CurPredConnection = CurPredConnection->next;
            }
            if (DeletedPredConnectionID)
               CurPool = CurPool->next;
         }
         // ������� ����� �� ���
         TPoolPredConnection* CurPrevPredConnection = CurPool->PredConnectednessSet;
         if (CurPrevPredConnection->ID == CurPredConnection->ID) // ���� ��� ������ ����� � �������
         {
            CurPool->PredConnectednessSet = CurPredConnection->next;
            delete CurPredConnection;
         }
         else
         {
            // ������� ����� ����� ���������
            while (CurPrevPredConnection->next->ID != CurPredConnection->ID)
               CurPrevPredConnection = CurPrevPredConnection->next;
            CurPrevPredConnection->next = CurPredConnection->next;
            delete CurPredConnection;
         }
         // ����������� ������
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

// ��������� ������� - ������� ���� (��� NEAT, ��� ���� ���������� ��������� ������������ � ��������)
void MutationInsertPool(TPoolNetwork* KidPoolNetwork, double InsertPoolProb, int EvolutionStep, double* CurrentInnovationNumber)
{
   if (UniformDistribution(0, 1) < InsertPoolProb) // ���� ����� ����� �������
   {
      TNeuralPool* CurPool = KidPoolNetwork->PoolsStructure;
      TPoolConnection* CurConnection;

      int EnabledConnectionQuantity = 0; // ���-�� ���������� ������ � ����
      // ������� ���-�� ���������� ������ � ����
      while (CurPool!=NULL) // ��������� �� ���� �����
      {
         CurConnection = CurPool->ConnectednessSet;
         while (CurConnection!=NULL) // ��������� �� ��� �������� ������ �������� ����
         {
            if (CurConnection->Enabled)
               EnabledConnectionQuantity++;
            CurConnection = CurConnection->next;
         }
         CurPool = CurPool->next;
      }

      int DividedConnectionID = UniformDiscreteDistribution(1, EnabledConnectionQuantity); // ����� ����������� �����

      CurPool = KidPoolNetwork->PoolsStructure;
      // ���������� ����������� �����
      while ((DividedConnectionID) && (CurPool!=NULL))
      {
         CurConnection = CurPool->ConnectednessSet;
         while ((DividedConnectionID) && (CurConnection!=NULL))
         {
            if (CurConnection->Enabled)
               DividedConnectionID--;
            if (DividedConnectionID) // ���� �� ��� �� ����� ��� �����, �� ��������� ������
               CurConnection = CurConnection->next;
         }
         CurPool = CurPool->next;
      }

      TPoolConnection* DividedConnection = CurConnection;
      int NewPoolLayer; // ���� ������ ����
      int CheckLayerShift = 0; // ������� ������������� ������ ����� � ��������� ����

       // ����������� ���� ������ ����
      if ( DividedConnection->PrePool->Layer < DividedConnection->PostPool->Layer ) // ���� ����������������� ��� ��������� � ����� ������� ����
         if ( DividedConnection->PostPool->Layer - DividedConnection->PrePool->Layer == 1) // ���� ���� ���������� � ������� �����
         {
            NewPoolLayer = DividedConnection->PostPool->Layer; // ���� ������ ���� ����� ���� ������������������ ����
            CheckLayerShift = 1;
         }
         else
            NewPoolLayer = DividedConnection->PrePool->Layer + 1; // ����� ��� ���������� � ��������� ���� ����� ����������������� ����
      else
         if( DividedConnection->PrePool->Layer > DividedConnection->PostPool->Layer ) // ���� ���������������� ���� ��������� � ����� ������� ����
            if ( DividedConnection->PrePool->Layer - DividedConnection->PostPool->Layer == 1) // ���� ���� ���������� � ������� �����
            {
               NewPoolLayer = DividedConnection->PrePool->Layer;// ���� ������ ���� ������ ���� ����������������� ����
               CheckLayerShift=1;
            }
            else
               NewPoolLayer = DividedConnection->PrePool->Layer - 1; // ����� ��� ���������� � ���� ����� ���������������� �����
         else // ���� ��� ���������� � ����� ����
         {
            NewPoolLayer = DividedConnection->PrePool->Layer;
            CheckLayerShift=2; // ������� ����, ��� ���� ���������� � ����� ����
         }

      // ���� ����� �������� ���� � ��������� ����, �� �������� ��� ��������
      if (CheckLayerShift)
      {
         CurPool = KidPoolNetwork->PoolsStructure;
         while (CurPool!=NULL) // ��������� �� ���� �����
         {
            if (CurPool->Layer >= NewPoolLayer) // ���� ���� �������� ���� ������ ���� ����� ���� ������������ ����
            {
               if (CheckLayerShift==2) // ���� ����������� ����� ���� ����� ������ ������ ����
               {
                  if ( (CurPool->Layer > NewPoolLayer) || (CurPool->ID == DividedConnection->PostPool->ID) ) // ���� ���� �������� ���� ������ ���� ������������ ��� ��� ����������������� ��� ����������� �����
                     CurPool->Layer = CurPool->Layer + 1;
               }
               else  // ���� ����������� ����� ����� �������� ������
                  CurPool->Layer = CurPool->Layer + 1;
            }

            CurPool = CurPool->next;
         }
      }

      // ������� ��������� ���
      CurPool = KidPoolNetwork->PoolsStructure;
      while (CurPool->next!=NULL)
         CurPool = CurPool->next;
      // ������� ����� ���
      TNeuralPool* NewPool; // ����� ���
      NewPool = CreateNeuralPool(++KidPoolNetwork->PoolQuantity, 1, 1/*!���������� ������� ��������� ����� ����!*/, UniformDistribution(-1,1), 0, NewPoolLayer, NULL, NULL, NULL); //!! ������������� ���.�������� [-1;1], ��������� [-0.1;0.1]
      CurPool->next = NewPool;

      // ��������� ����������� �����
      DividedConnection->Enabled = false;
      DividedConnection->DisStep = EvolutionStep;

      //��������� �������� ����� � ��������� ���
      NewPool->ConnectednessSet = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, UniformDistribution(-1, 1), 0, 1, ++(*CurrentInnovationNumber), 0, 1.0, DividedConnection->PrePool, NewPool, NULL); //!! ������������� ���.�������� [-1;1], ��������� [-0.1;0.1]

      //��������� �������� ����� � ����������������� ��� ����������� �����
      CurConnection = DividedConnection->PostPool->ConnectednessSet;
      while (CurConnection->next != NULL)
         CurConnection = CurConnection->next;
      CurConnection->next = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, UniformDistribution(-1, 1), 0, 1, ++(*CurrentInnovationNumber) , 0, 1.0, NewPool, DividedConnection->PostPool, NULL); //!! ������������� ���.�������� [-1;1], ��������� [-0.1;0.1]
   }
}

// ���������, ������� ������������ � ��������� ������� - ���������� ����, ����� ��������� ����������� ���������� � ������ ����
double F_duplicate(int PoolQuantity, int ConnectionQuantity, int PoolStandartAmount, int ConnectionStandartAmount)
{
   //double F = (double) 2*max_i(1,NeuronsQuantity-NetworkStandartSize);
   double F = max( 1.0, static_cast<double>(0.5*ConnectionQuantity/(static_cast<double>(ConnectionStandartAmount)) + 0.5*(PoolQuantity - PoolStandartAmount)) );
   return F;
}

// ��������� ������� - ���������� ����
void MutationPoolDuplicate(TPoolNetwork* KidPoolNetwork, double DuplicatePoolProb, double PoolDivisionCoefficient, int PoolStandartAmount, int ConnectionStandartAmount, double* CurrentInnovationNumber, double* CurrentPredInnovationNumber)
{
   TNeuralPool* CurPool;
   TPoolConnection* CurConnection;

   TNeuralPool* LastPool = KidPoolNetwork->PoolsStructure; // ������� ��������� ��� � ����
   while (LastPool->next!=NULL) // ������� ��������� ��� � ����
      LastPool = LastPool->next;

   int OldConnectionsQuantity = KidPoolNetwork->ConnectionQuantity; //�������� ���-�� ������ � ����
   int OldPoolsQuantity = KidPoolNetwork->PoolQuantity; // ������� ���-�� ����� � ����
   int OldPredConnectionQuantity = KidPoolNetwork->PredConnectionQuantity;
   CurPool = KidPoolNetwork->PoolsStructure;
   do       // �� ��� ��� ���� �� �������� ��� ������ ����, ��������� �� �� ����������� ��������������
   {
      if (CurPool->Type == 1) // ���� ������� ��� - ����������
      {
         // ��������� ����������� �� ��� (� ������ �������� ��������, ����������� ����������� ���������� � ������ ������������ ��������� � ��������)
         bool Duplicate = (UniformDistribution(0, 1) < DuplicatePoolProb/F_duplicate(OldPoolsQuantity, OldConnectionsQuantity, PoolStandartAmount, ConnectionStandartAmount));
         if (Duplicate) // ���� ��� �����������
         {
            if (CurPool->Capacity != 1) // ���� � ���� �� ������� ������ ���� ������ (����� ��������� ����������� = 1)
               CurPool->Capacity = static_cast<int>(CurPool->Capacity * PoolDivisionCoefficient + 0.5); //!!!!
            LastPool->next = CreateNeuralPool(++KidPoolNetwork->PoolQuantity, CurPool->Type, CurPool->Capacity, CurPool->BiasMean, CurPool->BiasVariance, CurPool->Layer, NULL, NULL, NULL);//!!!!!!!!! // �������� ������ ����

            LastPool = LastPool->next;

            CurConnection = CurPool->ConnectednessSet;
            TPoolConnection* LastConnectionInPool = NULL; // ���������� ��������� �������� ����� � ����� (� ���������� �� � �����) ����
            while (CurConnection!=NULL) // �������� ��� �������� ����� �� �������������� ���� � �����
            {
               if (CurConnection->ID<=OldConnectionsQuantity) // ���� ��� ������ ����� (�� ���� �� ��������� �������)
               {
                  if (LastConnectionInPool == NULL) //���� ������ ��� ���
                  {
                     if (CurConnection->PrePool->ID == CurPool->ID) // ���� ��� ����� �� ������ ����
								LastPool->ConnectednessSet = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, CurConnection->WeightMean, CurConnection->WeightVariance, CurConnection->Enabled, ++(*CurrentInnovationNumber), CurConnection->DisStep, CurConnection->DevelopSynapseProb, LastPool, LastPool, NULL);
                     else
                        LastPool->ConnectednessSet = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, CurConnection->WeightMean, CurConnection->WeightVariance, CurConnection->Enabled, ++(*CurrentInnovationNumber), CurConnection->DisStep, CurConnection->DevelopSynapseProb, CurConnection->PrePool, LastPool, NULL);
                     LastConnectionInPool = LastPool->ConnectednessSet;
                  }
                  else
                  {
                     if (CurConnection->PrePool->ID == CurPool->ID) // ���� ��� ����� �� ������ ����
                        LastConnectionInPool->next =  CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, CurConnection->WeightMean, CurConnection->WeightVariance, CurConnection->Enabled, ++(*CurrentInnovationNumber), CurConnection->DisStep, CurConnection->DevelopSynapseProb, LastPool, LastPool, NULL);
                     else
                        LastConnectionInPool->next =  CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, CurConnection->WeightMean, CurConnection->WeightVariance, CurConnection->Enabled, ++(*CurrentInnovationNumber), CurConnection->DisStep, CurConnection->DevelopSynapseProb, CurConnection->PrePool, LastPool, NULL);
                     LastConnectionInPool = LastConnectionInPool->next;
                  }
               }
               CurConnection = CurConnection->next;
            }

            TNeuralPool* CurInputPool = KidPoolNetwork->PoolsStructure; // ���������� �������� �������������� ������������������ ���� ��� ������ �� ������ ����
            // �������� ��� ��������� ����� �� �������������� ���� (��� ����� ���� �������� �� ���� ����, ��� ��� ��� ����� ����������)
            while ((CurInputPool!=NULL)&&(CurInputPool->ID <= OldPoolsQuantity)) // �������� �� ���� ����������� �����
            {
               if (CurInputPool->ID != CurPool->ID)
               {
                  CurConnection = CurInputPool->ConnectednessSet;
                  while (CurConnection!=NULL)
                  {
                     if (CurConnection->ID<=OldConnectionsQuantity) // ���� ��� ������ ����� (�� ���� �� ��������� �������)
                        if (CurConnection->PrePool->ID == CurPool->ID) // ���� � �������� �������������� ���� ���� ����� � ������� ������������� �����
                        {
                           LastConnectionInPool = CurConnection;
                           while (LastConnectionInPool->next!=NULL) LastConnectionInPool = LastConnectionInPool->next; // ������� ��������� �����
                           LastConnectionInPool->next = CreatePoolConnection(++KidPoolNetwork->ConnectionQuantity, CurConnection->WeightMean/2.0, CurConnection->WeightVariance, CurConnection->Enabled, ++*CurrentInnovationNumber, CurConnection->DisStep, CurConnection->DevelopSynapseProb, LastPool, CurConnection->PostPool, NULL);
                           CurConnection->WeightMean = CurConnection->WeightMean/2.0; // �������� ����� �������������� ������� �� ����� ������� ����� ����� � �������������
                        }
                     CurConnection = CurConnection->next;
                  }
               }
               CurInputPool = CurInputPool->next;
            }

            //!! ������ ���� ����� (��������) �� ����� ������������� ������� �������� ����
            TPoolPredConnection* CurPredConnection = CurPool->PredConnectednessSet;
            TPoolPredConnection* LastPredConnectionInPool = NULL; // ���������� ��������� �������� ����� � ����� (� ���������� �� � �����) ����
            while (CurPredConnection!=NULL) // �������� ��� �������� ����� �� �������������� ���� � �����
            {
               if (CurPredConnection->ID<=OldPredConnectionQuantity) // ���� ��� ������ ����� (�� ���� �� ��������� �������)
               {
                  if (LastPredConnectionInPool == NULL) //���� ������ ��� ���
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

            CurInputPool = KidPoolNetwork->PoolsStructure; // ���������� �������� �������������� ������������������ ���� ��� ������ �� ������ ����
            // �������� ��� ��������� ����� �� �������������� ���� (��� ����� ���� �������� �� ���� ����, ��� ��� ��� ����� ����������)
            while ((CurInputPool!=NULL)&&(CurInputPool->ID <= OldPoolsQuantity)) // �������� �� ���� ����������� �����
            {
               if (CurInputPool->ID != CurPool->ID)
               {
                  CurPredConnection = CurInputPool->PredConnectednessSet;
                  while (CurPredConnection!=NULL)
                  {
                     if (CurPredConnection->ID<=OldPredConnectionQuantity) // ���� ��� ������ ����� (�� ���� �� ��������� �������)
                        if (CurPredConnection->PrePool->ID == CurPool->ID) // ���� � �������� �������������� ���� ���� ����� � ������� ������������� �����
                        {
                           LastPredConnectionInPool = CurPredConnection;
                           while (LastPredConnectionInPool->next!=NULL) LastPredConnectionInPool = LastPredConnectionInPool->next; // ������� ��������� �����
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

// ��������� ������� ����������� �������� ������� �� ����� ����� ������
void MutationDevelopSynapseProb(TPoolNetwork* KidPoolNetwork, double MutDevelopSynapseProbProb, double MutDevelopSynapseProbDisp){
	TNeuralPool* CurrentPool = KidPoolNetwork->PoolsStructure;
	while (NULL != CurrentPool){
		TPoolConnection* CurrentConnection = CurrentPool->ConnectednessSet;
		while (NULL != CurrentConnection){
			if (UniformDistribution(0, 1) < MutDevelopSynapseProbProb)
				// �����, ����� ����������� ���� ����� ����� � ��������
				CurrentConnection->DevelopSynapseProb = min(1.0, max(0.0, CurrentConnection->DevelopSynapseProb + UniformDistribution(-MutDevelopSynapseProbDisp, MutDevelopSynapseProbDisp)));
			CurrentConnection = CurrentConnection->next;
		}
		CurrentPool = CurrentPool->next;
	}
}

// ��������� ������� ����������� �������� ������������ ����� �� ������������ ����� ����� ������
void MutationDevelopPredConProb(TPoolNetwork* KidPoolNetwork, double MutDevelopPredConProbProb, double MutDevelopPredConProbDisp){
	TNeuralPool* CurrentPool = KidPoolNetwork->PoolsStructure;
	while (NULL != CurrentPool){
		TPoolPredConnection* CurrentPredConnection = CurrentPool->PredConnectednessSet;
		while (NULL != CurrentPredConnection){
			if (UniformDistribution(0, 1) < MutDevelopPredConProbProb)
				// �����, ����� ����������� ���� ����� ����� � ��������
				CurrentPredConnection->DevelopPredConProb = min(1.0, max(0.0, CurrentPredConnection->DevelopPredConProb + UniformDistribution(-MutDevelopPredConProbDisp, MutDevelopPredConProbDisp)));
			CurrentPredConnection = CurrentPredConnection->next;
		}
		CurrentPool = CurrentPool->next;
	}
}

// ��������� ��������� ������� �� ���� ���������
void ComposeOffspringFromParents(TPoolNetwork* KidPoolNetwork, TPoolNetwork* FirstParentNetwork, TPoolNetwork* SecondParentNetwork)
{

   TPoolNetwork* MoreFitness;
   TPoolNetwork* LessFitness;
   // ������� ����� � ����� ���������������� ��������
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

   // ��������� �� ��� ����� ������� � ������� ���� �� �� ������ �������� ����� ����
  /* TNeuron* FirstTempNeuron = KidNetwork->Neurons;
   while (FirstTempNeuron!=NULL)
   {
      TSynapse* FirstTempSynapse = FirstTempNeuron->InSynapses;
      while (FirstTempSynapse!=NULL)
      {
         TSynapse* MatchSynapse = NULL; // ����������� ��� (���� ������)
         // ���� ����������� ���
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

         if (MatchSynapse != NULL) // ���� ������ ����������� ���, �� ����������
         {
            if ((FirstTempSynapse->Enabled && !MatchSynapse->Enabled) || (!FirstTempSynapse->Enabled && MatchSynapse->Enabled)) // ���� � ����� �� ��������� ���� ��� ��������, � � ������ �������
            {
               FirstTempSynapse->Enabled = (rand()%1000 > 500); // �������� ���������� ������� �� ���
               if (FirstTempSynapse->Enabled) // ���� � ����� ��� �������, �� ������ �������� ����, � ������� ������� ����� ��. ����� ����������
                  FirstTempSynapse->DisStep = 0;
               else // �����
                  if (!FirstTempSynapse->DisStep) // ���� � ����� ��������������� �������� ���� ��� ��� �������
                     FirstTempSynapse->DisStep = MatchSynapse->DisStep; // �� ������������ ����� ����� �� ������� �������� (����� ����� ����� ��� � ��� �������)
            }

            // ��������� ���
            FirstTempSynapse->Weight += MatchSynapse->Weight;
            FirstTempSynapse->Weight = FirstTempSynapse->Weight / 2.0;
            //if (rand()%1000 > 500) // � ������ ������������ �������� ������� ����������� ������ �� ���������
            //   FirstTempSynapse->Weight = MatchSynapse->Weight;

         }

         FirstTempSynapse = FirstTempSynapse->next;
      }

      FirstTempNeuron = FirstTempNeuron->next;
   }*/
}

// ��������� ��������� ������ �������
void GenerateOffspring(TPoolNetwork* KidPoolNetwork, TPoolNetwork* FirstParentNetwork, TPoolNetwork* SecondParentNetwork, int EvolutionStep, TMutationSettings* Settings, bool EvolutionMode, double* CurrentInnovationNumber, double* CurrentPredInnovationNumber, double InitialDevelopSynapseProb, double InitialDevelopPredConProb)
{
   ComposeOffspringFromParents(KidPoolNetwork, FirstParentNetwork, SecondParentNetwork); // ������� ���������� ���� ������� ����� ����������� ���������

   MutationDeleteConPop(KidPoolNetwork, EvolutionStep, Settings->DisLimit); // ������� - �������� ���������� �����
   MutationDeletePredConPop(KidPoolNetwork, EvolutionStep, Settings->DisLimit); // ������� - �������� ���������� �����

   if (!EvolutionMode) // ���� ������������ �������� - NEAT
      MutationInsertPool(KidPoolNetwork, Settings->InsertPoolProb, EvolutionStep, CurrentInnovationNumber); // ������� - ������� ����
   else
      MutationPoolDuplicate(KidPoolNetwork, Settings->DuplicatePoolProb, Settings->PoolDivisionCoef, Settings->PoolStandartAmount, Settings->ConnectionStandartAmount, CurrentInnovationNumber, CurrentPredInnovationNumber); // ������� ���������� �����

   MutationEnDisConnection(KidPoolNetwork, Settings->EnConProb, Settings->DisConProb, EvolutionStep); // ������� - ���������/���������� ������

	MutationAddConnection(KidPoolNetwork, Settings->AddConnectionProb, EvolutionMode, CurrentInnovationNumber, InitialDevelopSynapseProb);

	MutationAddPredConnection(KidPoolNetwork, Settings->AddPredConnectionProb, CurrentPredInnovationNumber, InitialDevelopPredConProb);

   MutationDeletePoolConnection(KidPoolNetwork, Settings->DeleteConnectionProb);

   MutationDeletePoolPredConnection(KidPoolNetwork, Settings->DeletePredConnectionProb);

   MutationConWeight(KidPoolNetwork, Settings->MutWeightProbability, Settings->MutWeightMeanDisp, Settings->MutWeightDispDisp); // ������� ����� ������

	MutationDevelopSynapseProb(KidPoolNetwork, Settings->MutDevelopConProbProb, Settings->MutDevelopConProbDisp);

	MutationDevelopPredConProb(KidPoolNetwork, Settings->MutDevelopConProbProb, Settings->MutDevelopConProbDisp);
}

//��������� ��������� ������ ������, ������������ � ���������� ���������
int RouletteWheelSelection(double* PopulationFitness, int PopulationQuantity)
{
   double TotalFitness = 0; // ������ ������� ���������
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

// ����� �� ������� �� ������ ������ ������
double RewardPenalty(TPoolNetwork* PoolNetwork, int PenaltyRewardLimit)
{
   TNeuralPool* CurPool;
   TPoolConnection* CurConnection;
   int EnabledConnectionQuantity=0; // ���-�� ���������� ������ � ����

   //������������ ���-�� ���������� ������ � ����
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

// ��������� ��������� ������ ��������� � ������ ������� �� �����
void GenerateNextPopulation(TAgentGenomePopulation* AgentGenomePopulation, TMutationSettings* MutationSettings, int EvolutionStep, bool EvolutionMode, bool RewardMode, double* CurrentInnovationNumber, double* CurrentPredInnovationNumber, double InitialDevelopSynapseProb, double InitialDevelopPredConProb)
{
   //double PopulationReward[AgentGenomePopulation->PopulationAgentQuantity]; // ������ �� ����� ��������� ��������
   double* PopulationReward = new double[AgentGenomePopulation->PopulationAgentQuantity]; // ������ �� ����� ��������� ��������
   double RewardSum = 0.0; // ����� ���� ������ �������
   for (int i=0; i<AgentGenomePopulation->PopulationAgentQuantity; i++) // ��������� ������ � ��������� ���������
      if (RewardMode) // ���� ������������ �������� �������
      {
         PopulationReward[i] = max( 0.0, AgentGenomePopulation->AgentGenome[i]->Reward - RewardPenalty(AgentGenomePopulation->AgentGenome[i], MutationSettings->PenaltyRewardLimit) ); // ��������� ����� �� ������ ������
         RewardSum += PopulationReward[i];
      }
      else
      {
         PopulationReward[i] = AgentGenomePopulation->AgentGenome[i]->Reward;
         RewardSum += PopulationReward[i];
      }


   TAgentGenomePopulation *NewGenomePopulation;// ����� ���������
   NewGenomePopulation = new TAgentGenomePopulation;
   NewGenomePopulation->PopulationAgentQuantity = AgentGenomePopulation->PopulationAgentQuantity;
   for (int i=0;i<AgentGenomePopulation->PopulationAgentQuantity;i++) // ���������� ����� ���������
   {
      int FirstParentAgent;
      int SecondParentAgent;
      // ���������� ����� ������������� ������
      if (RewardSum) // ���� ���� ���� ���� �����, ������� ������ ���� �������-�� �������
      {
         FirstParentAgent = RouletteWheelSelection(PopulationReward, AgentGenomePopulation->PopulationAgentQuantity) - 1;
         int CheckCount = 0;
         do
         {
            SecondParentAgent =  RouletteWheelSelection(PopulationReward, AgentGenomePopulation->PopulationAgentQuantity) - 1;
            CheckCount++;
         } while ((FirstParentAgent == SecondParentAgent)&&(CheckCount<200));

         if (FirstParentAgent == SecondParentAgent) //���� �� ������� ����� ������� ������ (������ ��� � ���� ��������� ����� ��������� �������)
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
      // ����� ����� ��������������� �������� ������ ����� �� ������ �����
      //printf("_%i_%i_", FirstParentAgent, SecondParentAgent);
      if (AgentGenomePopulation->AgentGenome[FirstParentAgent]->Reward < AgentGenomePopulation->AgentGenome[SecondParentAgent]->Reward)
      {
         int tmp = FirstParentAgent;
         FirstParentAgent = SecondParentAgent;
         SecondParentAgent = tmp;
      }
      NewGenomePopulation->AgentGenome[i] = CreatePoolNetwork(); // �������������� ������
		GenerateOffspring(NewGenomePopulation->AgentGenome[i], AgentGenomePopulation->AgentGenome[FirstParentAgent], AgentGenomePopulation->AgentGenome[SecondParentAgent], EvolutionStep, MutationSettings, EvolutionMode, CurrentInnovationNumber, CurrentPredInnovationNumber, InitialDevelopSynapseProb, InitialDevelopPredConProb);
      NewGenomePopulation->AgentGenome[i]->Parents[0] = FirstParentAgent;
      NewGenomePopulation->AgentGenome[i]->Parents[1] = SecondParentAgent;
   }
   delete []PopulationReward;


   // ������ ��������� ����� ��������� � ������ (�� �� ���������, � ������ ������)
   for (int i=0; i<AgentGenomePopulation->PopulationAgentQuantity; i++)
   {
      AgentGenomePopulation->AgentGenome[i] = DeletePoolNetwork(AgentGenomePopulation->AgentGenome[i]);
      AgentGenomePopulation->AgentGenome[i] = NewGenomePopulation->AgentGenome[i];
   }

   delete NewGenomePopulation;
}
