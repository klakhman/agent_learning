#ifndef AGENTLIFE_H_INCLUDED
#define AGENTLIFE_H_INCLUDED
#include <stdio.h>

//Процедура определения общей награды, полученной агентом в течение жизни
double CalcAgentReward(int* LifeRecord, int AgentLifetime, TEnvironmentAims* EnvironmentAims, int RewardRecoveryTime);

//Процедура действия агента в один такт времени
void AgentStep(TNeuralNetwork* AgentNetwork, int* EnvironmentVector, TEnvironmentAims* EnvironmentAims, int* LifeRecord, int AgentLifeStep, bool NetworkMode);

//Процедура моделирования жизни агента
void AgentLife(TNeuralNetwork* AgentNetwork, TEnvironmentAims* EnvironmentAims, int RewardRecoveryTime, double EnVarProb, int AgentLifetime, bool NetworkMode = false, bool LearningMode = false, TLearningSettings* LearningSettings = NULL);

// Отображение жизни агента в текстовом режиме
void AgentLifeView(TNeuralNetwork* AgentNetwork, TEnvironmentAims*  EnvironmentAims, int RewardRecoveryTime, double EnVarProb, int AgentLifetime, bool NetworkMode);

#endif // AGENTLIFE_H_INCLUDED
