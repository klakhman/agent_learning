#ifndef PRIMARY_SYSTEMOGENESIS_H_INCLUDED
#define PRIMARY_SYSTEMOGENESIS_H_INCLUDED

// Построение ПОЛНОЙ (без отбора) первичной нейронной сети из генотипа (структуры пулов)
void BuildPrimaryNetwork(TNeuralNetwork* PrimaryNetwork, TPoolNetwork* PoolNetwork);

// Функция отбора активирующихся нейронов
void NeuronsSelection(TNeuralNetwork* PrimaryNetwork, double* InitialSummaryPotential, int ActivationNeuronsQuantity);

// Функция отбора наиболее активных синапсов
void SynapsesSelection(TNeuralNetwork* PrimaryNetwork, double* SummaryPotential, double SynapsesActivityTreshold);

// Функция отбора предикторных связей
void PredConnectionSelection(TNeuralNetwork* PrimaryNetwork, double* PredictorSignificance, double SignificanceTreshold);

// Процедура проведения первичного системогенеза агента
void AgentPrimarySystemogenesis(TNeuralNetwork* PrimaryNetwork, TPoolNetwork* PoolNetwork, TPrimarySystemogenesisSettings* PrimarySystemogenesisSettings);

// Процедура линейного системогенеза, которая преобразует сеть из пулов в сеть из нейронов, как будто это одно и тоже (эволюция нейронных сетей, а не пулов)
void AgentLinearPrimarySystemogenesis(TNeuralNetwork* PrimaryNetwork, TPoolNetwork* PoolNetwork);

#endif // PRIMARY_SYSTEMOGENESIS_H_INCLUDED
