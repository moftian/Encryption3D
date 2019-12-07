
#include <cstdlib>
#include "paillier.hpp"

namespace encrypt3d {

  Paillier::Paillier(int32_t n)
      : m_pub_key{nullptr}, m_prv_key{nullptr} {
    m_bits = n;
    // generate public and private keys
    paillier_keygen(n, &m_pub_key, &m_prv_key, paillier_get_rand_devurandom);
  }

  Paillier::~Paillier() {
    paillier_freepubkey(m_pub_key);
    paillier_freeprvkey(m_prv_key);
  }

  Chiffre64 Paillier::encrypte(const encrypt3d::Chiffre32& m)
  {
    return Chiffre64::fromUint64(encrypte(m.toUint32()));
  }

  uint64_t Paillier::encrypte(uint32_t m) {
    //gmp_printf("==== encryption ====\n");
    //gmp_printf("Input: %d\n", m);

    paillier_plaintext_t *mtxt = paillier_plaintext_from_ui(m);
    //gmp_printf("PlainTxt: %Zd\n", mtxt);

    paillier_ciphertext_t *ctxt = paillier_enc(nullptr, m_pub_key, mtxt, paillier_get_rand_devurandom);
    //gmp_printf("CipherTxt: %Zd\n", ctxt);

    uint64_t c = mpz_get_ui(ctxt->c);

    //gmp_printf("Output: %d\n", c);
    //gmp_printf("==== ======== ====\n");

    paillier_freeplaintext(mtxt);
    paillier_freeciphertext(ctxt);

    return c;
  }

  Chiffre32 Paillier::decrypte(const encrypt3d::Chiffre64& c)
  {
    return Chiffre32::fromUint32(decrypte(c.toUint64()));
  }

  uint32_t Paillier::decrypte(uint64_t c) {
    //gmp_printf("==== decryption ====\n");
    //gmp_printf("Input: %d\n", c);

    auto *ctxt = (paillier_ciphertext_t *) malloc(sizeof(paillier_ciphertext_t));
    mpz_init_set_ui(ctxt->c, c);
    //gmp_printf("CipherTxt: %Zd\n", ctxt);

    paillier_plaintext_t *mtxt = paillier_dec(nullptr, m_pub_key, m_prv_key, ctxt);
    //gmp_printf("DecTxt: %Zd\n", mtxt);

    uint32_t m = mpz_get_ui(mtxt->m);

    paillier_freeciphertext(ctxt);
    paillier_freeplaintext(mtxt);

    //gmp_printf("Output: %d\n", m);
    //gmp_printf("==== ======== ====\n");

    return m;
  }

}
