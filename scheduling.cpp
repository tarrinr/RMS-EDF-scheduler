#include "scheduling.h"

#define ELSE_FIN_FAIL else { t.println(); t.println("ERROR: invalid input file."); t.println("Fix file formatting and restart."); t.println(); t.getInput(); return 1; }

// Get input file
int scheduler::getInputFile(int argc, char* argv[]) {

    // Reinitialize after error
    id.clear();
    id_ap.clear();
    execTime.clear();
    execTime_ap.clear();
    period.clear();
    release_ap.clear();

    std::string inputFile;
    t.println("Drag and drop or enter the path to the input file.");

    // Use file names passed in from command line
    if (argc >= 2) {
        t.println();
        t.println("It looks like you passed in an input file name.");
        t.println("Enter 'y' to use '");
        t.print(argv[1]);
        t.print("'.");
        if (argc >=3) {
            t.println();
            t.println("The passed in output file name '");
            t.print(argv[2]);
            t.print("' will also be used.");
        }
    }

    t.getInput(inputFile);

    if (argc >= 2 && inputFile == "y")
        inputFile = argv[1];

    t.loading(0, false);

    std::ifstream fin(inputFile);

    // Get number of periodic tasks
    fin >> numTasks;
    if (fin.good()) 
        t.loading(10, false);
    ELSE_FIN_FAIL

    // Get simulation time
    fin >> simTime;
    if (fin.good())
        t.loading(20, false);
    ELSE_FIN_FAIL

    std::string temp;

    // Display simulation time
    t.println("Simulation time: ");
    t.print(simTime);
    t.println();

    // Get periodic tasks and display
    t.println("PERIODIC TASKS");
    t.println("Total: ");
    t.print(numTasks);
    t.println("i: <id> <execTime> <period>");
    
    for (int i = 0; i < numTasks; i++) {

        fin >> temp;
        if (fin.good())
            t.loading(20 + i * 80/numTasks, false);
        ELSE_FIN_FAIL

        temp.pop_back();
        if (temp.length() > 0)
            id.push_back(temp[0]);
        ELSE_FIN_FAIL

        fin >> temp;
        if (fin.good()) {}
        ELSE_FIN_FAIL

        temp.pop_back();
        if (temp.length() > 0)
            execTime.push_back(stoi(temp));
        ELSE_FIN_FAIL

        fin >> temp;
        if (!fin.fail()) {}
        ELSE_FIN_FAIL

        if (temp.length() > 0)
            period.push_back(stoi(temp));
        ELSE_FIN_FAIL

        t.println(i + 1);
        t.print(": ");
        t.print(id[i]);
        t.print(" ");
        t.print(execTime[i]);
        t.print(" ");
        t.print(period[i]);

    }

    // Get number of aperiodic tasks
    fin >> numTasks_ap;
    if (fin.good()) 
        t.loading(20, false);
    else
        return 0;

    // Get aperiodic tasks and display
    t.println();
    t.println("APERIODIC TASKS");
    t.println("Total: ");
    t.print(numTasks_ap);
    t.println("i: <id> <execTime> <release>");

    for (int i = 0; i < numTasks_ap; i++) {

        fin >> temp;
        if (fin.good())
            t.loading(20 + i * 80/numTasks, false);
        ELSE_FIN_FAIL

        temp.pop_back();
        if (temp.length() > 0)
            id_ap.push_back(temp[0]);
        ELSE_FIN_FAIL

        fin >> temp;
        if (fin.good()) {}
        ELSE_FIN_FAIL

        temp.pop_back();
        if (temp.length() > 0)
            execTime_ap.push_back(stoi(temp));
        ELSE_FIN_FAIL

        fin >> temp;
        if (!fin.fail()) {}
        ELSE_FIN_FAIL

        if (temp.length() > 0)
            release_ap.push_back(stoi(temp));
        ELSE_FIN_FAIL

        t.println(i + 1);
        t.print(": ");
        t.print(id_ap[i]);
        t.print(" ");
        t.print(execTime_ap[i]);
        t.print(" ");
        t.print(release_ap[i]);

    }

    fin.close();
    t.loading(100, true);

    return 0;
}

