#ifndef ENVIRONMENT_H_INCLUDED
#define ENVIRONMENT_H_INCLUDED

// Процедура загрузки среды из файла
void InitEnvironment(TEnvironmentAims* EnvironmentAims, char* EnvironmentFilename);

//Процедура перезапуска среды
void ResetEnviroment(TEnvironmentAims* EnvironmentAims);

// Процедура вычисления коэффициента заполненности среды
double CoefFullEnvironment(TEnvironmentAims* EnvironmentAims);

// Процедура генерации срелы
double GenerateEnvironment(TEnvironmentAims* EnvironmentAims, int EnvironmentResolution, double RequiredEnvCoefFull, double Eps, int MaxComplexity, int MinMaxComplexity, int MinComplexity);

void WriteEnvironmentToFile(TEnvironmentAims* EnvironmentAims, char* EnvironmentFilename);

// Генерация пула сред
void GenerateEnvironmentsPool(double StartCoefficient, double EndCoefficient, double Step, int ReplyEnvQuanity, int FirstEnvironmentID, char* EnvironmentDirectory, int EnvironmentResolution, double Eps, int MaxComplexity, int MinMaxComplexity, int MinComplexity);


#endif // ENVIRONMENT_H_INCLUDED
