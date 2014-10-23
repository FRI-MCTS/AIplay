#define ENABLE_MPI
#define SEED 0

// enable or disable randomization
#ifdef ENABLE_RANDOM
#define SET_SEED() srand((unsigned int)time(NULL))
#else
#define SET_SEED() srand(SEED)
#endif

void set_mpi_vars (int mpi_rank, int mpi_num_proc);

int get_mpi_rank ();
int get_mpi_num_proc ();
