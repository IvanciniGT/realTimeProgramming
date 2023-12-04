//
// Created by Ivan Osuna Ayuste on 27/11/23.
//

#ifndef CARDDETECTOR_HEARTBEAT_H
#define CARDDETECTOR_HEARTBEAT_H

#include <sys/types.h>

void configure_heartbeat_thread(pid_t parentProcess);
void join_heartbeat_thread();

#endif //CARDDETECTOR_HEARTBEAT_H
