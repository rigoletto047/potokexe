#ifndef dsp_param_h
#define dsp_param_h
#define L_LEN 2048
#define M_LEN 512
#define R 2
#define L 4
typedef struct
{
// Дальность действия
    unsigned int    Daln_HM;      // Дальность ЧМ в метрах
    unsigned int    Daln_MONO;    // Дальность МОНО в метрах
// Параметры ДДС
    unsigned int    aOPENMODE;
    unsigned int    aGEN;       // 1-внешний генератор, 0-внутренний
    unsigned int    aMCLOCK;    // Частота внешнего генератора в герцах
    unsigned int    aDECIMATION;// Децимация в ДДС
    unsigned int    aCHAN;      // Маска каналов ДДС (0x00000000- все каналы выключены, 0xffffffff-все включены)
    int	            aFc[16];    // Частота опорного генератора каждого канала ДДС в герцах
    int	            aInput[16]; // Номер, соединенный с номером канала (aInput[1]=3 - 1-й АЦП соединен с 3 каналом ДДС)

  	unsigned int	aSYNC_MODE; // Тип синхронизации (1 - по импульсам запуска, 0 - программный старт)
	unsigned int	aPGA;
	unsigned int	aRAND;
	unsigned int	aDITH;
	unsigned int	aADC16;
	unsigned int	aBIT24;

	int             aCADR_ENABLE;
	int             aCADR_RESTART;
	int             aCADR_MSTART;
	int             aCADR_INVSTART;
	int             aCADR_CNT0;
	int             aCADR_CNT1;
	int             aCADR_CNT2;
	int             aCADR_SIZE;
	int             aCADR_DATA;


  	unsigned int	aSM_MSTART; // 2 - старт по импульсам запуска, 0 - программный старт)
	unsigned int	aSM_INVSTART;
	unsigned int    aSM_MSTOP;
	unsigned int	aSM_INVSTOP;
	unsigned int	aSM_TRSTART;
	float           aSM_THDAC1;
	float           aSM_THDAC2; // Порог компаратора в вольтах

    unsigned int    items;		// Размер программы ДДС
// Параметры
    unsigned int    Nomer_komplekta ;   // Номер комплекта
    unsigned int    f_proz ;            // Частота процессора
    unsigned int    zad_lhm ;           // Задержка нуля дальности ЧМ
    unsigned int    sizeHM ;            // Число данных ЧМ, соответствующее требуемой дальности+задержка
    unsigned int    zad_mono ;          // Задержка нуля дальности МОНО
    unsigned int    sizeMONO;           // Число данных МОНО, соответствующее требуемой дальности+задержка
    unsigned int    size_HM ;            // Число данных ЧМ, соответствующее требуемой дальности
    unsigned int    size_MONO;           // Число данных МОНО, соответствующее требуемой дальности
    unsigned int    f_getHM ;           // Частота гетеродина ЧМ
    unsigned int    f_getMONO ;         // Частота гетеродина МОНО
    unsigned int 	float_to_int;		// Множитель преобразования чисел float в int
// Параметры межпериодной обработки и обнаружения
    float    		porog;              // Порог, при превышении которого данные записываются в карту помех
    unsigned int    NR ;                // Размер карты помех ЧМ
    unsigned int    NR_M;               // Размер карты помех МОНО
    float           PL_K_Phm;            // Константа регуляризации ЧМ
    float           PL_K_Pmono;          // Константа регуляризации МОНО
    unsigned int    per_lhm ;           // Число периодов ЧМ
    unsigned int    per_mono;           // Число периодов МОНО
    float           beta;               // Коэффициент сглаживания карты помех ЧМ
    float           beta_MONO ;         // Коэффициент сглаживания карты помех МОНО
    float           PorogL;             // Порог обнаружения
    float			con;				// Константа, при превышении которой входной сигнал ЧМ
    									// нормируется: s*=sq_con
    float			sq_con;				// Константа, на которую умножается ЧМ	
    float			con_m;				// Константа, при превышении которой входной сигнал МОНО
    									// нормируется: s*=sq_conm
    float			sq_con_m;			// Константа, на которую умножается МОНО
    unsigned int	d_Wary1;			// Дальность действия ВАРУ ЧМ НУ
    unsigned int	wid1;				// Вид кривой ВАРУ НУ (0-канал отключен, 1-линейный,2-квадратичный)
    unsigned int	d_Wary2;			// Дальность действия ВАРУ ЧМ ВУ
    unsigned int	wid2;				// Вид кривой ВАРУ ВУ (0-канал отключен, 1-линейный,2-квадратичный)
    unsigned int	d_Wary1m;			// Дальность действия ВАРУ МОНО НУ
    unsigned int	wid1m;				// Вид кривой ВАРУ НУ (0-канал отключен, 1-линейный,2-квадратичный)
    unsigned int	d_Wary2m;			// Дальность действия ВАРУ МОНО ВУ
    unsigned int	wid2m;				// Вид кривой ВАРУ ВУ (0-канал отключен, 1-линейный,2-квадратичный)
    float           us0hm1;             // Усиление в 0 ВАРУ ЧМ 1 канала
    float           us0hm2;             // Усиление в 0 ВАРУ ЧМ 2 канала
    float           us0mono1;             // Усиление в 0 ВАРУ МОНО 1 канала
    float           us0mono2;             // Усиление в 0 ВАРУ МОНО 2 канала
// Режимы работы
    unsigned int    CJATIE;             // Сжатие
    unsigned int    Nakoplenie;         // Режим накопления включен
    unsigned int    meteo;              // Режим метео включен
    unsigned int    MTD;                // Режим MTD включен
    unsigned int    NU;                 // Нижние углы включены
    unsigned int    VU;                 // Верхние углы включены
    unsigned int	HM;					// Разрешена обработка МТД ЧМ на всей дистанции (1) или только после МОНО (0)
    unsigned int    nak;                // Количество обзоров для включения режима накопления карты помех
    unsigned int    otladka;            // 1- режим отладки, вывод полной информации в формате Buf_HM_MONO
    unsigned int    median;             // 1- включена медианная фильтрация 0 - выключена
    unsigned int    nom_hm;             // Номер ЧМ для отображения тестовых данных
	unsigned int    nom_mono;           // Номер MONO для отображения тестовых данных
	float 			energij;			// Превышение энергии ЧМ импульса к МОНО
	int				kol_oz;				// Через сколько зондирований оценки выдаются потребителю 
    unsigned int    WKL_HM;             // ЧМ включено
    unsigned int    WKL_MONO;           // МОНО включено
    unsigned int    tip_filt;			// Тип фильтра ДН. 0-прямоугольный, 1- трапеция, 2 -мах разрешение
    float			da;					// Ширина ДН в град
    float			tobs;				// Время обзора в с
    // Имитация двух радиально движущихся целей
    int				DALN;				// Угол начала ДН 1-й цели (в МАИ)
	int 			Das;				// Приращение угла 2-й цели относительно 1-й (в МАИ)
	int 			Ddal;				// Приращение по дальности 2-й цели относительно 1-й в дискретах
	float 			amplituda;			// Амплитуда цели 1
	float 			amplituda2;			// Амплитуда цели 2
	int 			DMax;				// Дальность начального положения целей в дискретах
	int 			DELTAD;				// Приращение дальности при движении целей
	int 			DELTAD2;			// Приращение дальности при движении целей
	int 			N_Buf_Oz;			// Длина буфера для вычисления оценок координат
	int				fd;					// Частота Доплера 1-й цели
	int				fd2;				// Частота Доплера 2-й цели
	int				md;					// Минимальная дальность
	
	//unsigned int    aDECIMONO;// Децимация в ДДС MONO

}PARAM_DSP;

#endif
