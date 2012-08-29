#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

const double Empty = -13; // Признак неозначенного выхода нейрона
const int MaxComplexity = 6; // Максимальная сложность цели
const int MaxAimQuantity = 2000; // Максимальное кол-во целей в среде
const int MaxAgentQuantity = 300; // Максимальное кол-во агентов в популяции
const int MessageLenght= 200; // Стандартная длина сообщение между процессами
const int HistogramCount = 10; //Кол-во отсчетов в относительной гистограмме распределения наград
const int RunAgentNumber = 500; // Кол-во прогонов каждого агента в процессе анализа
//const int InitialPoolCapacity = 1; // Изначальная размерность каждого пула

//! - НОВЫЕ ТИПЫ ----------------

// Тип "Связь между пулами"
struct TPoolConnection
{
   int ID;
   double WeightMean; // Мат. ожидание веса синапса
   double WeightVariance; // Дисперсия веса синапса
   bool Enabled; // Признак экспресии связи (включен/выключен)
   double InnNumber; // Номер инновации связи
   int DisStep; // Номер популяции, когда эта связь была выключена
   struct TNeuralPool* PrePool; // Ссылка на "пресинаптический" пул
   struct TNeuralPool* PostPool; // Ссылка на "постсинаптический" пул
   struct TPoolConnection* next;
};

// Тип "Предикторная связь между пулами"
struct TPoolPredConnection
{
   int ID;
   bool Enabled; // Признак экспресии предикторной связи
   double InnNumber; // Номер инновации предикторной связи
   int DisStep; // Номер популяции, когда эта предикторная связь была выключена
   struct TNeuralPool* PrePool; // Ссылка на "пресинаптический" пул
   struct TNeuralPool* PostPool; // Ссылка на "постсинаптический" пул
   struct TPoolPredConnection* next;
};

// Тип "Нейрональный пул"
struct TNeuralPool
{
   int ID;
   int Type; // Тип пула (0 - сенсорный, 1 - скрытый (интер-пул), 2 - выходной)
   int Capacity; // Объем пула
   double BiasMean; // Мат. ожидание смещения нейронов пула
   double BiasVariance; // Дисперсия смещения нейронов пула
   int Layer; // Номер слоя нейронов пула
   struct TPoolConnection* ConnectednessSet; // Множество связанности пула (входное)
   struct TPoolPredConnection* PredConnectednessSet; // Множество предикторной связанности пула
   struct TNeuralPool* next;
};

// Тип "Сеть из нейрональных пулов"
struct TPoolNetwork
{
   struct TNeuralPool* PoolsStructure; // Список пулов сети
   int PoolQuantity; // Кол-во пулов сети
   int ConnectionQuantity; // Кол-во связей сети
   int PredConnectionQuantity; // Кол-во предикторных связей сети
   int Parents[2]; // Номера родителей - первый более приспособленный
   double Reward; // Награда агента, который был получен из данного генома
};

// Структура "Популяция агентов"
struct TAgentGenomePopulation
{
   struct TPoolNetwork* AgentGenome[MaxAgentQuantity]; // Популяция
   int PopulationAgentQuantity; // Кол-во агентов в популяции
};

// Тип "Синапс"
struct TSynapse
{
   int ID;
   double Weight; // Синаптический коэффициент
   bool Enabled; // Признак экспресии связи
   struct TNeuron* PreNeuron; // Ссылка на пресинаптический нейрон
   struct TNeuron* PostNeuron; // Ссылка на постсинаптический нейрон
   struct TSynapse* next;
};

// Тип "Предикторная связь"
struct TPredConnection
{
   int ID;
   bool Enabled; // Признак эксперссии предикторной связи
   struct TNeuron* PreNeuron; // Ссылка на пресинаптический нейрон
   struct TNeuron* PostNeuron; // Ссылка на постсинаптический нейрон
   struct TPredConnection* next;
};

// Тип "Нейрон"
struct TNeuron
{
   int ID;
   int Type; // Тип нейрона (0 - сенсорный, 1 - скрытый (интер-нейрон), 2 - выходной)
   double Bias; // Смещение нейрона
   int Layer; // Номер слоя нейрона
   double CurrentOut; // Текущий выход нейрона
   double PreviousOut; // Предыдущий выход нейрона (для обратных связей) / потенциал нейрона в "синхронном" режиме
   double Potential;
   bool Active; // Признак активности (активный нейрон или молчащий)
   struct TNeuralPool* ParentPool; // Ссылка на родительский пул нейрона
   int ParentPoolID; // Номер родительского пула - если нет ссылки
   struct TSynapse* InSynapses; // Множество входящих синапсов нейрона
   struct TPredConnection* PredConnections; //Множество предикторных связей нейрона
   struct TNeuron* next;
};

// Тип "Нейронная сеть"
struct TNeuralNetwork
{
   struct TNeuron* NetworkStructure; // Список нейронов сети
   int NeuronQuantity; // Кол-во нейронов сети
   int SynapseQuantity; // Кол-во синапсов сети
   int PredConnectionQuantity; // Кол-во предикторных связей сети
   double Reward; // Набранная награда данной сети
};

// Структура "Популяция агентов"
struct TAgentPopulation
{
   struct TNeuralNetwork* Agent[MaxAgentQuantity]; // Популяция
   int PopulationAgentQuantity; // Кол-во агентов в популяции
};

//Cтруктура "Бит в цели"
struct TBinAim
{
    bool Value; // Требуемое значение бита
    int Number; // Номер бита в векторе
};

