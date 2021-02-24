#include <iostream>
#include <algorithm>
#include <vector>
#include "pareto_distribution.h"


int main(int argc, char *argv[])
{
  std::seed_seq seed{};
  std::default_random_engine eng;
  double shape = 2.0;
  double scale = 3.0;
  pareto_distribution<double> p(shape, scale);

  std::vector<double> values;
  auto rand = [&](){ return p(eng);};
  std::generate_n(std::back_inserter(values), 1000000, rand);

  double mean = std::accumulate(std::begin(values), std::end(values), 0) / static_cast<double>(values.size());
  double expected_mean = (shape*scale)/(shape-1.0);
  std::cout << "mean " << mean << std::endl;
  std::cout << "expected_mean " << expected_mean << std::endl;

  
  return 0;
}
