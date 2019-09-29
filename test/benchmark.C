#include "benchmark/benchmark.h"
#include "FortCpp.h"
#include "Eigen/Dense"

static void BM_add_two_arrays_into_third(benchmark::State& state) {
   int N = state.range(0);
   std::vector<double> a(N,1),b(N,2),c(N,0);
   while (state.KeepRunning()) {
      benchmark::DoNotOptimize(c.data());
      for(int i=0; i<N; i++) {
         c[i] = a[i] + b[i];
      }
      benchmark::ClobberMemory();
   }
}
BENCHMARK(BM_add_two_arrays_into_third)->Range(0,8<<20);

static void BM_add_two_FC_arrays_into_third(benchmark::State& state) {
   int N = state.range(0);
   FortCpp::Alloc<double,1> a(N),b(N),c(N);
   a = 1;
   b = 2;
   c = 0;
   while (state.KeepRunning()) {
      benchmark::DoNotOptimize(c.data());
      c = a + b;
      benchmark::ClobberMemory();
   }
}
BENCHMARK(BM_add_two_FC_arrays_into_third)->Range(0,8<<20);

static void BM_add_two_Eigen_arrays_into_third(benchmark::State& state) {
   int N = state.range(0);
   Eigen::ArrayXd a(N),b(N),c(N);
   a = 1;
   b = 2;
   c = 0;
   while (state.KeepRunning()) {
      benchmark::DoNotOptimize(c.data());
      c = a + b;
      benchmark::ClobberMemory();
   }
}
BENCHMARK(BM_add_two_Eigen_arrays_into_third)->Range(0,8<<20);

BENCHMARK_MAIN();
