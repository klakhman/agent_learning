#ifndef MUTATION_H_INCLUDED
#define MUTATION_H_INCLUDED

// Процедура мутации - удаления из агента связей, которые выключены более некоторого количества поколений
void MutationDeleteConPop(TPoolNetwork* KidPoolNetwork, int EvolutionStep, int DisLimit);

// Процедура мутации - удаления из агента предикторных связей, которые выключены более некоторого количества поколений
void MutationDeletePredConPop(TPoolNetwork* KidPoolNetwork, int EvolutionStep, int DisLimit);

// Процедура мутации - мутация весовых коэффициентов связи
void MutationConWeight(TPoolNetwork* KidPoolNetwork, double MutWeightProbability, double MutWeightMeanDisp, double MutWeightDispDisp);

// Процедура мутации - включения/выключения связей
void MutationEnDisConnection(TPoolNetwork* KidPoolNetwork, double EnConProb, double DisConProb, int EvolutionStep);

// Процедура мутации - включения/выключения предикторных связей
void MutationEnDisPredConnection(TPoolNetwork* KidPoolNetwork, double EnConProb, double DisConProb, int EvolutionStep);

// Процедура мутации - добавление связи
void MutationAddConnection(TPoolNetwork* KidPoolNetwork, double AddConnectionProb, bool EvolutionMode, double* CurrentInnovationNumber);

// Процедура мутации - добавление предикторной связи связи
void MutationAddPredConnection(TPoolNetwork* KidPoolNetwork, double AddPredConnectionProb, double* CurrentPredInnovationNumber);

// Процедура мутации - удаление связи
void MutationDeletePoolConnection(TPoolNetwork* KidNetwork, double DeleteConnectionProb);

// Процедура мутации - удаление пред. связи
void MutationDeletePoolPredConnection(TPoolNetwork* KidNetwork, double DeletePredConnectionProb);

// Процедура мутации - вставка пула (для NEAT, при этом невозможен первичный системогенез и обучение)
void MutationInsertPool(TPoolNetwork* KidPoolNetwork, double InsertPoolProb, int EvolutionStep, double* CurrentInnovationNumber);

// Множитель, который используется в процедуре мутации - дупликация пула, чтобы уменьшить вероятность дупликации с ростом сети
double F_duplicate(int PoolQuantity, int ConnectionQuantity, int PoolStandartAmount, int ConnectionStandartAmount);

// Процедура мутации - дупликация пула
void MutationPoolDuplicate(TPoolNetwork* KidPoolNetwork, double DuplicatePoolProb, double PoolDivisionCoefficient, int PoolStandartAmount, int ConnectionStandartAmount, double* CurrentInnovationNumber, double* CurrentPredInnovationNumber);

// Процедура генерации потомка от двух родителей
void ComposeOffspringFromParents(TPoolNetwork* KidPoolNetwork, TPoolNetwork* FirstParentNetwork, TPoolNetwork* SecondParentNetwork);

// Процедура генерации одного потомка
void GenerateOffspring(TPoolNetwork* KidPoolNetwork, TPoolNetwork* FirstParentNetwork, TPoolNetwork* SecondParentNetwork, int EvolutionStep, TMutationSettings* Settings, bool EvolutionMode, double* CurrentInnovationNumber, double* CurrentPredInnovationNumber);

//Процедура получения номера агента, используемая в рулеточном алгоритме
int RouletteWheelSelection(double* PopulationFitness, int PopulationQuantity);

// Штраф из награды за размер генома агента
double RewardPenalty(TPoolNetwork* PoolNetwork, int PenaltyRewardLimit);

// Процедура генерации нового поколения и замены старого на новое
void GenerateNextPopulation(TAgentGenomePopulation* AgentGenomePopulation, TMutationSettings* MutationSettings, int EvolutionStep, bool EvolutionMode, bool RewardMode, double* CurrentInnovationNumber, double* CurrentPredInnovationNumber);

#endif // MUTATION_H_INCLUDED

