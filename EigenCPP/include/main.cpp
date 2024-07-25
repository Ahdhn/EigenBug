#include "Eigen/OrderingMethods"
#include "Eigen/SparseCholesky"

#include <iostream>

int main(int argc, char** argv)
{
    int n = 10;

    Eigen::SparseMatrix<float> A(n, n);

    std::vector<Eigen::Triplet<float>> triplets;
    for (uint32_t v = 0; v < n; ++v) {
        triplets.push_back(Eigen::Triplet<float>(v, v, 0.1f));
    }

    A.setFromTriplets(triplets.begin(), triplets.end());

    Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>,
                          Eigen::UpLoType::Lower,
                          Eigen::AMDOrdering<int>>
        solver;

    solver.compute(A);

    Eigen::VectorXf x(n);


    Eigen::VectorXf b(n);
    b.setOnes();

    x = solver.solve(b);

    std::cout << x << "\n";

    return 0;
}