int scheduler::getSchedule() {

    // Initialization of vectors and variables
    for (int i = 0; i < numTasks; i++)
        queue_RMS.push_back(0);
    queue_EDF = queue_RMS;

    for (int i = 0; i < numTasks; i++)
        late_RMS.push_back(vec());
    late_EDF = late_RMS;
    preempt_RMS = late_RMS;
    preempt_EDF = late_RMS;

    utilization_RMS = 0;
    utilization_EDF = 0;

    // Main loop going through each time slot
    for (int i = 0; i < simTime; i++) {

        bool done = 0;
        int shortest_EDF = simTime;
        int priority_EDF = -1;

        // Update aperiodic task queue
        for (int j = 0; j < numTasks_ap; j++) {
            if (i == release_ap[j]) {
                queue_id_RMS_ap.push_back(j);
                queue_id_EDF_ap.push_back(j);
                queue_RMS_ap.push_back(execTime_ap[j]);
                queue_EDF_ap.push_back(execTime_ap[j]);
            }
        }

        for(int j = 0; j < numTasks; j++) {
            
            if (i % period[j] == 0) {

                // Update RMS queue and record RMS late
                if (queue_RMS[j] != 0)
                    late_RMS[j].push_back(i);
                queue_RMS[j] += execTime[j];

                // Update EDF queue and record EDF late
                if (queue_EDF[j] != 0)
                    late_EDF[j].push_back(i);
                queue_EDF[j] += execTime[j];

            }

            // Pick RMS task for this time slot
            if (queue_RMS[j] != 0) {
                if (done == 0) {
                    schedule_RMS.push_back(id[j]);
                    queue_RMS[j]--;
                    done = 1;
                }
                // Record RMS preempts
                else
                    preempt_RMS[j].push_back(i);
            }

            // Find highest priority EDF task
            int priority = period[j] - (i % period[j]);
            if (priority < shortest_EDF && queue_EDF[j] != 0) {
                shortest_EDF = priority;
                priority_EDF = j;
            }

        }

        // Pick RMS aperiodic task if available 
        if (done == 0) 
            if (queue_RMS_ap.size() != 0) {
                schedule_RMS.push_back(id_ap[queue_id_RMS_ap[0]]);
                queue_RMS_ap[0]--;

                // RMS aperiodic task done
                if (queue_RMS_ap[0] == 0) {
                    response_id_RMS_ap.push_back(queue_id_RMS_ap[0]);
                    response_RMS_ap.push_back(i + 1);
                    queue_RMS_ap.erase(queue_RMS_ap.begin());
                    queue_id_RMS_ap.erase(queue_id_RMS_ap.begin());
                }
            }
            // RMS slack
            else {
                schedule_RMS.push_back(' ');
                utilization_RMS++;
            }

        // Schedule the highest priority EDF task
        if (shortest_EDF != simTime) {
            schedule_EDF.push_back(id[priority_EDF]);
            queue_EDF[priority_EDF]--;
        }
        // Pick EDF aperiodic task if available
        else if (queue_EDF_ap.size() != 0) {
                schedule_EDF.push_back(id_ap[queue_id_EDF_ap[0]]);
                queue_EDF_ap[0]--;
                
                // EDF aperiodic task done
                if (queue_EDF_ap[0] == 0) {
                    response_id_EDF_ap.push_back(queue_id_EDF_ap[0]);
                    response_EDF_ap.push_back(i + 1);
                    queue_EDF_ap.erase(queue_EDF_ap.begin());
                    queue_id_EDF_ap.erase(queue_id_EDF_ap.begin());
                }
        }
        // EDF slack
        else {
            schedule_EDF.push_back(' ');
            utilization_EDF++;
        }

        // Record EDF preempts
        for (int j = 0; j < numTasks; j++)
            if (j != priority_EDF && queue_EDF[j] != 0)
                preempt_EDF[j].push_back(i);

        if (i % 100 == 0)
            t.loading(i * 100/simTime, false);

    }

    t.loading(100, true);

    return 1;
}

