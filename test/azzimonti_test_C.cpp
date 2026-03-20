#include<fdaPDE/fdapde.h>
#include<utils.h>
using namespace fdapde;

int main(int argc, char *argv[]){

       
    using vector_t = Eigen::Matrix<double, Dynamic, 1>; 
    using matrix_t = Eigen::Matrix<double, Dynamic, Dynamic>; 
    
    using sparse_matrix_t = Eigen::SparseMatrix<double>;
    using sparsesolver_t = Eigen::SparseLU<sparse_matrix_t>;
    constexpr int local_dim = 2;
    using PointT = Eigen::Matrix<double, local_dim, 1>;
    
    std::string output_dir = executable_name(argv[0]) + "-output/";
    std::string command_str = "mkdir -p " + output_dir;
    system(command_str.c_str());


    //int N = 41; // h = 1 / (N-1) 
    std::vector<int> N_NODES = {11, 21, 41, 81};
    vector_t errors_L2 = vector_t::Zero(N_NODES.size());
    vector_t errors_N = vector_t::Zero(N_NODES.size());
    
    std::mt19937 gen(12345);
    // !!!
    for( int n=0; n<N_NODES.size(); ++n){
    std::cout << "---- N = " << N_NODES[n] << " ----" << std::endl;
    Triangulation<2, 2> unit_square = Triangulation<2, 2>::UnitSquare(N_NODES[n], cache_cells);
    
    // finite element space
    FeSpace Vh(unit_square, P1<1>);   // piecewise linear continuous scalar finite elements
    TrialFunction u(Vh);
    TestFunction  v(Vh);
    
    auto& dof_handler = Vh.dof_handler();
    int n_dofs = dof_handler.n_dofs();
    // Dirichlet BCs
    ScalarField<2, decltype([](const PointT&) { return 0; })> g;
    dof_handler.set_dirichlet_constraint(/* on = */  BoundaryAll, /* data = */ g);

    // !!!!!!
    auto _forcing = [](const PointT& p) {
        return -2*( p[0]*(p[0] - 1.) + p[1]*(p[1] - 1.) );
    };
    ScalarField<2> forcing;
    forcing = _forcing;

    auto exact = [](const PointT& p) {
        return ( p[0]*(p[0] - 1.) * p[1]*(p[1] - 1.) );
    };

    vector_t f_ex = vector_t::Zero(n_dofs);
    for(int i=0; i < unit_square.nodes().rows(); ++i){
        f_ex[i] = exact(unit_square.nodes().row(i) );
    }

    // variational forms
    auto a = integral(unit_square)(dot(grad(u), grad(v)));
    auto F = integral(unit_square)(forcing* v);
    sparse_matrix_t mass = integral(unit_square)(u*v).assemble();
    
    // data generation
    int n_locs = 200;
    matrix_t locs = unit_square.sample(n_locs);
    
   
    vector_t response = vector_t::Zero(n_locs);
    for(int i=0; i < n_locs; ++i){
        response[i] = exact(locs.row(i));  // no noise
    } 
    
    response += noise(n_locs, 1, 0.05, gen); // with noise, sigma = 0.05

    GeoFrame data(unit_square);
    auto &l = data.insert_scalar_layer<POINT>("layer", locs);
    l.load_vec("y", response);
    
    // SR-PDE
    double lambda = 1.0;
    SRPDE elliptic("y ~ f", data, fe_ls_elliptic(a, F));
    
    elliptic.fit(lambda);

    // ERRORs
    errors_L2[n] = std::sqrt( (elliptic.f() - f_ex).dot( mass*(elliptic.f() - f_ex) ));
    errors_N[n] = std::sqrt( (elliptic.f() - f_ex).array().square().mean() ); 
    
    std::cout << "L2 error: " << errors_L2[n] << std::endl;
    std::cout << "N error: " << errors_N[n] << std::endl;
    // save results ???
    // .....
    
    }

    vector_t prev_L2 = errors_L2.head(N_NODES.size() - 1);
    vector_t next_L2 = errors_L2.tail(N_NODES.size() - 1);
    vector_t rates_L2 = (prev_L2.array() / next_L2.array()).log() / std::log(2.0);
    
    vector_t prev_N = errors_N.head(N_NODES.size() - 1);
    vector_t next_N = errors_N.tail(N_NODES.size() - 1);
    vector_t rates_N = (prev_N.array() / next_N.array()).log() / std::log(2.0);
    
    std::cout << "\nL2 rates: " << rates_L2.transpose() << std::endl;
    std::cout << "N rates: " << rates_N.transpose() << std::endl;
    // change output folder permissions
    
    // Eigen::saveMarket(grid, output_dir + "rates_L2.mtx");
    eigen2txt<double>(rates_L2, output_dir + "rates_L2.txt");
    
    command_str = "chown -R 1000:1000 " + output_dir;
    system(command_str.c_str());
    return 0;
}