//Структура "Цель"
struct TAim
{
    struct TBinAim AimStructure[MaxComplexity]; // Массив всех битов цели
    int VisitTime; // Время последнего достижения цели
    double Reward; // Награда за достижение цели
    int Complexity; // Сложность цели
};

// Структура "Структура целей в среде"
struct TEnvironmentAims
{
   struct TAim Aim[MaxAimQuantity]; // Массив целей в среде
   int AimQuantity; // Количество целей в среде
   int EnvironmentResolution; // Размерность среды
   int OutputResolution; // Выходная размерность кодирования бита вектора состояния среды
};

//!-----------------------------------------------------------

// Структура "Найстройки мутационных процедур" (сюда включаются необходимые константы, относящиеся к мутациям)
struct TMutationSettings
{
   double MutWeightProbability; // Вероятность мутации веса связи
   double MutWeightMeanDisp; // Дисперсия значения мутации веса связи
   double MutWeightDispDisp; // Дисперсия значения мутации дисперсии связи
   int DisLimit;  // Максимальное кол-во тактов, после которого выключенная связь удаляется из генома
   double EnConProb; // Вероятность включения выключенной связи
   double DisConProb; // Вероятность выключения включенной связи
   double InsertPoolProb; // Вероятность вставки пула (NEAT)
   double AddConnectionProb; // Вероятность вставки связи
   double AddPredConnectionProb; // Вероятность вставки предикторной связи
   double DeleteConnectionProb; // Вероятность удаления связи
   double DeletePredConnectionProb; //  Вероятность удаления предикторной связи
   double DuplicatePoolProb;  // Вероятность дуплицирования нейронального пула (модернизированный эволюционный алгоритм)
   double PoolDivisionCoef; // Коэффициент уменьшения размерности пула при его деления в процедуре дупликации пула
   int PoolStandartAmount; // Стандартный размер сети в кол-ве пулов(для использования в процедуре дупликации нейрона)
   int ConnectionStandartAmount;
   int PenaltyRewardLimit; // Стандартный размер сети (в кол-ве включенных связей), после которого начинается начисление штрафа за размер (для использования в режиме со штрафной функцией)
};

// Структура "Временные параметры"
struct TTimeSettings
{
   int EvolutionTime; // Количество тактов эволюции
   int AgentLifetime; // Количество тактов жизни агента
   int AgentPopulationQuantity; // Количество агентов в популяции
   int RewardRecoveryTime; // Время восстановления награды за достижения цели
};

// Структура "Параметры режимов"
struct TModeSettings
{
   bool NetworkMode; // Режим работы нейросети: 0 - стандартный, 1 - синхронный
   bool EvolutionMode; // Режим эволюции: 0 - NEAT, 1 - модернизированный алгоритм
   bool RewardMode; // Режим начисления награды: 0 - без штрафа, 1 - со штрафом
	bool LearningMode; // Отсутствие или наличие обучения (в данной версии это также будет влиять на типа системогенеза - обычный или линейно-однозначный)

};

//Структура "Общие параметры запуска"
struct TGeneralRunSettings
{
   int FirstEnvironmentNumber;
   int LastEnvironmentNumber;
   int FirstTryNumber;
   int LastTryNumber;
   int VariableNumber;
   int LastVariableNumber;
};

// Структура "Параметры запуска конкретного процесса(прогона)"
struct TRunSettings
{
   int EnvironmentNumber; // Номер среды прогона
   int TryNumber; // Первый попытки прогона
   int VariableNumber; // Номер параметров прогона
   int RandomizeRank; // Инициализация рандомизации
};

// Структура "Рабочие директории"
struct TDirectorySettings
{
  char EnvironmentDirectory[255]; // Директория со средами
  char ResultDirectory[255]; // Директория для записи отчетов по результатам
  char WorkDirectory[255]; // Рабочая директория
};

// Стуктура "Имена отчетных файлов" (передается процедуре эволюции)
struct TFilenameSettings
{
   char EnvironmentFilename[255]; // Файл среды
   char ResultFilename[255];  // Файл результатов
   char WorkFilename[255]; // Файл записи рабочей информации
   char BestAgentDumpFilename[255]; // Файл записи лучшей популяции агентов
   char HistogramFilename[255];
   char BestPopulationFilename[255];
};

struct TPrimarySystemogenesisSettings
{
   int InitialPoolCapacity; // Изначальная размерность каждого пула
   double DevelopSynapseProbability; // Вероятность образования синапса связи в процессе построения первичной сети
   double DevelopPredConnectionProbability; // Вероятность образования предикторной связи в процессе построения первичной сети
   int PrimarySystemogenesisTime; // Время (в тактах), в течение которого происходит первичный системогенез
   double SpontaneousActivityProb; // Вероятность спонтанной активации нейрона
   double ActiveNeuronsPercent; // Процент наиболее активных нейронов, которые становятся активными после ПС
   double SynapsesActivityTreshold; //! Порог общего сигнала, прошедшего по синапсу в ПС, по которому происходит отбор связей между активными нейронами (или процент синапсов которые надо отобрать)
   double SignificanceTreshold; // Порог значимости предсказания, осуществляемого по пред. связи в ПС, по которому происходит отбор связей между активными нейронами
};

struct TLearningSettings
{
   double MismatchSignificanceTreshold; // Порог значимости предсказания для определения момент рассогласования
};

#endif // TYPES_H_INCLUDED
