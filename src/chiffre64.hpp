#ifndef MINIPROJET_CHIFFRE64_HPP
#define MINIPROJET_CHIFFRE64_HPP

#include "chiffre32.hpp"

namespace encrypt3d {

  typedef uint64_t uint64;

  class Chiffre64 {
  private:
    double m_double {};
    uint64 m_uint64 {};

  public:
    Chiffre64() = default;

    double toDouble() const { return m_double; }
    uint64 toUint64() const { return m_uint64; }

    static Chiffre64 fromUint32(uint32 value);
    static Chiffre64 fromUint64(uint64 value);
    static Chiffre64 fromFloat(float value);
    static Chiffre64 fromDouble(double value);
    static Chiffre64 fromChiffre32(const Chiffre32& value);

    Chiffre64 mantisse() const;

    Chiffre64 remplaceMantisse(const Chiffre64& m) const;

  private:
    static uint64 get_n_bits_(uint64 f, int beg, int n);
    static uint64 remplace_n_bits(uint64 f, int beg, int n, uint64 val);
  };

  std::ostream& operator<<(std::ostream& os, const Chiffre64& c);
}

#endif //MINIPROJET_CHIFFRE64_HPP
