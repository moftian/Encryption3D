#ifndef MINIPROJET_ENCRYPTION3D_HPP
#define MINIPROJET_ENCRYPTION3D_HPP

#include <Eigen/Core>
#include "chiffre32.hpp"
#include "chiffre64.hpp"
#include "paillier.hpp"

namespace encrypt3d {

  struct Encryption3D {

    struct DecryptionData {
      Chiffre32 mantisse;
      Chiffre32 message;
    };

    using BitStream = std::vector<uint8_t>;

    /**
     * Insert message in encrypted mesh RSA.
     * @param msg     [in]
     * @param len     [in]
     * @param inMesh  [in]
     * @param outMesh [out]
     */
    static void insereMsgRSA(const std::string& msg, int bpf, const Eigen::MatrixXf &inMesh, Eigen::MatrixXf &outMesh, const RSA& rsa);

    /**
     *
     * @param msg
     * @param bpf
     * @param inMesh
     * @param outMesh
     * @param paillier
     */
    static void insereMsgPaillier(const std::string& msg, int bpf, const Eigen::MatrixXf &inMesh, Eigen::MatrixXd &outMesh, const Paillier& paillier);

    /**
     *
     * @param msg
     * @param len    [in] on bytes
     * @param inMesh
     * @param outMesh
     * @param paillier
     */
    static void retireMsgPaillier(std::string& msg, int len, int bpf, const Eigen::MatrixXd& inMesh, Eigen::MatrixXf& outMesh, const Paillier& paillier);

    /**
     * Decrypter un chiffre de 64 bits(sans signe et exposant) a deux parties (23bits + 3bits)
     * @param c        [in]
     * @param paillier [in]
     * @return DecryptionData contains a 23 bits mantisse and a 3 bits message
     */
    static DecryptionData decrypte_(const Chiffre64& c, int bpf, const Paillier& paillier);

    /**
     * Chiffrer la mantisse (23 bits) and le message (3 bits) ensemble.
     * @param mantisse [in]
     * @param message  [in]
     * @param paillier [in]
     * @return E(mantisse) * E(message) = E(mantisse + message)
     */
    static Chiffre64 encrypte_(const Chiffre32& mantisse, const Chiffre32& message, int bpf, const Paillier& paillier);

    /**
     * Convertir un message de string en bits.
     * @param message [in]
     * @return Message in bits representation.
     */
    static BitStream string_to_bit_stream_(const std::string& string);

    static std::string bit_stream_to_string_(const BitStream& stream);

    static uint32_t get_n_bits_of_bit_stream_(const BitStream& msg, int pos, int n);

    static void dump_n_bits_to_bit_stream_(BitStream& bitStream, uint32_t bits, int pos, int n);
  };

  std::ostream& operator<<(std::ostream& os, const Encryption3D::BitStream& bitStream);

}

#endif //MINIPROJET_ENCRYPTION3D_HPP
