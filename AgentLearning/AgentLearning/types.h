#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

const double Empty = -13; // ������� ������������� ������ �������
const int MaxComplexity = 6; // ������������ ��������� ����
const int MaxAimQuantity = 2000; // ������������ ���-�� ����� � �����
const int MaxAgentQuantity = 300; // ������������ ���-�� ������� � ���������
const int MessageLenght= 200; // ����������� ����� ��������� ����� ����������
const int HistogramCount = 10; //���-�� �������� � ������������� ����������� ������������� ������
const int RunAgentNumber = 500; // ���-�� �������� ������� ������ � �������� �������
//const int InitialPoolCapacity = 1; // ����������� ����������� ������� ����

//! - ����� ���� ----------------

// ��� "����� ����� ������"
struct TPoolConnection
{
   int ID;
   double WeightMean; // ���. �������� ���� �������
   double WeightVariance; // ��������� ���� �������
   bool Enabled; // ������� ��������� ����� (�������/��������)
   double InnNumber; // ����� ��������� �����
   int DisStep; // ����� ���������, ����� ��� ����� ���� ���������
   struct TNeuralPool* PrePool; // ������ �� "����������������" ���
   struct TNeuralPool* PostPool; // ������ �� "�����������������" ���
   struct TPoolConnection* next;
};

// ��� "������������ ����� ����� ������"
struct TPoolPredConnection
{
   int ID;
   bool Enabled; // ������� ��������� ������������ �����
   double InnNumber; // ����� ��������� ������������ �����
   int DisStep; // ����� ���������, ����� ��� ������������ ����� ���� ���������
   struct TNeuralPool* PrePool; // ������ �� "����������������" ���
   struct TNeuralPool* PostPool; // ������ �� "�����������������" ���
   struct TPoolPredConnection* next;
};

// ��� "������������ ���"
struct TNeuralPool
{
   int ID;
   int Type; // ��� ���� (0 - ���������, 1 - ������� (�����-���), 2 - ��������)
   int Capacity; // ����� ����
   double BiasMean; // ���. �������� �������� �������� ����
   double BiasVariance; // ��������� �������� �������� ����
   int Layer; // ����� ���� �������� ����
   struct TPoolConnection* ConnectednessSet; // ��������� ����������� ���� (�������)
   struct TPoolPredConnection* PredConnectednessSet; // ��������� ������������ ����������� ����
   struct TNeuralPool* next;
};

// ��� "���� �� ������������ �����"
struct TPoolNetwork
{
   struct TNeuralPool* PoolsStructure; // ������ ����� ����
   int PoolQuantity; // ���-�� ����� ����
   int ConnectionQuantity; // ���-�� ������ ����
   int PredConnectionQuantity; // ���-�� ������������ ������ ����
   int Parents[2]; // ������ ��������� - ������ ����� ���������������
   double Reward; // ������� ������, ������� ��� ������� �� ������� ������
};

// ��������� "��������� �������"
struct TAgentGenomePopulation
{
   struct TPoolNetwork* AgentGenome[MaxAgentQuantity]; // ���������
   int PopulationAgentQuantity; // ���-�� ������� � ���������
};

// ��� "������"
struct TSynapse
{
   int ID;
   double Weight; // ������������� �����������
   bool Enabled; // ������� ��������� �����
   struct TNeuron* PreNeuron; // ������ �� ���������������� ������
   struct TNeuron* PostNeuron; // ������ �� ����������������� ������
   struct TSynapse* next;
};

// ��� "������������ �����"
struct TPredConnection
{
   int ID;
   bool Enabled; // ������� ���������� ������������ �����
   struct TNeuron* PreNeuron; // ������ �� ���������������� ������
   struct TNeuron* PostNeuron; // ������ �� ����������������� ������
   struct TPredConnection* next;
};

// ��� "������"
struct TNeuron
{
   int ID;
   int Type; // ��� ������� (0 - ���������, 1 - ������� (�����-������), 2 - ��������)
   double Bias; // �������� �������
   int Layer; // ����� ���� �������
   double CurrentOut; // ������� ����� �������
   double PreviousOut; // ���������� ����� ������� (��� �������� ������) / ��������� ������� � "����������" ������
   double Potential;
   bool Active; // ������� ���������� (�������� ������ ��� ��������)
   struct TNeuralPool* ParentPool; // ������ �� ������������ ��� �������
   int ParentPoolID; // ����� ������������� ���� - ���� ��� ������
   struct TSynapse* InSynapses; // ��������� �������� �������� �������
   struct TPredConnection* PredConnections; //��������� ������������ ������ �������
   struct TNeuron* next;
};

// ��� "��������� ����"
struct TNeuralNetwork
{
   struct TNeuron* NetworkStructure; // ������ �������� ����
   int NeuronQuantity; // ���-�� �������� ����
   int SynapseQuantity; // ���-�� �������� ����
   int PredConnectionQuantity; // ���-�� ������������ ������ ����
   double Reward; // ��������� ������� ������ ����
};

// ��������� "��������� �������"
struct TAgentPopulation
{
   struct TNeuralNetwork* Agent[MaxAgentQuantity]; // ���������
   int PopulationAgentQuantity; // ���-�� ������� � ���������
};

//C�������� "��� � ����"
struct TBinAim
{
    bool Value; // ��������� �������� ����
    int Number; // ����� ���� � �������
};

