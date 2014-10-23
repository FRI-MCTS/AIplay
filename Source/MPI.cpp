#include "MPI.hpp"

static int mpi_rank, mpi_num_proc;

void set_mpi_vars (int rank, int num_proc) {
    mpi_rank = rank;
    mpi_num_proc = num_proc;
}

int get_mpi_rank () {
    return mpi_rank;
}

int get_mpi_num_proc () {
    return mpi_num_proc;
}
