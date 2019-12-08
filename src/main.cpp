#include <igl/readOFF.h>
#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>
#include <sstream>
#include <libigl/include/igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <libigl/external/imgui/imgui.h>

#include "encryption3d.hpp"
#include "chiffre32.hpp"
#include "chiffre64.hpp"
#include "paillier.hpp"

#ifndef __STDC_IEC_559__
  #error The following Programm only supports float operations using the IEEE 754 Standard.
#endif

bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifiers);
void menu_callback();

Eigen::MatrixXd viewV;
Eigen::MatrixXd V;
Eigen::MatrixXi F;
Eigen::MatrixXd encryptedV;
Eigen::MatrixXf decryptedV;

igl::opengl::glfw::imgui::ImGuiMenu menu;
igl::opengl::glfw::Viewer viewer;
bool showLabel = false;

int main(int argc, char** argv)
{
  using namespace encrypt3d;

  // bits per float
  int bpf = 2;

  if (argc < 2) {
    std::cout << "MiniProjet <model.obj>\n";
  }

  Paillier paillier(26);

  std::string path(argv[1]);

  std::string ext = path.substr(path.find_last_of('.') + 1, 3);
  if (ext == "obj") {
    igl::readOBJ(path, V, F);

  } else if (ext == "off") {

    igl::readOFF(path, V, F);

  } else {
    std::cout << "Cannot read file." << std::endl;
    exit(-1);
  }

  viewV = V;

  std::string message = "1234";

  std::cout << "Message : " << message << std::endl;

  encryptedV = Eigen::MatrixXd(V.rows(), V.cols());

  Eigen::MatrixXf Vf = V.cast<float>();

  Encryption3D::insereMsgPaillier1(message, bpf, Vf, encryptedV, paillier);

  std::string decryptedMessage;
  decryptedV = Eigen::MatrixXf(V.rows(), V.cols());
  Encryption3D::retireMsgPaillier1(decryptedMessage, message.size(), bpf, encryptedV, decryptedV, paillier);

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

  //viewV = V;
  //viewV = encryptedV;
  //viewV = decryptedV.cast<double>();

  viewer.callback_key_down = &key_down;

  menu.callback_draw_viewer_window = &menu_callback;

  viewer.plugins.push_back(&menu);

  viewer.data().set_mesh(viewV, F);
  viewer.data().set_face_based(true);
  viewer.launch(true, false, "3D Encryption");

  Chiffre32 c = Chiffre32::fromUint32(67108728);
  std::cout << c << std::endl;
  std::cout << paillier.encrypte(c) << std::endl;
  std::cout << paillier.decrypte(paillier.encrypte(c)) << std::endl;


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
  float a = 372.345239239f;
  float b= 390.12354f;

  /*
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
            */

  /*
  std::cout << "-----------------------" << std::endl;
  Encryption3D::BitStream bitStream = Encryption3D::string_to_bit_stream_(message);

  for (auto& e : bitStream) {
    std::cout << (unsigned)e;
  }
  std::cout << std::endl;

  std::cout << "-----------------------" << std::endl;
  float number = 4299342342.234528896f;
  auto numberd = static_cast<double>(number);
  auto numberdf = static_cast<float>(numberd);
  Chiffre32 f = Chiffre32::fromFloat(number);
  Chiffre64 d = Chiffre64::fromDouble(numberd);
  uint64_t ud = d.toUint64();
   */
  /*
  std::cout << "Number                   : " << number << std::endl
            << "                         : " << f
            << "Float2Double             : " << numberd << std::endl
            << "                         : " << std::bitset<64>(*reinterpret_cast<uint64_t*>(&numberd)) << std::endl
            << "Double2Float             : " << (float)(double)number << std::endl
            << "                         : " << std::bitset<32>(*reinterpret_cast<uint32_t*>(&numberdf)) << std::endl
            << "Chiffre64::fromFloat     : " << Chiffre64::fromFloat(number)
            << "                         : " << Chiffre64::fromFloat(number).toDouble() << std::endl
            << "Chiffre64::fromDouble    : " << Chiffre64::fromDouble((double)number)
            << "                         : " << Chiffre64::fromDouble((double)number).toDouble() << std::endl
            << "f.toUint32               : " << f
            << "Chiffre64::fromUint32    : " << Chiffre64::fromUint32(f.toUint32())
            << "                         : " << Chiffre64::fromUint32(f.toUint32()).toDouble() << std::endl
            << "Chiffre64::fromUint64    : " << Chiffre64::fromUint32((uint64_t)f.toUint32())
            << "                         : " << Chiffre64::fromUint32((uint64_t)f.toUint32()).toDouble() << std::endl
            << "Chiffre64::fromChiffre32 : " << Chiffre64::fromChiffre32(f)
            << "                         : " << Chiffre64::fromChiffre32(f).toDouble() << std::endl
            << "Chiffre32::fromDouble    : " << Chiffre32::fromDouble(numberd)
            << "                         : " << Chiffre32::fromDouble(numberd).toFloat() << std::endl
            << "Chiffre32::fromChiffre64 : " << Chiffre32::fromChiffre64(d)
            << "                         : " << Chiffre32::fromChiffre64(d).toFloat() << std::endl;
            //<< "Chiffre32::fromUint64    : " << Chiffre32::fromUint64(ud)
            //<< "                         : " << Chiffre32::fromUint64(ud).toFloat() << std::endl;
            */

  /*
  Chiffre64 e_m = Encryption3D::encrypte_(f.mantisse(), Chiffre32::fromUint32(7), paillier);
  Chiffre64 e = Chiffre64::fromChiffre32(f).remplaceMantisse(e_m);
  Encryption3D::DecryptionData data = Encryption3D::decrypte_(e.mantisse(), paillier);

  std::cout << "f         : " << f
            << "          : " << f.toFloat() << std::endl
            << "          : " << Chiffre64::fromChiffre32(f)
            << "E(m+m)    : " << e_m
            << "E(f(m+m)) : " << e
            << "          : " << e.toDouble() << std::endl
            << "D(man)    : " << data.mantisse
            << "D(msg)    : " << data.message
            << "D(E(f)    : " << Chiffre32::fromDouble(e.toDouble()).remplaceMantisse(data.mantisse)
            << "          : " << Chiffre32::fromDouble(e.toDouble()).remplaceMantisse(data.mantisse).toFloat();
  */

  return 0;
}

bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifiers)
{
  if (key == '1') {

    viewV = V;

  } else if (key == '2') {

    viewV = encryptedV;

  } else if (key == '3') {

    viewV = decryptedV.cast<double>();

  } else if (key == 's' || key == 'S') {

    showLabel = !showLabel;

  }

  viewer.data().clear();
  viewer.data().set_mesh(viewV, F);

  viewer.core().align_camera_center(V, F);

  if (showLabel) {
    for (int i = 0; i < viewV.rows(); ++i) {
      std::stringstream s;
      Eigen::VectorXd p = viewV.row(i);
      s << p(0) << ", " << p(1) << ", " << p(2);
      viewer.data().add_label(p, s.str());
    }
  } else {
    viewer.data().clear_labels();
  }

  viewer.data().set_face_based(true);

  //std::cout << viewV << std::endl << std::endl;

  return true;
}

void menu_callback()
{
  // Define next window position + size
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(200, 360), ImGuiSetCond_FirstUseEver);
  ImGui::Begin("Parameters", nullptr,ImGuiWindowFlags_NoSavedSettings);

  // Expose the same variable directly ...
  static double doubleVariable;
  ImGui::PushItemWidth(-80);
  ImGui::DragScalar("double", ImGuiDataType_Double, &doubleVariable, 0.1, 0, 0, "%.4f");
  ImGui::PopItemWidth();

    static float floatVariable;
    // Expose variable directly ...
    ImGui::InputFloat("float", &floatVariable, 0, 0, 3);

    if (ImGui::Checkbox("Show Label ?", &showLabel))
    {
      if (showLabel) {
        for (int i = 0; i < viewV.rows(); ++i) {
          std::stringstream s;
          Eigen::VectorXd p = viewV.row(i);
          s << p(0) << ", " << p(1) << ", " << p(2);
          viewer.data().add_label(p, s.str());
        }
      } else {
        viewer.data().clear_labels();
      }
    }

    // Expose an enumeration type
    enum Orientation { Up=0, Down, Left, Right };
    static Orientation dir = Up;
    ImGui::Combo("Direction", (int *)(&dir), "Up\0Down\0Left\0Right\0\0");

    // Add a button
    if (ImGui::Button("Print Hello", ImVec2(-1,0)))
    {
      std::cout << "Hello\n";
    }

  ImGui::End();
}