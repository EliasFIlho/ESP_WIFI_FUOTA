#include <stdio.h>
#include "ota_tcp_server.h"

void ota_server(void *pvParameters)
{

    int socketfd;
    int portnum = 8000;
    struct sockaddr_in server_add;
    server_add.sin_addr.s_addr = htonl(INADDR_ANY);
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(portnum);

    char recv_buffer[2048];

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    {
        ESP_LOGE("OTA", "ERROR TO CREATE SOCKET");
        close(socketfd);
        abort();
    }

    if (bind(socketfd, (struct sockaddr *)&server_add, sizeof(server_add)) < 0)
    {
        ESP_LOGE("OTA", "ERROR TO BIND SOCKET");
        close(socketfd);
        abort();
    }

    listen(socketfd, 1);

    int client_socket;

    //client_socket = accept(client_socket);

}

void fuota_tcp_server_init()
{
    xTaskCreate(ota_server, "OTA Monitor", configMINIMAL_STACK_SIZE + 4096, NULL, tskIDLE_PRIORITY + 1, NULL);
}
