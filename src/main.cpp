#include <igl/readOFF.h>
#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>
#include <sstream>
#include <libigl/include/igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <libigl/external/imgui/imgui.h>
#include <libigl/include/igl/hausdorff.h>

#include "encryption3d.hpp"
#include "chiffre32.hpp"
#include "chiffre64.hpp"
#include "paillier.hpp"

#ifndef __STDC_IEC_559__
  #error The following Programm only supports float operations using the IEEE 754 Standard.
#endif

void method_paillier(int bpf, const encrypt3d::Paillier& paillier, const std::string& message,
                     std::string& decryptedMessage, const Eigen::MatrixXd& vertices,
                     Eigen::MatrixXd& encrypted, Eigen::MatrixXf& decrypted);
void method_naive(int bpf, const encrypt3d::RSA& rsa, const std::string& message,
                     std::string& decryptedMessage, const Eigen::MatrixXd& vertices,
                     Eigen::MatrixXf& encrypted, Eigen::MatrixXf& decrypted);

int main(int argc, char** argv)
{
  using namespace encrypt3d;

  Paillier paillier(26);
  RSA rsa(7411);
  rsa.generate(18439, 113);

  Eigen::MatrixXd V;
  Eigen::MatrixXi F;

  igl::opengl::glfw::imgui::ImGuiMenu menu;
  igl::opengl::glfw::Viewer viewer;

  viewer.plugins.push_back(&menu);

  Eigen::MatrixXd encryptedV;
  Eigen::MatrixXf decryptedV;

  int item_current = 0;
  const char* items[] = {
      "apple.off", "arma.off", "bunny.off", "cube.obj", "dodecahedron.obj", "heart.off", "icosahedron.obj", "klingon.off",
      "robot.off", "seashell.off", "suzanne.off", "sword.off", "tetrahedron.obj"
  };

  int bpf = 2;
  char message[128] = "Demonstration du \nchiffrement selectif \n3D.";
  std::string decryptedMessage;
  decryptedMessage.reserve(128);

  double hausdorff_distance = 0.0;

  menu.callback_draw_viewer_window = [&](){
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(200, 460), ImGuiSetCond_Always);
    ImGui::Begin("Parameters", nullptr,ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);

    ImGui::PushItemWidth(120);

    {
      ImGui::InputTextMultiline("Message", message, IM_ARRAYSIZE(message));
      ImGui::Separator();
    }

    {
      ImGui::InputInt("Bits/float", &bpf, 0, 0, 1);
      ImGui::Separator();
    }

    {
      ImGui::Combo("Mesh", &item_current, items, IM_ARRAYSIZE(items));
      ImGui::Separator();
    }

    {
      if (ImGui::Button("Execute(Paillier)", ImVec2(-1, 0))) {
        // Clear decrypted message
        decryptedMessage = "";
        // Load mesh file
        std::string path = "../models/" + std::string(items[item_current]);
        std::string ext = path.substr(path.find_last_of('.') + 1, 3);
        if (ext == "obj") {
          igl::readOBJ(path, V, F);
        } else if (ext == "off") {
          igl::readOFF(path, V, F);
        }
        // Method
        encryptedV = Eigen::MatrixXd(V.rows(), V.cols());
        decryptedV = Eigen::MatrixXf(V.rows(), V.cols());
        method_paillier(bpf, paillier, std::string(message), decryptedMessage, V, encryptedV, decryptedV);
      }
      ImGui::Separator();
    }

    {
      if (ImGui::Button("Execute(Naive)", ImVec2(-1, 0))) {
        decryptedMessage = "";
        // Load mesh file
        std::string path = "../models/" + std::string(items[item_current]);
        std::string ext = path.substr(path.find_last_of('.') + 1, 3);
        if (ext == "obj") {
          igl::readOBJ(path, V, F);
        } else if (ext == "off") {
          igl::readOFF(path, V, F);
        }
        // Method
        Eigen::MatrixXf encryptedVf = Eigen::MatrixXf(V.rows(), V.cols());
        decryptedV = Eigen::MatrixXf(V.rows(), V.cols());
        method_naive(bpf, rsa, std::string(message), decryptedMessage, V, encryptedVf, decryptedV);
        encryptedV = encryptedVf.cast<double>();
      }
      ImGui::Separator();
    }

    {
      ImGui::PushItemWidth(56);
      if (ImGui::Button("Original")) {
        viewer.data().clear();
        viewer.data().set_mesh(V, F);
        viewer.core().align_camera_center(V, F);
        viewer.data().set_face_based(true);
      } ImGui::SameLine();

      if (ImGui::Button("Encrypted")) {
        viewer.data().clear();
        viewer.data().set_mesh(encryptedV, F);
        viewer.core().align_camera_center(encryptedV, F);
        viewer.data().set_face_based(true);
      } ImGui::SameLine();

      if (ImGui::Button("Decrypted")) {
        viewer.data().clear();
        viewer.data().set_mesh(decryptedV.cast<double>(), F);
        viewer.core().align_camera_center(decryptedV.cast<double>(), F);
        viewer.data().set_face_based(true);
      } ImGui::SameLine();

      ImGui::PopItemWidth();
    }

    {
      ImGui::Spacing();
      ImGui::Separator();
      ImGui::InputTextMultiline("Decrypted\nMessage", &decryptedMessage[0], decryptedMessage.size());
    }

    {
      if (ImGui::Button("Calc Distance Hausdorff", ImVec2(-1, 0))) {
        if (V.size() != 0 && decryptedV.size() != 0) {
          Eigen::MatrixXd v = decryptedV.cast<double>();
          igl::hausdorff(V, F, v, F, hausdorff_distance);
        }
      }
    }

    {
      ImGui::InputDouble("Hausdorff\nDistance ", &hausdorff_distance, 0, 0, "%.4f", ImGuiInputTextFlags_ReadOnly);
      ImGui::Separator();
    }

    ImGui::PopItemWidth();
    ImGui::End();
  };

  viewer.launch(true, false, "3D Encryption");

  return 0;
}

void method_paillier(int bpf, const encrypt3d::Paillier& paillier, const std::string& message, std::string& decryptedMessage,
                     const Eigen::MatrixXd& vertices, Eigen::MatrixXd& encrypted, Eigen::MatrixXf& decrypted)
{
  using namespace encrypt3d;

  if (bpf > 3) {
    std::cout << "Too many bits to insert" << std::endl;
    return;
  }

  std::cout << "Message : " << message << std::endl;
  Eigen::MatrixXf Vf = vertices.cast<float>();
  Encryption3D::insereMsgPaillier(message, bpf, Vf, encrypted, paillier);
  Encryption3D::retireMsgPaillier(decryptedMessage, message.size(), bpf, encrypted, decrypted, paillier);
  std::cout << "DecryptedMessage : " << decryptedMessage << std::endl;
}

void method_naive(int bpf, const encrypt3d::RSA& rsa, const std::string& message,
                  std::string& decryptedMessage, const Eigen::MatrixXd& vertices,
                  Eigen::MatrixXf& encrypted, Eigen::MatrixXf& decrypted)
{
  using namespace encrypt3d;
  std::cout << "Message : " << message << std::endl;
  Eigen::MatrixXf Vf = vertices.cast<float>();
  Encryption3D::insereMsgRSA(message, bpf, Vf, encrypted, rsa);

  for (int i = 0; i < Vf.size(); ++i) {
    decrypted(i) = Chiffre32::fromFloat(Vf(i)).insereMessageLSB(bpf, 0).toFloat();
  }

  decryptedMessage = message;

  std::cout << "DecryptedMessage : " << message << std::endl;
}