// p3_satq.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "structs.h"

/* ---------- allocation helpers ---------- */

static Queue *make_queue() {
    Queue *q = malloc(sizeof(Queue));
    if (q == NULL) {
        return NULL;
    }
    q->head = NULL;
    q->count = 0;
    return q;
}


static MissionControl *mc_init() {
    MissionControl *mc = malloc(sizeof(MissionControl));
    if (mc == NULL) {
        return NULL;
    }

    mc->nominal_queue = make_queue();
    mc->safe_queue = make_queue();
    mc->downlink_queue = make_queue();
    mc->mission_time = 0;

    // if any of the queues failed to allocate, free everything and return NULL
    if (mc->nominal_queue == NULL || mc->safe_queue == NULL || mc->downlink_queue == NULL) {
        free(mc->nominal_queue);
        free(mc->safe_queue);
        free(mc->downlink_queue);
        free(mc);
        return NULL;
    }

    return mc;
}

/*
    TODO:
    - free an individual satellite
*/
static void free_satellite(Satellite *sat) {
    free(sat->name);
    free(sat);
    return;
}

/*
    TODO:
    - free all nodes in the queue
*/
static void free_queue_nodes(Queue *q) {
    Satellite* cur = q->head;
    while (cur != NULL){
        Satellite* t = cur->next;
        free_satellite(cur);
        cur = t; 
    }
    return;
}

// Frees all memory associated with the mission control struct
static void mc_free(MissionControl *mc) {
    if (mc == NULL) {
        return;
    }

    if (mc->nominal_queue != NULL) {
        free_queue_nodes(mc->nominal_queue);
        free(mc->nominal_queue);
    }
    if (mc->safe_queue != NULL) {
        free_queue_nodes(mc->safe_queue);
        free(mc->safe_queue);
    }
    if (mc->downlink_queue != NULL) {
        free_queue_nodes(mc->downlink_queue);
        free(mc->downlink_queue);
    }

    free(mc);
}

/* ---------- queue helpers ---------- */


static int insert_sorted_by_sid(Queue *q, Satellite *sat) {
    Satellite *prev = NULL;
    Satellite *cur = q->head;
    if (!cur){
        q->head = sat;
        q->count = 1;
        return 0;
    }

    while(cur != NULL && cur->sid < sat->sid){
        prev = cur;
        cur = cur->next;
    }

    
    q->count++;
    if(!prev){
        q->head = sat;
        sat->next = cur;
        return 0;
    }

    if(!cur){
        prev->next = sat;
        return 0;
    }

    sat->next = cur;
    prev->next = sat;

    return 0;
}


static int insert_fifo(Queue *q, Satellite *sat) {
    Satellite *cur = q->head;
    if (!cur){
        q->head = sat;
        q->count = 1;
        return 0;
    }

    while (cur->next != NULL){
        cur = cur->next;
    }
    cur->next = sat;
    q->count++;
    return 0;
}


static Satellite *remove_by_sid(Queue *q, int sid) {
    Satellite *prev = NULL;
    Satellite *cur = q->head;

    if (q == NULL) {
        return NULL;
    }

    while (cur != NULL && cur->sid != sid){
        prev = cur;
        cur = cur->next;
    }

    if (!cur){
        return NULL;
    }

    q->count--;
    if (!prev){
        q->head = cur->next;
        return cur;
    }

    prev->next = cur->next;

    cur->next = NULL;
    return cur;

}


static Satellite *pop_front(Queue *q) {
    Satellite *sat;

    if (q == NULL || q->head == NULL) {
        return NULL;
    }

    sat = q->head;
    q->head = sat->next;
    return sat;
}


