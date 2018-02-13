//------------------------------------------------------------------------------
//
// kernel:  floyd_warshall  
//
// Purpose: Compute one distance IJ = min(IJ, IK + KJ)
// 
// input: A : distance matrix, numNodes : V, k : pass number
//
// output: A modified
//

__kernel void floyd_warshall(__global unsigned short* A, const unsigned int V, const unsigned int k)
{                                          
    int id = get_global_id(0);               
    if(id < V*V) {
        int oldWeight = A[id];
        int currentWeight = A[k*V + id%V] + A[(id/V)*V + k];
        if (oldWeight > currentWeight) {
            A[id] = currentWeight;
        }
    }
}
