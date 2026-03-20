#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <chrono> 
#include <filesystem>
#include <limits>
#include <algorithm>
#include <Eigen/Dense>
#include <unsupported/Eigen/SparseExtra>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i + 1 != v.size()) os << " ";
    }
    return os;
}

auto noise(std::size_t nrow, std::size_t ncol, double sigma, std::mt19937& gen) {
    Eigen::MatrixXd res = Eigen::MatrixXd::Zero(nrow, ncol);
    std::normal_distribution<> __noise(0.0, sigma);
    for (std::size_t i = 0; i < nrow; ++i){
      for( std::size_t j = 0; j < ncol; ++j ){
       res(i, j) = __noise(gen); 
      }
    }
    return res;
}

std::string executable_name(const char* argv0) {
    std::string s(argv0);
    size_t pos = s.find_last_of("/\\");
    if (pos == std::string::npos)
        return s; 
    return s.substr(pos + 1);
}

template<typename T> void eigen2ext(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& M, const std::string& sep, const std::string& filename, bool append = false){
    std::ofstream file;

    if(!append) 
        file.open(filename);
    else
        file.open(filename, std::ios_base::app); 
    
    for(std::size_t i = 0; i < M.rows(); ++i) {
            for(std::size_t j=0; j < M.cols()-1; ++j) file << M(i,j) << sep;
            file << M(i, M.cols()-1) <<  "\n";  
    }
    file.close();
}

template<typename T> void eigen2txt(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& M, const std::string& filename = "mat.txt", bool append = false){
    eigen2ext<T>(M, " ", filename, append);
}

template<typename T> void eigen2csv(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& M, const std::string& filename = "mat.csv", bool append = false){
    eigen2ext<T>(M, ",", filename, append);
}

template< typename T> void vector2ext(const std::vector<T>& V, const std::string& sep, const std::string& filename, bool append = false){
    std::ofstream file;

    if(!append) 
        file.open(filename);
    else
        file.open(filename, std::ios_base::app);
    
    for(std::size_t i = 0; i < V.size()-1; ++i) file << V[i] << sep;
    
    file << V[V.size()-1] << "\n";  
    
    file.close();
}

template< typename T> void vector2txt(const std::vector<T>& V, const std::string& filename = "vec.txt", bool append = false){
   vector2ext<T>(V, " ", filename, append);
}

template< typename T> void vector2csv(const std::vector<T>& V, const std::string& filename = "vec.csv", bool append = false){
   vector2ext<T>(V, ",", filename, append);
}

void write_table(const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& M, const std::vector<std::string>& header = {}, const std::string& filename = "data.txt"){

    std::ofstream file(filename);

    if(header.empty() || header.size() != M.cols()){
        std::vector<std::string> head(M.cols());
        for(std::size_t i = 0; i < M.cols(); ++i)
                head[i] =  "V" + std::to_string(i);
        vector2txt<std::string>(head, filename);    
    }else vector2txt<std::string>(header, filename);
    
    eigen2txt<double>(M, filename, true);
}

void write_csv(const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& M, const std::vector<std::string>& header = {}, const std::string& filename = "data.csv"){
    std::ofstream file(filename);

    if(header.empty() || header.size() != M.cols()){
        std::vector<std::string> head(M.cols());
        for(std::size_t i = 0; i < M.cols(); ++i)
                head[i] =  "V" + std::to_string(i);
        vector2csv(head, filename);    
    }else vector2csv(header, filename);
    
    eigen2csv<double>(M, filename, true);
}

#endif // __UTILS_H__