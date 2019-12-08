#ifndef MINIPROJET_CHIFFRE64_HPP
#define MINIPROJET_CHIFFRE64_HPP

namespace encrypt3d {

  class Chiffre32;

  class Chiffre64 {
  private:
    double m_double {};
    uint64_t m_uint64 {};

  public:
    Chiffre64() = default;

    double toDouble() const { return m_double; }
    uint64_t toUint64() const { return m_uint64; }

    static Chiffre64 fromUint32(uint32_t value);
    static Chiffre64 fromUint64(uint64_t value);
    static Chiffre64 fromFloat(float value);
    static Chiffre64 fromDouble(double value);
    static Chiffre64 fromChiffre32(const Chiffre32& value);

    Chiffre64 mantisse() const;

    Chiffre64 remplaceMantisse(const Chiffre64& m) const;

  public:
    static uint64_t getNBits(uint64_t f, int beg, int n);
    static uint64_t replaceNBits(uint64_t f, int beg, int n, uint64_t val);
  };

  std::ostream& operator<<(std::ostream& os, const Chiffre64& c);
}

#endif //MINIPROJET_CHIFFRE64_HPP