int scheduler::printSchedule(int argc, char* argv[]) {

    // Print RMS periodic summary
    t.println("RMS PERIODIC SUMMARY (1 of 4)");

    int total_late_RMS = 0;
    int total_preempt_RMS = 0;
    
    for (int i = 0; i < numTasks; i++) {
        t.println();
        t.println("Task: ");
        t.print(id[i]);
        t.println("Missed deadlines: ");
        t.print(late_RMS[i].size());
        total_late_RMS += late_RMS[i].size();
        t.println("Preempted cycles: ");
        t.print(preempt_RMS[i].size());
        total_preempt_RMS += preempt_RMS[i].size();
    }

    t.println();
    t.println("Total:");
    t.println("Missed deadlines: ");
    t.print(total_late_RMS);
    t.println("Preempted cycles: ");
    t.print(total_preempt_RMS);

    t.println();
    t.getInput();

    // Print EDF periodic summary
    t.println("EDF PERIODIC SUMMARY (2 of 4)");

    int total_late_EDF = 0;
    int total_preempt_EDF = 0;
    
    for (int i = 0; i < numTasks; i++) {
        t.println();
        t.println("Task: ");
        t.print(id[i]);
        t.println("Missed deadlines: ");
        t.print(late_EDF[i].size());
        total_late_EDF += late_EDF[i].size();
        t.println("Preempted cycles: ");
        t.print(preempt_EDF[i].size());
        total_preempt_EDF += preempt_EDF[i].size();
    }

    t.println();
    t.println("Total:");
    t.println("Missed deadlines: ");
    t.print(total_late_EDF);
    t.println("Preempted cycles: ");
    t.print(total_preempt_EDF);

    t.println();
    t.getInput();

    // Print RMS aperiodic summary
    t.println("RMS APERIODIC SUMMARY (3 of 4)");

    double average_response = 0;
    for (int i = 0; i < response_RMS_ap.size(); i++) {
        t.println();
        t.println("Task: ");
        t.print(id_ap[response_id_RMS_ap[i]]);
        t.println("Release time: ");
        t.print(release_ap[response_id_RMS_ap[i]]);
        t.println("Completed time: ");
        t.print(response_RMS_ap[i]);
        t.println("Response time: ");
        t.print(response_RMS_ap[i] - release_ap[response_id_RMS_ap[i]]);
        average_response += (response_RMS_ap[i] - release_ap[response_id_RMS_ap[i]]) / response_RMS_ap.size();
    }

    t.println();
    t.println("Average response time: ");
    t.print(average_response);

    t.println("Utilization: ");
    t.print((1-(double)utilization_RMS/simTime) * 100);
    t.print("%");

    t.println();
    t.getInput();

    // Print EDF aperiodic summary
    t.println("EDF APERIODIC SUMMARY (4 of 4)");

    average_response = 0;
    for (int i = 0; i < response_EDF_ap.size(); i++) {
        t.println();
        t.println("Task: ");
        t.print(id_ap[response_id_EDF_ap[i]]);
        t.println("Release time: ");
        t.print(release_ap[response_id_EDF_ap[i]]);
        t.println("Completed time: ");
        t.print(response_EDF_ap[i]);
        t.println("Response time: ");
        t.print(response_EDF_ap[i] - release_ap[response_id_EDF_ap[i]]);
        average_response += (response_EDF_ap[i] - release_ap[response_id_EDF_ap[i]]) / response_EDF_ap.size();
    }

    t.println();
    t.println("Average response time: ");
    t.print(average_response);

    t.println("Utilization: ");
    t.print((1-(double)utilization_EDF/simTime) * 100);
    t.print("%");

    t.println();
    t.getInput();

    // Print schedules to file
    t.println("      RMS EDF   (!!) differences between schedules;  (Xm) task X missed a deadline;  (Xp) task X is being preempted;");

    for (int i = 0; i < simTime; i++) {

        t.println(i);
        if (i < 10) t.print(" ");
        if (i < 100) t.print(" ");
        t.print("  | ");
        t.print(schedule_RMS[i]);
        t.print(" | ");
        t.print(schedule_EDF[i]);
        t.print(" | ");
        t.print(" ");

        if (schedule_RMS[i] != schedule_EDF[i])
            t.print("!!; ");

        for (int j = 0; j < numTasks; j++) {
            if(!late_RMS[j].empty())
                if (late_RMS[j][0] == i) {
                    t.print(id[j]);
                    t.print("m_RMS; ");
                    late_RMS[j].erase(late_RMS[j].begin());
                }
            if(!late_EDF[j].empty())
                if (late_EDF[j][0] == i) {
                    t.print(id[j]);
                    t.print("m_EDF; ");
                    late_EDF[j].erase(late_EDF[j].begin());
                }
            if (!preempt_RMS[j].empty())
                if (preempt_RMS[j][0] == i) {
                    t.print(id[j]);
                    t.print("p_RMS; ");
                    preempt_RMS[j].erase(preempt_RMS[j].begin());
                }
            if(!preempt_EDF[j].empty())
                if (preempt_EDF[j][0] == i) {
                    t.print(id[j]);
                    t.print("p_EDF; ");
                    preempt_EDF[j].erase(preempt_EDF[j].begin());
                }
        }
    }

    if (argc >= 3)
        while (t.toFile(argv[2])) {}

    else 
        while (t.toFile("schedule.txt")) {}

    return 1;
}
