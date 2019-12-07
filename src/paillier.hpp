#ifndef MINIPROJET_PAILLIER_HPP
#define MINIPROJET_PAILLIER_HPP

#include "chiffre32.hpp"
#include "chiffre64.hpp"

#include <gmp.h>

extern "C" {
  #include <paillier/paillier.h>
}

#include <cstdint>

namespace encrypt3d {

  struct Paillier {
  public:
    /// Constructs Paillier Crypto-System
    /// \param [in] bits number of bits of the modulus
    explicit Paillier(int32_t bits);

    ~Paillier();

    uint64_t encrypte(uint32_t m);

    Chiffre64 encrypte(const Chiffre32 &m);

    uint32_t decrypte(uint64_t c);

    Chiffre32 decrypte(const Chiffre64 &c);

  private:
    int32_t m_bits;
    paillier_prvkey_t* m_prv_key;
    paillier_pubkey_t* m_pub_key;
  };

}


#endif //MINIPROJET_PAILLIER_HPP
