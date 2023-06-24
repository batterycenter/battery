
#include "App.hpp"

void MainWindow::onAttach() {
    this->create("Battery MazeSolver Example");
    mazeHost.init(this);
}

void MainWindow::onUpdate() {
    mazeHost.update();
    mazeHost.render();
}

void MainWindow::onDetach() {

}
