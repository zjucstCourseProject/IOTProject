#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"

#define ADDRESS "tcp://localhost:1883"
#define TOPIC1 "command_to"
#define TOPIC2 "info_return"

int send_command(char *command, char *topic, char *client_id) {
    char buf[1024];
    const int time_out = 10000000;
    int rv;
    int QOS = 1;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message publish_msg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;

    rv = MQTTClient_create(&client, ADDRESS, client_id, MQTTCLIENT_PERSISTENCE_NONE, nullptr);
    
    rv = MQTTClient_connect(client, &conn_opts);
    publish_msg.qos = QOS;
    publish_msg.retained = 0;
    publish_msg.payload = (void *)command;
    publish_msg.payloadlen = strlen(command);
    rv = MQTTClient_publishMessage(client, topic, &publish_msg, &token);
    rv = MQTTClient_waitForCompletion(client, token, time_out);
    MQTTClient_disconnect(client, 100000);
    MQTTClient_destroy(&client);
}

void recv_command(char *message, char *topic, char *client_id) {
    char *payload = topic;
    int rv, i;
    char *ptr = nullptr;
    char *cli_topic = nullptr;
    int cli_topic_len;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_deliveryToken token;
    MQTTClient_message *receive_msg = nullptr;
    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;

    rv = MQTTClient_create(&client, ADDRESS, client_id, MQTTCLIENT_PERSISTENCE_NONE, nullptr);
    rv = MQTTClient_connect(client, &conn_opts);
        
    MQTTClient_subscribe(client, payload, 1);
    rv = MQTTClient_receive(client, &cli_topic, &cli_topic_len, &receive_msg, 100000);
    ptr = (char *)receive_msg->payload;
    for (i = 0; i < receive_msg->payloadlen; i++)
        message[i] = *ptr++;
    message[i] = '\0';
    MQTTClient_unsubscribe(client, payload);
    MQTTClient_disconnect(client, 100000);
    MQTTClient_destroy(&client);
}