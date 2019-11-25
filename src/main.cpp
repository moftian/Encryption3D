#include <igl/readOFF.h>
#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>

Eigen::MatrixXd vertices;
Eigen::MatrixXi faces;

int main(int argc, char** argv)
{
    igl::readOBJ("../models/suzanne.obj", vertices, faces);
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(vertices, faces);
    viewer.launch();
}