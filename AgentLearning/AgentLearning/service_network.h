#ifndef SERVICE_NETWORK_H_INCLUDED
#define SERVICE_NETWORK_H_INCLUDED

#include "stdio.h"

//! Конструкторы всех элементов, а также некоторые дескруторы

// Функция создания пустой сети из пулов
TPoolNetwork* CreatePoolNetwork();

// Удаление сети из пулов
TPoolNetwork* DeletePoolNetwork(TPoolNetwork* PoolNetwork);

// Функция создания и заполнения новой связи между пулами
TPoolConnection* CreatePoolConnection(int ID, double WeightMean, double WeightVariance, bool Enabled, double InnNumber, int DisStep, double DevelopSynapseProb, TNeuralPool* PrePool, TNeuralPool* PostPool, TPoolConnection* next = NULL);

// Функция создание и заполнения предикторной связи между пулами
TPoolPredConnection* CreatePoolPredConnection(int ID, bool Enabled, double InnNumber, int DisStep, double DevelopPredConProb, TNeuralPool* PrePool, TNeuralPool* PostPool, TPoolPredConnection* next = NULL);

// Функция создания и заполнения нового нейронального пула
TNeuralPool* CreateNeuralPool(int ID, int Type, int Capacity, double BiasMean, double BiasVariance, int Layer, TPoolConnection* ConnectednessSet = NULL, TPoolPredConnection* PredConnectednessSet = NULL, TNeuralPool* next = NULL);

// Функция создания пустой нейронной сети
TNeuralNetwork* CreateNeuralNetwork();

// Удаление нейронной сети
TNeuralNetwork* DeleteNeuralNetwork(TNeuralNetwork* NeuralNetwork);

// Функция создания и заполнения нового синапса
TSynapse* CreateSynapse(int ID, double Weight, bool Enabled, TNeuron* PreNeuron, TNeuron* PostNeuron, TSynapse* next = NULL);

// Функция создание и заполнения предикторной связи
TPredConnection* CreatePredConnection(int ID, bool Enabled, TNeuron* PreNeuron, TNeuron* PostNeuron, TPredConnection* next = NULL);

// Функция создания и заполнения нового нейрона
TNeuron* CreateNeuron(int ID, int Type, double Bias, int Layer, bool Active, TNeuralPool* ParentPool, int ParentPoolID, TSynapse* InSynapses = NULL, TPredConnection* PredConnections = NULL, TNeuron* next = NULL);

//! Фукнции получения ссылок на элементы сети
// Функция получения ссылки на пул с соответствующим ID
TNeuralPool* GetPointer2Pool(TPoolNetwork* PoolNetwork, int PoolID);

TPoolConnection* GetPointer2PoolConnection(TPoolNetwork* PoolNetwork, int PoolConnectionID);

TPoolConnection* GetPointer2PoolConnection(TPoolNetwork* PoolNetwork, int PrePoolID, int PostPoolID);

TPoolPredConnection* GetPointer2PoolPredConnection(TPoolNetwork* PoolNetwork, int PoolPredConnectionID);

TPoolPredConnection* GetPointer2PoolPredConnection(TPoolNetwork* PoolNetwork, int PrePoolID, int PostPoolID);

TNeuron* GetPointer2Neuron(TNeuralNetwork* Network, int NeuronID);

TSynapse* GetPointer2Synapse(TNeuralNetwork* Network, int SynapseID);

TPredConnection* GetPointer2PredConnection(TNeuralNetwork* Network, int PredConnectionID);

//! Служебные функции работы с сетью
// Процедура стирания нейронной сети
void EraseNeuralNetwork(TNeuralNetwork* NeuralNetwork);

// Процедура стирания сети из пулов
void ErasePoolNetwork(TPoolNetwork* PoolNetwork);

// Процедура копирования одной сети в другую
void CopyPoolNetwork(TPoolNetwork* NewPoolNetwork, TPoolNetwork* ParentPoolNetwork);

// Процедура копирования одной сети в другую
void CopyNeuralNetwork(TNeuralNetwork* NewNetwork, TNeuralNetwork* ParentNetwork);

