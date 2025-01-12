#pragma once 

#ifndef HANDLE_ACCIDENTS_REPORTS
#define HANDLE_ACCIDENTS_REPORTS

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h> 

extern void reportAccidents(int client_socket);

#endif 