//��������� "����"
struct TAim
{
    struct TBinAim AimStructure[MaxComplexity]; // ������ ���� ����� ����
    int VisitTime; // ����� ���������� ���������� ����
    double Reward; // ������� �� ���������� ����
    int Complexity; // ��������� ����
};

// ��������� "��������� ����� � �����"
struct TEnvironmentAims
{
   struct TAim Aim[MaxAimQuantity]; // ������ ����� � �����
   int AimQuantity; // ���������� ����� � �����
   int EnvironmentResolution; // ����������� �����
   int OutputResolution; // �������� ����������� ����������� ���� ������� ��������� �����
};

//!-----------------------------------------------------------

// ��������� "���������� ����������� ��������" (���� ���������� ����������� ���������, ����������� � ��������)
struct TMutationSettings
{
   double MutWeightProbability; // ����������� ������� ���� �����
   double MutWeightMeanDisp; // ��������� �������� ������� ���� �����
   double MutWeightDispDisp; // ��������� �������� ������� ��������� �����
   int DisLimit;  // ������������ ���-�� ������, ����� �������� ����������� ����� ��������� �� ������
   double EnConProb; // ����������� ��������� ����������� �����
   double DisConProb; // ����������� ���������� ���������� �����
   double InsertPoolProb; // ����������� ������� ���� (NEAT)
   double AddConnectionProb; // ����������� ������� �����
   double AddPredConnectionProb; // ����������� ������� ������������ �����
   double DeleteConnectionProb; // ����������� �������� �����
   double DeletePredConnectionProb; //  ����������� �������� ������������ �����
   double DuplicatePoolProb;  // ����������� �������������� ������������� ���� (����������������� ������������ ��������)
   double PoolDivisionCoef; // ����������� ���������� ����������� ���� ��� ��� ������� � ��������� ���������� ����
   int PoolStandartAmount; // ����������� ������ ���� � ���-�� �����(��� ������������� � ��������� ���������� �������)
   int ConnectionStandartAmount;
   int PenaltyRewardLimit; // ����������� ������ ���� (� ���-�� ���������� ������), ����� �������� ���������� ���������� ������ �� ������ (��� ������������� � ������ �� �������� ��������)
};

// ��������� "��������� ���������"
struct TTimeSettings
{
   int EvolutionTime; // ���������� ������ ��������
   int AgentLifetime; // ���������� ������ ����� ������
   int AgentPopulationQuantity; // ���������� ������� � ���������
   int RewardRecoveryTime; // ����� �������������� ������� �� ���������� ����
};

// ��������� "��������� �������"
struct TModeSettings
{
   bool NetworkMode; // ����� ������ ���������: 0 - �����������, 1 - ����������
   bool EvolutionMode; // ����� ��������: 0 - NEAT, 1 - ����������������� ��������
   bool RewardMode; // ����� ���������� �������: 0 - ��� ������, 1 - �� �������
	bool LearningMode; // ���������� ��� ������� �������� (� ������ ������ ��� ����� ����� ������ �� ���� ������������� - ������� ��� �������-�����������)

};

//��������� "����� ��������� �������"
struct TGeneralRunSettings
{
   int FirstEnvironmentNumber;
   int LastEnvironmentNumber;
   int FirstTryNumber;
   int LastTryNumber;
   int VariableNumber;
   int LastVariableNumber;
};

// ��������� "��������� ������� ����������� ��������(�������)"
struct TRunSettings
{
   int EnvironmentNumber; // ����� ����� �������
   int TryNumber; // ������ ������� �������
   int VariableNumber; // ����� ���������� �������
   int RandomizeRank; // ������������� ������������
};

// ��������� "������� ����������"
struct TDirectorySettings
{
  char EnvironmentDirectory[255]; // ���������� �� �������
  char ResultDirectory[255]; // ���������� ��� ������ ������� �� �����������
  char WorkDirectory[255]; // ������� ����������
};

// �������� "����� �������� ������" (���������� ��������� ��������)
struct TFilenameSettings
{
   char EnvironmentFilename[255]; // ���� �����
   char ResultFilename[255];  // ���� �����������
   char WorkFilename[255]; // ���� ������ ������� ����������
   char BestAgentDumpFilename[255]; // ���� ������ ������ ��������� �������
   char HistogramFilename[255];
   char BestPopulationFilename[255];
};

struct TPrimarySystemogenesisSettings
{
   int InitialPoolCapacity; // ����������� ����������� ������� ����
   double DevelopSynapseProbability; // ����������� ����������� ������� ����� � �������� ���������� ��������� ����
   double DevelopPredConnectionProbability; // ����������� ����������� ������������ ����� � �������� ���������� ��������� ����
   int PrimarySystemogenesisTime; // ����� (� ������), � ������� �������� ���������� ��������� ������������
   double SpontaneousActivityProb; // ����������� ���������� ��������� �������
   double ActiveNeuronsPercent; // ������� �������� �������� ��������, ������� ���������� ��������� ����� ��
   double SynapsesActivityTreshold; //! ����� ������ �������, ���������� �� ������� � ��, �� �������� ���������� ����� ������ ����� ��������� ��������� (��� ������� �������� ������� ���� ��������)
   double SignificanceTreshold; // ����� ���������� ������������, ��������������� �� ����. ����� � ��, �� �������� ���������� ����� ������ ����� ��������� ���������
};

struct TLearningSettings
{
   double MismatchSignificanceTreshold; // ����� ���������� ������������ ��� ����������� ������ ���������������
};

#endif // TYPES_H_INCLUDED
