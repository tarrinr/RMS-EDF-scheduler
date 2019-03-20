#include "scheduling.h"

int main(int argc, char* argv[]) {

    scheduler s;

    // Get input file parameters
    char stop = 'n';
    do {
        while (s.getInputFile(argc, argv)) {};
        s.t.println();
        s.t.println("Correct? [y/n]");
        s.t.println("If entries are not correct, fix file formatting and restart.");
        s.t.getInput(stop);
    } while (stop != 'y' && stop != 'Y');

    // Generate schedules
    if (s.getSchedule())
        // Print summary and schedule file
        s.printSchedule(argc, argv);

    else {
        s.t.println("ERROR: schedule not created.");
        s.t.getInput();
    }

    return EXIT_SUCCESS;
}