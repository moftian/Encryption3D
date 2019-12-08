#include <iostream>
#include <vector>
#include "encryption3d.hpp"

namespace encrypt3d {

  void Encryption3D::insereMsgPaillier2(const std::string& msg, int blockSize, const Eigen::MatrixXf& inMesh, Eigen::MatrixXf& outMesh)
  {
    /*
    if (len <= 0 || msg == nullptr || blockSize <= 0)
      return;

    //Eigen::Map<Eigen::VectorXf> coords(inMesh.data(), inMesh.);;

    auto coords = inMesh.array();

    int msg_len_bits = len * 8;

    std::cout << "Mesh vertices num : " << inMesh.rows() << std::endl;
    std::cout << "Mesh float num : " << coords.array().size() << std::endl;
    std::cout << "Message bits : " << msg_len_bits << std::endl;

    // Combien de bits a inserer dans un chiffre flottant
    int k = 0;
    for (k = 1; k < sizeof(float);) {
      if (msg_len_bits / k > coords.size())
        k++;
      else
        break;
    }

    std::cout << k << " bits de message per float.\n";

    // reorganiser les float en blocs
    for (int i = 0; i < coords.size(); i += blockSize) {
      if (i * blockSize <= msg_len_bits) {
        std::vector<Chiffre32> block(blockSize);
        for (int j = 0; j < blockSize; ++j) {
          block[j] = Chiffre32::fromFloat(coords(i + j)).mantisse();
        }
      }
      break;
    }
     */
  }

  void Encryption3D::insereMsgPaillier1(const std::string& msg, const Eigen::MatrixXf& inMesh, Eigen::MatrixXd& outMesh, const Paillier& paillier)
  {
    uint32_t msg_len_bits = msg.size() * 8;

    if (msg_len_bits > inMesh.rows() * 3 * 3) {
      std::cout << "Message is too long to insert!\n";
      return;
    }

    auto coords = inMesh.array();
    outMesh = inMesh.cast<double>();
    auto encrypted_floats = outMesh.array();

    // Message char* to bit stream
    BitStream msg_bits = string_to_bit_stream_(msg);

    // Chaque float insere 3 bit
    for (int i = 0; i < coords.size(); ++i) {
      Chiffre32 f = Chiffre32::fromFloat(coords(i));
      Chiffre32 mantisse = f.mantisse();
      Chiffre32 message;

      if ((i+1) * 3 > msg_bits.size()) {

        message = Chiffre32::fromUint32(0);

      } else {

        message = Chiffre32::fromUint32(get_n_bits_of_bit_stream_(msg_bits, i * 3, 3));

      }

      Chiffre64 e = encrypte_(mantisse, message, paillier);
      encrypted_floats(i) = Chiffre64::fromChiffre32(f).remplaceMantisse(e).toDouble();
    }
  }

  void Encryption3D::retireMsgPaillier1(std::string& msg, const Eigen::MatrixXd& inMesh, Eigen::MatrixXf& outMesh, const Paillier& paillier)
  {
    auto coords = inMesh.array();
    auto out = outMesh.array();

    BitStream bitStream;

    for (int i = 0; i < coords.size(); ++i) {
      Chiffre64 f = Chiffre64::fromDouble(coords(i));
      Chiffre64 mantisse = f.mantisse();
      DecryptionData data = decrypte_(f.mantisse(), paillier);

      if (data.message.toUint32() != 0) {
        dump_n_bits_to_bit_stream_(bitStream, data.message.toUint32(), 29, 3);
      }

      Chiffre32 d = Chiffre32::fromDouble(coords(i));

      out(i) = d.remplaceMantisse(data.mantisse).toFloat();
    }

    outMesh = out;

    msg = bit_stream_to_string_(bitStream);
  }

  Chiffre64 Encryption3D::encrypte_(const encrypt3d::Chiffre32& mantisse,
                                    const encrypt3d::Chiffre32& message,
                                    const encrypt3d::Paillier& paillier)
  {
    // left shift la mantisse
    Chiffre32 mantisee_ = mantisse.leftShift(3);
    Chiffre64 e_mantisse = paillier.encrypte(mantisee_);
    Chiffre64 e_message  = paillier.encrypte(message);

    /*
    Chiffre64 t = paillier.multiply(e_mantisse, e_message);
    Chiffre32 dt = paillier.decrypte_(t);
    std::cout << " (man): " << mantisee_
              << "E(man): " << e_mantisse
              << " (msg): " << message
              << "E(msg): " << e_message
              << "E(mm) : " << t
              << "D(mm) : " << d;1
              */

    return paillier.multiply(e_mantisse, e_message);
  }

  Encryption3D::DecryptionData Encryption3D::decrypte_(const encrypt3d::Chiffre64 &c, const encrypt3d::Paillier &paillier)
  {
    Chiffre32 d_c = paillier.decrypte(c);
    DecryptionData data;
    data.mantisse = d_c.rightShift(3);
    data.message = Chiffre32::fromUint32(Chiffre32::getNBits(d_c.toUint32(), 23, 3));
    return data;
  }

  std::vector<uint8_t> Encryption3D::string_to_bit_stream_(const std::string &string)
  {
    std::vector<uint8_t> msg_bits;
    for (const char& m : string) {
      for (uint32_t j = 0; j < 8; ++j) {
        msg_bits.push_back(((m << j) & 0x80) >> 7);
      }
    }
    return msg_bits;
  }

  std::string Encryption3D::bit_stream_to_string_(const BitStream& stream)
  {
    std::string string;
    for (int i = 0; i < stream.size(); i += 8) {
      string.push_back((char)get_n_bits_of_bit_stream_(stream, (i/8)*8, 8));
    }

    return string;
  }

  uint32_t Encryption3D::get_n_bits_of_bit_stream_(const BitStream& msg, int pos, int n)
  {
    if (pos < 0 || n <= 0 || pos > msg.size()) return 0;

    uint32_t out = 0;
    for (int i = 0; i < n; ++i) {
      out |= (msg[pos+i] << (unsigned)(n-i-1));
    }

    return out;
  }

  void Encryption3D::dump_n_bits_to_bit_stream_(BitStream& bitStream, uint32_t bits, int pos, int n)
  {
    if (pos < 0 || n <= 0 || pos > 32) return;

    for (int i = 0; i < n; ++i) {
      bitStream.push_back(((bits << (pos+i)) & 0x80000000) >> 31);
    }
  }

}