// Обнуление внутреннего состояния сети
void ResetNeuralNetwork(TNeuralNetwork* NeuralNetwork);

// Процедура исправления ID нейронов (в случае если нейроны были удалены)
void fix_neurons_id(TNeuralNetwork* neural_network);

// Процедура исправления ID синапсов (в случае если синапсы были удалены)
void fix_synapses_id(TNeuralNetwork* neural_network);

// Процедура исправления ID предикторных связей (в случае если связи были удалены)
void fix_pred_connections_id(TNeuralNetwork* neural_network);

//! Функции активности сети
//Рекурентная процедура моделирования нейронной сети - определение выхода запрашиваемого нейрона (для одновременного режима)
double NeuronOutput(TNeuron* Neuron, int* EnvironmentVector, bool RecurrentMode);

// Процедура прогона нейронной сети и определения ее выхода (для одновременного режима работы)
void NetworkOutput_standart(TNeuralNetwork* Network, int* InputVector, double* OutputVector);

// Процедура прогона нейронной сети и определения ее выхода (для синхронного режима работы)
void NetworkOutput_synch(TNeuralNetwork* Network, int* InputVector, double* OutputVector);

//Рекурентная процедура определения выхода нейрона с возможностью спонтанной активации
double NeuronOutput_spontan(TNeuron* Neuron, bool RecurrentMode, double SpontaneousActivityProb);

// Процедура прогона нейронной сети для процедуры первичного системогенеза
void NetworkOutput_primarysys(TNeuralNetwork* PrimaryNetwork, double SpontaneousActivityProb);

//! Функции записи и считывания сетей из файлов
//Процедура заполнения нейронной сети из файла
void FillNextNeuralNetwork(TNeuralNetwork* NeuralNetwork, FILE* hNetworkFile, int EnvironmentResolution, int OutputResolution, int NetworkMode);

// Пропуск записи нейронной сети в файле
void SkipNeuralNetworkInFile(FILE* hNetworkFile, int NetworkMode);

// Загрузка конкретной нейронной сети (по номеру) из файла
void FillSpecificNeuralNetwork(TNeuralNetwork* NeuralNetwork, char* NetworkFilename, int NetworkNumber, int EnvironmentResolution, int OutputResolution, int NetworkMode);

//Процедура заполнения сети из пулов из файла
void FillNextPoolNetwork(TPoolNetwork* PoolNetwork, FILE* hNetworkFile, int EnvironmentResolution, int OutputResolution, int NetworkMode);

// Пропуск записи сети из пулов в файле
void SkipPoolNetworkInFile(FILE* hNetworkFile, int NetworkMode);

// Загрузка конкретной сети из пулов (по номеру) из файла
void FillSpecificPoolNetwork(TPoolNetwork* PoolNetwork, char* NetworkFilename, int NetworkNumber, int EnvironmentResolution, int OutputResolution, int NetworkMode);

// Процедура записи одной сети в файл
void RecordNeuralNetwork(TNeuralNetwork* NeuralNetwork, int NetworkNumber, FILE* hNetworkFile);

// Процедура записи одной сети из пулов в файл
void RecordPoolNetwork(TPoolNetwork* PoolNetwork, int NetworkNumber, FILE* hNetworkFile);

//! Визуализация сетей
// Преобразование нейронной сети в файл .dot для дальнейшей визуализации в виде графа
void NeuralNetwork2Dot(TNeuralNetwork* NeuralNetwork, const char NetworkFigFile[]);

// Преобразование нейронной сети в файл .dot для дальнейшей визуализации в виде графа, но без слоев
void NeuralNetwork2UnLayeredDot(TNeuralNetwork* NeuralNetwork, const char NetworkFigFile[]);

// Преобразование сети из пулов в файл .dot для дальнейшей визуализации в виде графа
void PoolNetwork2Dot(TPoolNetwork* PoolNetwork, const char NetworkFigFile[]);

#endif // SERVICE_NETWORK_H_INCLUDED
