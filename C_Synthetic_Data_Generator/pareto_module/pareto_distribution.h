
#include <limits>
#include <random>
#include <boost/math/distributions/pareto.hpp>

template<class Float>
class pareto_distribution {
  public:
  using result_type = Float;
  using param_type = std::pair<Float,Float>;

  pareto_distribution(): p(1,1) {}
  pareto_distribution(param_type p): p(p) {}
  pareto_distribution(Float shape, Float scale): p{shape,scale} {}

  Float min() const { return 0; }
  Float max() const { return std::numeric_limits<Float>::max(); }

  bool operator==(pareto_distribution const& rhs) const {
    return p == rhs.p;
  }
  bool operator!=(pareto_distribution const& rhs) const {
    return p != rhs.p;
  }

  template <class RandomGenerator>
  result_type operator()(RandomGenerator& r) {
    return (*this)(r, p);
  }

  template <class RandomGenerator>
  result_type operator()(RandomGenerator& r, param_type p) {
    std::uniform_real_distribution<double> d(0,1.0);
    boost::math::pareto pareto(p.second, p.first);
    double uniform = d(r);
    return boost::math::quantile(pareto, uniform);
  }

  template <class CharT, class Traits>
  friend std::basic_ostream<CharT, Traits>& operator<<(
      std::basic_ostream<CharT, Traits>& os,
      pareto_distribution<Float>& pareto
    );
  template <class CharT, class Traits>
  friend std::basic_istream<CharT, Traits>& operator>>(
      std::basic_istream<CharT, Traits>& os,
      pareto_distribution<Float>& pareto
    );

  private:
    param_type p;
};

template <class Float, class CharT, class Traits = std::char_traits<CharT>>
std::basic_ostream<CharT, Traits>& operator<<(
      std::basic_ostream<CharT, Traits>& os,
      pareto_distribution<Float> const& pareto
    ) {
  os << pareto.p.first << ' ' << pareto.p.second;
  return os;
}

template <class Float, class CharT, class Traits = std::char_traits<CharT>>
std::basic_istream<CharT, Traits>& operator>>(
      std::basic_istream<CharT, Traits>& os,
      pareto_distribution<Float>& pareto
    ) {
  os >> pareto.p.first >> ' ' >> pareto.p.second;
  return os;
}
