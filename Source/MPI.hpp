#define ENABLE_MPI
#define SEED 0

// enable or disable randomization
#ifdef ENABLE_RANDOM
#define SET_SEED() srand((unsigned int)time(NULL))
#else
#define SET_SEED() srand(SEED)
#endif

#ifdef ENABLE_MPI

#include<mpi.h>

// new mpi datatype
struct s_update_weights {
    int selected_action;
    double dw;
};

extern MPI_Datatype mpi_update_weights_type;

void init_update_weights_type ();

// mpi variables
void set_mpi_vars (int mpi_rank, int mpi_num_proc);

int get_mpi_rank ();
int get_mpi_num_proc ();

void cleanup_mpi ();
#endif
