#ifndef _SCHEDULING_H_
#define _SCHEDULING_H_

#include "Twin.h"

class scheduler {

public:

    Twin t {"Lab 2 -- Scheduling"};

    int getInputFile(int, char*[]);
    int getSchedule();
    int printSchedule(int, char*[]);

private:

    int numTasks;
    int simTime;
    int utilization_RMS;
    int utilization_EDF;
    mat late_RMS;
    mat late_EDF;
    mat preempt_RMS;
    mat preempt_EDF;
    std::vector<char> id;
    vec execTime;
    vec period;
    vec queue_RMS;
    vec queue_EDF;
    std::vector<char> schedule_RMS;
    std::vector<char> schedule_EDF;
    int numTasks_ap;
    std::vector<char> id_ap;
    vec execTime_ap;
    vec release_ap;
    vec queue_id_RMS_ap;
    vec queue_id_EDF_ap;
    vec queue_RMS_ap;
    vec queue_EDF_ap;
    vec response_RMS_ap;
    vec response_id_RMS_ap;
    vec response_EDF_ap;
    vec response_id_EDF_ap;
    
};

#endif