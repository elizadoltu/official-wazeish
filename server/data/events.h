#pragma once 

#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <fstream>
#include <json-c/json.h>
#include <string>
#include <vector>
#include <algorithm>

extern void handleEventReports(int client_socket, const std::string& data);

#endif