#include <iostream>
#include <vector>
#include "encryption3d.hpp"

namespace encrypt3d {

  void Encryption3D::insereMsgPaillier(const std::string& msg, int bpf, const Eigen::MatrixXf& inMesh, Eigen::MatrixXd& outMesh, const Paillier& paillier)
  {
    uint32_t msg_len_bits = msg.size() * sizeof(char) * 8;

    std::cout << "Message bits   : " << msg_len_bits << std::endl;
    std::cout << "Available bits : " << inMesh.rows() * 3 * 2 << std::endl;

    if (msg_len_bits > inMesh.rows() * 3 * bpf) {
      std::cout << "Message is too long to insert!\n";
      exit(-1);
    }

    auto coords = inMesh.array();
    outMesh = inMesh.cast<double>();
    auto encrypted_floats = outMesh.array();

    BitStream msg_bits = string_to_bit_stream_(msg);

    //for (const auto& e : msg) {
    //  std::cout << std::bitset<8>(e) << " ";
    //}
    //std::cout << "\nMessage bit stream : " << msg_bits << std::endl;

    // Chaque float insere bpf bit
    for (int i = 0; i < coords.size(); ++i) {
      Chiffre32 f = Chiffre32::fromFloat(coords(i));
      Chiffre32 mantisse = f.mantisse();
      Chiffre32 message;

      if ((i+1) * bpf > msg_bits.size()) {

        message = Chiffre32::fromUint32(0);

      } else {

        message = Chiffre32::fromUint32(get_n_bits_of_bit_stream_(msg_bits, i * bpf, bpf));

      }

      Chiffre64 e = encrypte_(mantisse, message, bpf, paillier);

      encrypted_floats(i) = Chiffre64::fromChiffre32(f).remplaceMantisse(e).toDouble();

      /*
      std::cout << "Float          : " << f << " = " << f.toFloat() << std::endl;
      std::cout << "Encrypt mantisse " << mantisse << " & message " << message << std::endl;
      std::cout << "Encrypt result : " << Chiffre64::fromChiffre32(f).remplaceMantisse(e) << " = "
                << Chiffre64::fromChiffre32(f).remplaceMantisse(e).toDouble()
                << "         saved : " << encrypted_floats(i) << std::endl;
      */
    }
  }

  void Encryption3D::retireMsgPaillier(std::string& msg, int len, int bpf, const Eigen::MatrixXd& inMesh, Eigen::MatrixXf& outMesh, const Paillier& paillier)
  {
    auto coords = inMesh.array();
    auto out = outMesh.array();

    BitStream bitStream;

    //std::cout << "Retire " << len * 8 << "bits" << std::endl;

    for (int i = 0; i < coords.size(); ++i) {
      Chiffre64 f = Chiffre64::fromDouble(coords(i));

      DecryptionData data = decrypte_(f.mantisse(), bpf, paillier);

      if (bitStream.size() < len * 8) {
        dump_n_bits_to_bit_stream_(bitStream, data.message.toUint32(), 32-bpf, bpf);
      }

      out(i) = Chiffre32::fromDouble(coords(i)).remplaceMantisse(data.mantisse).toFloat();

      //std::cout << "Current bit stream : " << bitStream << std::endl;
      /*
      std::cout << "Decrypt from double : " << f << " = " << f.toDouble() << std::endl;
      std::cout << "Decrypt result : " << Chiffre32::fromDouble(coords(i)).remplaceMantisse(data.mantisse) << " = "
                << Chiffre32::fromDouble(coords(i)).remplaceMantisse(data.mantisse).toFloat()
                << "         saved : " << out(i) << std::endl
      */
    }

    outMesh = out;

    //std::cout << "Final bit stream   : " << bitStream << std::endl;

    msg = bit_stream_to_string_(bitStream);
  }

  Chiffre64 Encryption3D::encrypte_(const encrypt3d::Chiffre32& mantisse,
                                    const encrypt3d::Chiffre32& message,
                                    int bpf,
                                    const encrypt3d::Paillier& paillier)
  {
    // left shift la mantisse
    Chiffre32 mantisee_ = mantisse.leftShift(bpf);
    Chiffre64 e_mantisse = paillier.encrypte(mantisee_);
    Chiffre64 e_message  = paillier.encrypte(message);

    /*
    Chiffre64 t = paillier.multiply(e_mantisse, e_message);
    Chiffre32 dt = paillier.decrypte(t);
    std::cout << "Mantisse  [in] : " << mantisse << std::endl
              << "Mantisse_      : " << mantisee_ << std::endl
              << "Message   [in] : " << message << std::endl
              << "E(Mantisse_)   : " << e_mantisse << std::endl
              << "E(Message)     : " << e_message << std::endl
              << "E(M_M)         : " << t << std::endl
              << "D(E(M_M))      : " << dt << std::endl << std::endl;
    */

    return paillier.multiply(e_mantisse, e_message);
  }

  Encryption3D::DecryptionData Encryption3D::decrypte_(const encrypt3d::Chiffre64 &c, int bpf, const encrypt3d::Paillier &paillier)
  {
    Chiffre32 d_c = paillier.decrypte(c);
    DecryptionData data;
    data.mantisse = d_c.rightShift(bpf);
    data.message = Chiffre32::fromUint32(Chiffre32::getNBits(d_c.toUint32(), 32-bpf, bpf));

    /*
    std::cout << "Decrypted : " << d_c << std::endl
              << "Decrypted Mantisse  : " << data.mantisse << std::endl
              << "Decrypted Message  : "  << std::bitset<8>(data.message.toUint32()) << std::endl;
    */

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

  void Encryption3D::insereMsgRSA(const std::string& msg, int bpf, const Eigen::MatrixXf &inMesh, Eigen::MatrixXf &outMesh, const RSA& rsa)
  {
    uint32_t msg_len_bits = msg.size() * sizeof(char) * 8;

    if (msg_len_bits > inMesh.rows() * 3 * bpf) {
      std::cout << "Message is too long to insert!\n";
      exit(-1);
    }

    auto coords = inMesh.array();
    outMesh = inMesh;
    auto encrypted_floats = outMesh.array();

    BitStream msg_bits = string_to_bit_stream_(msg);

    for (int i = 0; i < coords.size(); ++i) {
      Chiffre32 f = Chiffre32::fromFloat(coords(i));
      Chiffre32 message;

      if ((i+1) * bpf > msg_bits.size()) {

        message = Chiffre32::fromUint32(0);

      } else {

        message = Chiffre32::fromUint32(get_n_bits_of_bit_stream_(msg_bits, i * bpf, bpf));

      }

      Chiffre32 chiffre = f.encrypteMantisseRSA(21, rsa);
      chiffre.insereMessageLSB(bpf, message.toUint32());

      encrypted_floats(i) = chiffre.toFloat();
    }
  }

  std::ostream& operator<<(std::ostream& os, const Encryption3D::BitStream& bitStream)
  {
    for (const auto& e : bitStream) {
      os << (unsigned)e;
    }
    os << std::endl;
    return os;
  }

}
