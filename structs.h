// structs.h
#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct satellite_struct {
    char *name;
    int sid;
    int work_remaining; // the amount of work remaining until the satellite can be moved 
    int launch_time; // the last time this satellite was launched
    int wait_remaining;
    struct satellite_struct *next;
} Satellite;

typedef struct queue_struct {
    Satellite *head;
    int count;
} Queue;

typedef struct control_struct {
    Queue *nominal_queue;
    Queue *safe_queue;
    Queue *downlink_queue;
    int mission_time;
} MissionControl;

#endif