static int sid_exists_in_queue(Queue *q, int sid) {
    Satellite *cur = q->head;
    while (cur != NULL){
        if (cur->sid == sid){
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

static int sid_exists_anywhere(MissionControl *mc, int sid) {
    if (mc == NULL) {
        return 0;
    }

    return sid_exists_in_queue(mc->nominal_queue, sid) ||
           sid_exists_in_queue(mc->safe_queue, sid) ||
           sid_exists_in_queue(mc->downlink_queue, sid);
}

/* ---------- satellite creation ----------- */

static Satellite *sat_generate(const char *name, int sid, int work, int mission_time) {
    Satellite *sat;
    int len;

    if (name == NULL || *name == '\0') {
        return NULL;
    }

    // allocate memory for the satellite struct
    sat = malloc(sizeof(Satellite));
    if (sat == NULL) {
        return NULL;
    }

    len = strlen(name);
    // we allocate memory for the satellite's name because in the struct, we have a pointer to a char for the name. We need actual memory for that char pointer to point to
    sat->name = malloc(len + 1); 
    if (sat->name == NULL) {
        free(sat);
        return NULL;
    }

    // memcpy (or we could have also used strncpy) to copy the name into the allocated memory for the satellite's name
    memcpy(sat->name, name, len + 1);

    sat->sid = sid;
    sat->work_remaining = work;
    sat->launch_time = mission_time;
    sat->wait_remaining = 0;
    sat->next = NULL;

    return sat;
}

/* ---------- main operations ---------- */

/*
    This function adds a satellite to the nominal queue, sorted by sid
     - if the mission control or satellite is NULL, return -1
     - otherwise, return 0 (returning insert_sorted_by_sid(..) returns 0 on success)
*/
static int mc_add_nominal(MissionControl *mc, Satellite *sat) {
    if (mc == NULL || sat == NULL) {
        return -1;
    }
    return insert_sorted_by_sid(mc->nominal_queue, sat);
}

/*
    Similar to the above function, except it adds the satellite to the safe queue instead of the nominal queue
     - if the mission control or satellite is NULL, return -1
     - otherwise, return 0 (returning insert_sorted_by_sid(..) also returns 0 on success)
     - this function also removes the satellite from the nominal queue, so if the satellite with the given sid does not exist in the nominal queue, return -1
*/
static int mc_move_safe(MissionControl *mc, int sid) {
    Satellite *sat;

    if (mc == NULL) {
        return -1;
    }

    sat = remove_by_sid(mc->nominal_queue, sid);
    if (sat == NULL) {
        return -1;
    }

    return insert_sorted_by_sid(mc->safe_queue, sat);
}

/*
    This function moves a satellite from the safe queue back to the nominal queue. In more detail:
    - if the mission control is NULL, return -1
    - remove the satellite with the given sid from the safe queue, and if no such satellite exists, return -1
    - if the satellite is successfully removed from the safe queue, insert it back into the nominal queue, and return 0
*/
static int mc_move_recover(MissionControl *mc, int sid) {
    Satellite *sat;

    if (mc == NULL) {
        return -1;
    }

    sat = remove_by_sid(mc->safe_queue, sid);
    if (sat == NULL) {
        return -1;
    }

    return insert_sorted_by_sid(mc->nominal_queue, sat);
}

/*
    This function moves a satellite from the nominal queue to the downlink queue, and sets its wait_remaining to the given wait_time
     - if the satellite with the given sid does not exist in the nominal queue, return -1
     - if the wait_time is not positive, return -1
     - otherwise, return 0
*/
static int mc_move_downlink(MissionControl *mc, int sid, int wait_time) {
    Satellite *sat;

    // if the mission control is NULL or the wait time is not positive, return -1
    if (mc == NULL || wait_time <= 0) {
        return -1;
    }

    // remove the satellite with the given sid from the nominal queue
    sat = remove_by_sid(mc->nominal_queue, sid);
    if (sat == NULL) {
        return -1;
    }

    // set wait_remaining equal to the specified wait_time, and insert the satellite into the downlink queue
    sat->wait_remaining = wait_time;
    return insert_fifo(mc->downlink_queue, sat);
}

/*
    This function simulates the passage of one unit of time, and should be called whenever the "tick" command is issued.
    - increment the mission time
    - if there is a satellite in the downlink queue, decrement its wait_remaining
    - if wait_remaining reaches 0, move it back to the nominal queue
*/
static int mc_tick_downlink(MissionControl *mc) {
    Satellite *sat;
    if (mc == NULL) {
        return -1;
    }
    mc->mission_time--;
    sat = mc->downlink_queue->head;

    if (!--(sat->wait_remaining)){
        insert_sorted_by_sid(mc->nominal_queue, pop_front(mc->downlink_queue));
    }

    return 0;
}


static Satellite *mc_select_dispatch(MissionControl *mc) {

    if (!mc->nominal_queue->head){
        fprintf(stderr, "error: %s\n", "Nominal queue is empty");
        return NULL;
    }
    Satellite* cur = mc->nominal_queue->head;
    Satellite* min = cur;
    while (cur != NULL){
        if (cur->work_remaining < min->work_remaining){
            min = cur;
        }
        if (cur->work_remaining == min->work_remaining && cur->sid < min->sid){
            min = cur;
        }

        cur = cur->next;
        
    }


    return min;
}

/* ---------- printing helpers ---------- */

// prints each 
static void print_satellite(const Satellite *sat) {
    printf("SID=%d NAME=%s WORK=%d WAIT=%d LAUNCH=%d\n", sat->sid, sat->name, sat->work_remaining, sat->wait_remaining, sat->launch_time);
}

/*
    This function:
    - iterates through the queue and prints each satellite using print_satellite
    - if the queue is NULL, it should just print the label (name of the queue) and return
*/
static void list_queue(const char *label, const Queue *q) {
    const Satellite *cur;

    printf("%s\n", label);

    if (q == NULL) {
        return;
    }

    cur = q->head;
    while (cur != NULL) {
        print_satellite(cur);
        cur = cur->next;
    }
}

/* ---------- parser helper ---------- */
static int tokenize(char *line, char **argv, int max_tokens) {
    int argc = 0;
    char *tok = strtok(line, " \t\r\n");

    while (tok != NULL && argc < max_tokens - 1) {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t\r\n");
    }

    argv[argc] = NULL;
    return argc;
}

/* ---------- main ---------- */
int main(void) {
    MissionControl *mc = mc_init();
    char line[LINE_LEN];
    char *argv[16];
    int argc;

    if (mc == NULL) {
        fprintf(stderr, "error: %s\n", "allocation failure. Zeus isn't having it today");
        return 1;
    }

    /*
        Main simulation loop:
        - print the prompt "MissionControl>"
        - read a line of input from the user
        - parse the line into a command and arguments
        - execute the command, and print relevant output/error messages

    */
    while (1) {
        printf("MissionControl>");
        fflush(stdout); // This just ensures the prompt prints before waiting for input

        if (fgets(line, sizeof(line), stdin) == NULL) {
            mc_free(mc);
            return 0;
        }

        argc = tokenize(line, argv, 16);

        if (argc == 0) {
            continue;
        }

        if (strcmp(argv[0], "quit") == 0) {
            mc_free(mc);
            return 0;

        // if help is entered, print the list of commands and their expected arguments
        } else if (strcmp(argv[0], "help") == 0) {
            printf("help\n");
            printf("launch NAME SID WORK\n");
            printf("count QUEUE\n");
            printf("list [all|nominal|safe|downlink]\n");
            printf("dispatch\n");
            printf("safe SID\n");
            printf("recover SID\n");
            printf("downlink SID WAIT\n");
            printf("tick\n");
            printf("quit\n");

        /*
            If command is launch, then create a new satellite with the
            given name, sid, and work, and add it to the nominal queue.
            Note: the satellite's launch_time is
            initialized to the current mission time, and wait_remaining
            should be initialized to 0. Also, if a satellite with
            the given sid already exists in any of the queues, print
            an error message and do not launch a new satellite.
        */
        } else if (strcmp(argv[0], "launch") == 0) {
            Satellite *sat;
            int sid, work;

            if (argc != 4) {
                fprintf(stderr, "error: %s\n", "incorrect number of arguments for launch");
                continue;
            }
            
            char* name = argv[1];
            sid = atoi(argv[2]);
            work = atoi(argv[3]);

            if (sid < 0){
                fprintf(stderr, "error: %s\n", "sid must be postive");
                continue;
            }

            if (sid_exists_anywhere(mc, sid)){
                fprintf(stderr, "error: %s\n", "sid already exists");
                continue;
            }

            if (work < 0) {
                fprintf(stderr, "error: %s\n", "work must be postive");
                continue;
            }

            sat = sat_generate(name, sid, work, 0);

            if (!sat) {
                fprintf(stderr, "error: %s\n", "satellite failed to generate");
                continue;
            }

            mc_add_nominal(mc, sat);


        } else if (strcmp(argv[0], "count") == 0) { // if command is "count", then print the count of the specified queue
            if (argc != 2) {
                fprintf(stderr, "error: %s\n", "incorrect number of arguments");
            } else if (strcmp(argv[1], "nominal") == 0) {
                printf("nominal: %d\n", mc->nominal_queue->count);
            } else if (strcmp(argv[1], "safe") == 0) {
                printf("safe: %d\n", mc->safe_queue->count);
            } else if (strcmp(argv[1], "downlink") == 0) {
                printf("downlink: %d\n", mc->downlink_queue->count);
            } else {
                fprintf(stderr, "error: %s\n", "Invalid queue name");
            }

        } else if (strcmp(argv[0], "list") == 0) {
            if (argc == 1 || (argc == 2 && strcmp(argv[1], "all") == 0)) {
                list_queue("nominal", mc->nominal_queue);
                list_queue("safe", mc->safe_queue);
                list_queue("downlink", mc->downlink_queue);
            } else if (argc == 2 && strcmp(argv[1], "nominal") == 0) {
                list_queue("nominal", mc->nominal_queue);
            } else if (argc == 2 && strcmp(argv[1], "safe") == 0) {
                list_queue("safe", mc->safe_queue);
            } else if (argc == 2 && strcmp(argv[1], "downlink") == 0) {
                list_queue("downlink", mc->downlink_queue);
            } else {
                fprintf(stderr, "error: %s\n", "Invalid queue name");
            }

        } else if (strcmp(argv[0], "safe") == 0) { // if command entered is "safe"
            if (argc != 2 || mc_move_safe(mc, atoi(argv[1])) != 0) {
                fprintf(stderr, "error: %s\n", "Id does not exist or incorrect argument count");
                // add specified error messages
            }

        } else if (strcmp(argv[0], "recover") == 0) { // if command is "recover"
            if (argc != 2 || mc_move_recover(mc, atoi(argv[1])) != 0) {
                fprintf(stderr, "error: %s\n", "Id does not exist or incorrect argument count");
            }

        } else if (strcmp(argv[0], "downlink") == 0) { // if command is "downlink"
            if (argc != 3 || mc_move_downlink(mc, atoi(argv[1]), atoi(argv[2])) != 0) {
                fprintf(stderr, "error: %s\n", "invalid downlink request");
            }

        } else if (strcmp(argv[0], "tick") == 0) { // if the command is "tick"
            mc_tick_downlink(mc);

        } else if (strcmp(argv[0], "dispatch") == 0) { // if the command is "dispatch"
            Satellite *sat = mc_select_dispatch(mc);


            if (sat == NULL) {
                fprintf(stderr, "error: %s\n", "dispatch not implemented yet");
            }
            else {
                printf("Dispatching satellite:\n");
                remove_by_sid(mc->nominal_queue, sat->sid);
                sat->next = NULL;
                mc->mission_time++;
                if (!--sat->work_remaining){
                    printf("Satellite %s, sid: %d has completed\n", sat->name, sat->sid);
                    free_satellite(sat);
                }else{
                    insert_sorted_by_sid(mc->nominal_queue, sat);
                }
            }
            

        } else {
            fprintf(stderr, "error: %s\n", "unknown command");
        }
    }
}