#include <iostream>
#include <omp.h>
#include <math.h>

double f(double x){
    return exp(x);
}

int thread_count = 8;

double Trap(double a, double b, int n){
    double h, x, my_result;
    double local_a, local_b;
    int i, local_n;
    int my_rank = omp_get_thread_num();
    //int thread_count = omp_get_num_threads();

    h = (b - a) / n;
    local_n = n/thread_count;
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n * h;
    my_result = (f(local_a) + f(local_b)) / 2.0;
    for (i = 1; i < local_n; i++){
        x = local_a + i * h;
        my_result += f(x);
    }
    my_result = my_result * h;

    return my_result;
}

int main(int argc, char *argv[]){
    double global_result = 0.0;
    double a, b;
    int n;

    a = 100;
    b = 99;
    n = 1000000000;

    // std::cout << "Entre a: ";
    // std::cin >> a;

    // std::cout << "Entre b: ";
    // std::cin >> b;

    // std::cout << "Entre n: ";
    // std::cin >> n;

    #pragma omp parallel num_threads(thread_count) reduction(+: global_result)
    global_result += Trap(a, b, n);

    std::cout << "Com " << thread_count << "threads" << std::endl;
    std::cout << "Com " << n << " trapezios a aproximacao é: " << global_result << std::endl;
    return 0;
}