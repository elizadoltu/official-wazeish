#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H 

void *handleClientDataThread(void *arg);
void *handleAccidentsReports(void *arg);
void *reportGasPrices(void *arg);
void *updateTheWeather(void *arg);
void *handleEventsReports(void *arg);
void *handleRenderCity(void *arg);
void *handleClientData(void *arg);

#endif