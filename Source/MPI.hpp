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

const int BCAST_EXIT = -1;      // call MPI_Finalize()
const int BCAST_NUM_GAMES = 0;  // send num games
const int BCAST_WEIGHTS = 1;    // send weights for Action_Update_Weights()
const int BCAST_PARAM_VALUES = 2;

// new mpi datatype; command defines action and 2nd and 3rd parameter, refer to
// the table below
struct s_update_params {
    int command;         // BCAST_EXIT || BCAST_NUM_GAMES || BCAST_WEIGHTS
    int selected_action; // undefined  || num_games       || selected_action
    double dw;           // undefined  || undefined       || dw
};

extern MPI_Datatype mpi_update_params_type;

void init_update_params_type ();

// mpi variables
void set_mpi_vars (int mpi_rank, int mpi_num_proc);

int get_mpi_rank ();
int get_mpi_num_proc ();

void cleanup_mpi ();
#endif
