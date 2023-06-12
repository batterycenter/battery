
#include "App.hpp"

void MainWindow::onAttach() {
    this->create("Battery MazeSolver Example");
    mazeHost.init(this->getSize());
}

void MainWindow::onUpdate() {
    mazeHost.update();
    mazeHost.render(*this);
}

void MainWindow::onDetach() {

}
