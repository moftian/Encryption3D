#include <igl/readOFF.h>
#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>

#include "encryption3d.hpp"
#include "chiffre32.hpp"
#include "chiffre64.hpp"
#include "paillier.hpp"

#ifndef __STDC_IEC_559__
  #error The following Programm only supports float operations using the IEEE 754 Standard.
#endif

int main(int argc, char** argv)
{
  using namespace encrypt3d;

  Eigen::MatrixXd V;
  Eigen::MatrixXi F;

  igl::readOBJ("../models/suzanne.obj", V, F);

  std::string message = "asdffe0t3-4t345df";


  std::cout << "Message : " << message << std::endl;

  Eigen::MatrixXd encryptedV(V.rows(), V.cols());

  Eigen::MatrixXf encryptedF = V.cast<float>();

  Paillier paillier(26);
  Encryption3D::insereMsgPaillier1(message, encryptedF, encryptedV, paillier);

  std::string decryptedMessage;
  Eigen::MatrixXf decryptedV(V.rows(), V.cols());
  Encryption3D::retireMsgPaillier1(decryptedMessage, message.size(), encryptedV, decryptedV, paillier);

  std::cout << "DecryptedMessage : " << decryptedMessage << std::endl;

  /*
  Chiffre32 a = Chiffre32::fromUint32('a');
  std::cout << "a : " << a;
  Chiffre64 ae = paillier.encrypte(a);
  std::cout << ae;
  std::cout << paillier.decrypte(ae);
   */

  /*
  std::cout << V.row(0) << std::endl;
  for (int i = 0; i < V.rows(); ++i) {
    std::cout << Chiffre32::fromDouble(V.row(i)(0)) << Chiffre32::fromDouble(V.row(i)(1)) << Chiffre32::fromDouble(V.row(i)(2)) << std::endl
              << Chiffre64::fromDouble(encryptedV.row(i)(0)) << Chiffre32::fromDouble(encryptedV.row(i)(1)) << Chiffre32::fromDouble(encryptedV.row(i)(2)) << std::endl
              << Chiffre64::fromFloat(decryptedV.row(i)(0)) << Chiffre32::fromDouble(decryptedV.row(i)(1)) << Chiffre32::fromDouble(decryptedV.row(i)(2)) << std::endl;
  }
   */

  Eigen::MatrixXd viewV;

  //viewV = encryptedF.cast<double>();
  //viewV = encryptedV * 1e308;
  viewV = decryptedV.cast<double>() * 1e38;

  //std::cout << viewV;

  igl::opengl::glfw::Viewer viewer;

  viewer.data().set_mesh(viewV, F);
  viewer.launch(true, false, "3D Encryption");

  /*

  RSA rsa2(17);
  rsa2.generate(23, 11);
  //rsa2.generate(863, 379);

  for (long i = 0; i < verticesF.rows(); ++i) {
    Eigen::Vector3f v = verticesF.row(i);
    Eigen::Vector3f ve;
    for (int j = 0; j < 3; ++j) {
      Chiffre32 chiffre = Chiffre32::fromFloat(v(j)).encrypteMantisseRSA(8, rsa2);
      chiffre.insereMessageLSB(4, 13);
      ve(j) = chiffre.toFloat();
    }
    verticesEncrypte.row(i) = ve;
  }

  igl::opengl::glfw::Viewer viewer;
  //viewer.data().set_mesh(verticesEncrypte.cast<double>(), faces);
  viewer.data().set_mesh(verticesEncrypte.cast<double>(), faces);
  viewer.launch();
   */

  /*
  RSA rsa(17);
  rsa.generate(11, 23);

  Chiffre32 a32 = Chiffre32::fromFloat(372.345239239f);
  Chiffre32 b32 = Chiffre32::fromFloat(1.8091024502f);
  Chiffre32 am32 = a32.mantisse();
  Chiffre32 bm32 = b32.mantisse();
  Chiffre32 c32 = a32.remplaceMantisse(bm32);

  Chiffre32 ac32 = a32.encrypteMantisseRSA(8, rsa);
  Chiffre32 ad32 = ac32.decrypteMantisseRSA(8, rsa);

  Chiffre32 ai32 = ac32.insereMessageLSB(3, 7);
  Chiffre32 ar32 = ai32.retireMessageLSB(3);
   */

  /*
  Paillier pailler(26);
  float a = 372.345239239f;
  float b= 390.12354f;

  Chiffre32 ca = Chiffre32::fromFloat(a);
  Chiffre32 cb = Chiffre32::fromFloat(b);

  Chiffre32 ca_m = ca.mantisse();
  Chiffre32 cb_m = cb.mantisse();

  Chiffre64 ca_m_c = pailler.encrypte(ca_m);
  Chiffre64 cb_m_c = pailler.encrypte(cb_m);

  Chiffre32 ca_m_c_d = pailler.decrypte(ca_m_c);
  Chiffre32 cb_m_c_d = pailler.decrypte(cb_m_c);

  Chiffre64 cab_m_c = pailler.multiply(ca_m_c, cb_m_c);

  Chiffre32 cab_m_c_d = pailler.decrypte(cab_m_c);

  std::cout << "a           : " //<< ca << " : "
            << ca.toFloat() << std::endl
            << "a_M         : " //<< ca_m << " : "
            << ca_m.toUint32() << std::endl
            << "E(a_M)      : " //<< ca_m_c << " : "
            << ca_m_c.toUint64() << std::endl
            << "D(E(a_M))   : " //<< ca_m_c_d << " : "
            << ca_m_c_d.toUint32() << std::endl
            << "b           : "// << cb << " : "
            << cb.toFloat() << std::endl
            << "b_M         : " //<< cb_m << " : "
            << cb_m.toUint32() << std::endl
            << "E(b_M)      : " //<< ca_m_c << " : "
            << cb_m_c.toUint64() << std::endl
            << "D(E(b_M))   : " //<< ca_m_c_d << " : "
            << cb_m_c_d.toUint32() << std::endl
            << "E(a_M)E(b_M): " //<< cab_m_c << " : "
            << cab_m_c.toUint64() << std::endl
            << "D(E(a_M)E(b_M)): " //<< cab_m_c_d << " : "
            << cab_m_c_d.toUint32() << std::endl
            << "a_M + b_M : " << ca_m.toUint32() + cb_m.toUint32() << std::endl;

  std::string msg = "abc";

  Encryption3D::BitStream bitStream = Encryption3D::string_to_bit_stream_(message);

  for (auto& e : bitStream) {
    std::cout << (unsigned)e;
  }
  std::cout << std::endl;

  std::cout << "_----------------------" << std::endl;
  Chiffre32 f = Chiffre32::fromFloat(4299342342.234528896f);

  Chiffre64 e = Encryption3D::encrypte_(f.mantisse(), Chiffre32::fromUint32(7), paillier);
  Encryption3D::DecryptionData data = Encryption3D::decrypte_(e, paillier);

  std::cout << f
            << Chiffre64::fromChiffre32(f)
            << e
            << data.mantisse
            << data.message;
              */

  return 0;
}