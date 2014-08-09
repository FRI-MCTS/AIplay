#define ENABLE_MPI
#define SEED 0

// enable or disable randomization
#ifdef ENABLE_RANDOM
#define SET_SEED; srand((unsigned int)time(NULL));
#else
#define SET_SEED() srand(SEED)
#endif

