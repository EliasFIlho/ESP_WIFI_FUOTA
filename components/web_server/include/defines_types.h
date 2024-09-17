#pragma once

#define CONFIG_TOKENS_NUMBER 9
#define CONFIG_TOKENS_SIZE 150

#define DEVICE_NAME_BUFFER 16
#define DEVICE_IP_BUFFER 16

#define SERVER_IP_BUFFER DEVICE_IP_BUFFER

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define EQUAL(X, Y) (((X) == (Y)) ? (0) : (1))

typedef enum {
    DISABLE =0,
    ON_OFF,
    PULSE_COUNTER
}io_set_t;


typedef struct {
    char device_name[DEVICE_NAME_BUFFER];
    char device_ip[DEVICE_IP_BUFFER];
    char server_ip[SERVER_IP_BUFFER];
    int port;
    int sample_time;
    io_set_t io_01;
    io_set_t io_02;
    io_set_t io_03;
    io_set_t io_04;
}device_cfg_t;