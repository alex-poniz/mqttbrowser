#include "mqttclient.h"
#include <iostream>

#define LOG(debug) std::cout

namespace Mqtt {

static bool __is_mosquitto_initialized = false;

static inline IMqttCallbacks* __get_delegate(void* userdata) {
  class MQTTClient* mqtt_inst = (class MQTTClient*)userdata;
  return  mqtt_inst ? mqtt_inst->getDelegate() : nullptr;
}

static void __on_publish(struct mosquitto* /*msqt*/, void* userdata, int mid) {
  LOG(debug) << __PRETTY_FUNCTION__;

  IMqttCallbacks* delegate = __get_delegate(userdata);
  if (delegate) {
    delegate->on_publish(mid);
  }
}

static void __on_subscribed(struct mosquitto* /*msqt*/,
                            void* userdata,
                            int mid,
                            int qos_count,
                            const int* granted_qos) {
  LOG(debug) << __PRETTY_FUNCTION__;

  IMqttCallbacks* delegate = __get_delegate(userdata);
  if (delegate) {
    delegate->on_subscribed(mid, qos_count, granted_qos);
  }
}

static void __on_unsubscribed(struct mosquitto* /*msqt*/, void* userdata, int mid) {
  LOG(debug) << __PRETTY_FUNCTION__;

  IMqttCallbacks* delegate = __get_delegate(userdata);
  if (delegate) {
    delegate->on_unsubscribed(mid);
  }
}

static void __on_message_received(struct mosquitto* /*msqt*/, void* userdata, const struct mosquitto_message* msg) {
  LOG(debug) << __PRETTY_FUNCTION__;

  IMqttCallbacks* delegate = __get_delegate(userdata);
  if (delegate) {
    delegate->on_message_received(msg);
  }
}

static void __on_connected(struct mosquitto* /*msqt*/, void* userdata, int rc) {
  LOG(debug) << __PRETTY_FUNCTION__;

  IMqttCallbacks* delegate = __get_delegate(userdata);
  if (delegate) {
    delegate->on_connected(rc);
  }
}

static void __on_connected_with_flags(struct mosquitto* /*msqt*/, void* userdata, int rc, int flags) {
  LOG(debug) << __PRETTY_FUNCTION__;

  IMqttCallbacks* delegate = __get_delegate(userdata);
  if (delegate) {
    delegate->on_connected_with_flags(rc, flags);
  }
}

static void __on_disconnected(struct mosquitto* /*msqt*/, void* userdata, int rc) {
  LOG(debug) << __PRETTY_FUNCTION__;

  IMqttCallbacks* delegate = __get_delegate(userdata);
  if (delegate) {
    delegate->on_disconnected(rc);
  }
}

static void __on_log(struct mosquitto* /*msqt*/, void* userdata, int level, const char* str) {
  LOG(trace) << __PRETTY_FUNCTION__;

  IMqttCallbacks* delegate = __get_delegate(userdata);
  if (delegate) {
    delegate->on_log(level, str);
  }
}

MQTTClient::MQTTClient(const std::string& id, bool clean_session, QObject *parent) :
    __isconnected(false), __msqt(NULL), QObject{parent} {
  LOG(debug) << __PRETTY_FUNCTION__;

  lib_init();
  if (id != MOCKMODE) {
  __msqt = mosquitto_new(id.c_str(), clean_session, this);

  mosquitto_publish_callback_set(__msqt, __on_publish);
  mosquitto_subscribe_callback_set(__msqt, __on_subscribed);
  mosquitto_unsubscribe_callback_set(__msqt, __on_unsubscribed);
  mosquitto_connect_callback_set(__msqt, __on_connected);
  mosquitto_connect_with_flags_callback_set(__msqt, __on_connected_with_flags);
  mosquitto_disconnect_callback_set(__msqt, __on_disconnected);
  mosquitto_message_callback_set(__msqt, __on_message_received);
  mosquitto_log_callback_set(__msqt, __on_log);
  }
  else {
    __msqt = nullptr;
    //__mockRouter = MqttRouter_Mock::getInstance();
  }
  
}

MQTTClient::~MQTTClient() {
  LOG(debug) << __PRETTY_FUNCTION__;
}

void MQTTClient::lib_init(bool force){
  LOG(debug) << __PRETTY_FUNCTION__;

  if (!__is_mosquitto_initialized || force) {
    mosquitto_lib_init();
    __is_mosquitto_initialized = true;
    LOG(debug) << "Initialized mosquitto library.";
  }
}

void MQTTClient::lib_deinit() {
  LOG(debug) << __PRETTY_FUNCTION__;

  mosquitto_lib_cleanup();
  __is_mosquitto_initialized = false;
}

int MQTTClient::connect(const std::string& host, unsigned int port, unsigned int keepalive) {
  LOG(debug) << __PRETTY_FUNCTION__;

  return mosquitto_connect(__msqt, host.c_str(), port, keepalive);
}

int MQTTClient::disconnect() {
  LOG(debug) << __PRETTY_FUNCTION__;

  return mosquitto_disconnect(__msqt);
}

MessageInfo MQTTClient::subscribe(const std::string& topic, unsigned int qos) {
  LOG(debug) << __PRETTY_FUNCTION__;

  int mid = 0;
  int rc = mosquitto_subscribe(__msqt, &mid, topic.c_str(), qos);
  return MessageInfo{mid, rc};
}

int MQTTClient::unsubscribe(const std::string& topic) {
  LOG(debug) << __PRETTY_FUNCTION__;

  return mosquitto_unsubscribe(__msqt, NULL, topic.c_str());
}

MessageInfo MQTTClient::publish(const std::string& topic,
                                unsigned int payloadlen,
                                const void* payload,
                                unsigned int qos,
                                bool retain) {
  LOG(debug) << __PRETTY_FUNCTION__;

  int mid = 0;
  int rc = __msqt ? mosquitto_publish(__msqt, &mid, topic.c_str(), payloadlen, payload, qos, retain) : MOSQ_ERR_INVAL;
  return MessageInfo{mid, rc};
}

MessageInfo MQTTClient::publish_string(const std::string& topic,
                                       const std::string& message,
                                       unsigned int qos,
                                       bool retain) {
  LOG(debug) << __PRETTY_FUNCTION__;

  int mid = 0;
  int rc = mosquitto_publish(__msqt, &mid, topic.c_str(), message.size(), message.c_str(), qos, retain);
  return MessageInfo{mid, rc};
}

int MQTTClient::loop(int timeout) {
  //LOG(trace) << __PRETTY_FUNCTION__;

  return __msqt ? mosquitto_loop(__msqt, timeout, 1) : MOSQ_ERR_INVAL;
}

int MQTTClient::loop_forever(int timeout) {
  LOG(debug) << __PRETTY_FUNCTION__;

  return __msqt ? mosquitto_loop_forever(__msqt, timeout, 1) : MOSQ_ERR_INVAL;
}

void MQTTClient::printStringMessage(const struct mosquitto_message* message) {
  if (message) {
    LOG(info) << "MQTT message received: payload: " << (char*)message->payload;
    LOG(info) << "MQTT message received: payloadlen: " << message->payloadlen;
    LOG(info) << "MQTT message received: topic: " << message->topic;
    LOG(info) << "MQTT message received: qos: " << message->qos;
    LOG(info) << "MQTT message received: mid: " << message->mid;
    LOG(info) << "MQTT message received: retain: " << message->retain;
  }
}
}  // namespace NGGW_MQTT
