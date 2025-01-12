#pragma once 

#ifndef HANDLE_EVENTS
#define HANDLE_EVENTS

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h> 

extern void reportEvent(int client_socket);

#endif