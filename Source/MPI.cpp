#include "MPI.hpp"
#include <stddef.h>

#ifdef ENABLE_MPI
static int mpi_rank, mpi_num_proc;

MPI_Datatype mpi_update_weights_type;

void init_update_weights_type () {
    int num_items = 2;
    int blocklengths[2] = {1,1};
    MPI_Datatype types[2] = {MPI_INT, MPI_DOUBLE};
    MPI_Aint offsets[2];

    offsets[0] = offsetof(s_update_weights, selected_action);
    offsets[1] = offsetof(s_update_weights, dw);

    MPI_Type_create_struct(num_items, blocklengths, offsets, types, &mpi_update_weights_type);
    MPI_Type_commit(&mpi_update_weights_type);
}

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

void cleanup_mpi () {
    MPI_Type_free (&mpi_update_weights_type);
    MPI_Finalize();
}
#endif
