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
    int i = get_global_id(0);               
    int j = get_global_id(1);               
    int oldWeight = A[i*V + j];
    int currentWeight = A[k*V + j];
    if (oldWeight > currentWeight) {
        A[i*V + j] = currentWeight;
    }
}
