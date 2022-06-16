#include <thread>
#include "GUI.h"
#include "Solver.h"

int main(int, char **)
{
    std::jthread solver_thread(Solver::main);
    Solver::set_delay(100);
    GUI::init();

    GUI::loop();

    GUI::cleanup();
    Solver::set_delay(0);
	return 0;
}
