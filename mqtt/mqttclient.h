#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <mosquitto.h>
#include <string>

#include <QObject>

namespace Mqtt {

static constexpr const char* const MOCKMODE = "MOCKMODE";

struct MessageInfo {
  int mid;
  int rc;
};

/**
 * @brief MQTT delegate interface. Represents "C" callbacks.
 *
 */
class IMqttCallbacks {
 public:
  virtual void on_message_received(const struct mosquitto_message* message) = 0;
  virtual void on_connected(int rc) = 0;
  virtual void on_connected_with_flags(int rc, int flags) = 0;
  virtual void on_disconnected(int rc) = 0;
  virtual void on_publish(int id) = 0;
  virtual void on_subscribed(int mid, int qos_count, const int* granted_qos) = 0;
  virtual void on_unsubscribed(int mid) = 0;
  virtual void on_log(int level, const char* str) = 0;
};

/**
 * @brief Wrapper over Eclipse Mosquitto MQTT client API.
 *        Implements basic functionality like publishing, subscribing and callbacks activation.
 *        More info here: https://mosquitto.org/api/files/mosquitto-h.html
 *
 */
class MQTTClient : public QObject
{
    Q_OBJECT
 public:
  /**
   * @brief Construct a new MQTTClient object
   *
   * @param id Client name
   * @param clean_session Session cleaning. Set to true to instruct the broker to clean all messages
   *                      and subscriptions on disconnect, false to instruct it to keep them.
   */
  explicit MQTTClient(const std::string& id, bool clean_session = true, QObject *parent = nullptr);
  virtual ~MQTTClient();

  /**
   * @brief Connects to MQTT broker.
   *        Activates on_connected() callback of delegate object if connected.
   *
   * @param host Broker's hostname or IP
   * @param port Broker's port
   * @param keepalive Connection keepalive
   * @return  MOSQ_ERR_SUCCESS	on success.
   *          MOSQ_ERR_INVAL	if the input parameters were invalid.
   *          MOSQ_ERR_ERRNO	if a system call returned an error.
   */
  int connect(const std::string& host, unsigned int port = 1883, unsigned int keepalive = 9999);

  /**
   * @brief Disconnects from MQTT broker.
   *        Activates on_disconnected() callback of delegate object if disconnected.
   *
   * @return  MOSQ_ERR_SUCCESS	on success.
   *          MOSQ_ERR_INVAL	if the input parameters were invalid.
   *          MOSQ_ERR_NO_CONN	if the client isn’t connected to a broker.
   */
  int disconnect();

  /**
   * @brief Publish binary message on the topic.
   *        Activates on_published() callback of delegate object if published.
   *
   * @param topic Topic
   * @param payloadlen  Size of message
   * @param payload Pointer to message chunk
   * @param qos Required QoS
   * @param retain set to true to make the message retained.
   * @return  MOSQ_ERR_SUCCESS	on success.
   *          MOSQ_ERR_INVAL	if the input parameters were invalid.
   *          MOSQ_ERR_NOMEM	if an out of memory condition occurred.
   *          MOSQ_ERR_NO_CONN	if the client isn’t connected to a broker.
   *          MOSQ_ERR_PROTOCOL	if there is a protocol error communicating with the broker.
   *          MOSQ_ERR_PAYLOAD_SIZE	if payloadlen is too large.
   *          MOSQ_ERR_MALFORMED_UTF8	if the topic is not valid UTF-8
   *          MOSQ_ERR_QOS_NOT_SUPPORTED	if the QoS is greater than that supported by the broker.
   *          MOSQ_ERR_OVERSIZE_PACKET	if the resulting packet would be larger than supported by the broker.
   */
  MessageInfo publish(const std::string& topic,
                      unsigned int payloadlen,
                      const void* payload,
                      unsigned int qos = 1,
                      bool retain = false);
  /**
   * @brief Publish string message on the topic.
   *        Activates on_published() callback of delegate object if published.
   *
   * @param topic Topic
   * @param message  string message
   * @param qos Required QoS
   * @param retain set to true to make the message retained.
   * @return  MOSQ_ERR_SUCCESS	on success.
   *          MOSQ_ERR_INVAL	if the input parameters were invalid.
   *          MOSQ_ERR_NOMEM	if an out of memory condition occurred.
   *          MOSQ_ERR_NO_CONN	if the client isn’t connected to a broker.
   *          MOSQ_ERR_PROTOCOL	if there is a protocol error communicating with the broker.
   *          MOSQ_ERR_PAYLOAD_SIZE	if payloadlen is too large.
   *          MOSQ_ERR_MALFORMED_UTF8	if the topic is not valid UTF-8
   *          MOSQ_ERR_QOS_NOT_SUPPORTED	if the QoS is greater than that supported by the broker.
   *          MOSQ_ERR_OVERSIZE_PACKET	if the resulting packet would be larger than supported by the broker.
   */
  MessageInfo publish_string(const std::string& topic,
                             const std::string& message,
                             unsigned int qos = 1,
                             bool retain = false);

  /**
   * @brief Subscribes to the topic.
   *        Activates on_subscribed() callback of delegate object on success.
   *
   * @param topic Topic
   * @param qos Required QoS
   * @return  MOSQ_ERR_SUCCESS	on success.
   *          MOSQ_ERR_INVAL	if the input parameters were invalid.
   *          MOSQ_ERR_NOMEM	if an out of memory condition occurred.
   *          MOSQ_ERR_NO_CONN	if the client isn’t connected to a broker.
   *          MOSQ_ERR_MALFORMED_UTF8	if the topic is not valid UTF-8
   *          MOSQ_ERR_OVERSIZE_PACKET	if the resulting packet would be larger than supported by the broker.
   */
  MessageInfo subscribe(const std::string& topic, unsigned int qos = 1);

  /**
   * @brief Unsubscribes from the topic.
   *        Activates on_unsubscribed() callback of delegate object on success.
   *
   * @param topic topic
   * @return  MOSQ_ERR_SUCCESS	on success.
   *          MOSQ_ERR_INVAL	if the input parameters were invalid.
   *          MOSQ_ERR_NOMEM	if an out of memory condition occurred.
   *          MOSQ_ERR_NO_CONN	if the client isn’t connected to a broker.
   *          MOSQ_ERR_MALFORMED_UTF8	if the topic is not valid UTF-8
   *          MOSQ_ERR_OVERSIZE_PACKET	if the resulting packet would be larger than supported by the broker.
   */
  int unsubscribe(const std::string& topic);

  /**
   * @brief Infinite blocking loop.
   *
   * @param timeout Maximum number of milliseconds to wait for network activity in the select() call before timing out.
   *                Set to 0 for instant return.
   *                Set negative to use the default of 1000ms.
   * @return  MOSQ_ERR_SUCCESS	on success.
   *          MOSQ_ERR_INVAL	if the input parameters were invalid.
   *           MOSQ_ERR_NOMEM	if an out of memory condition occurred.
   *           MOSQ_ERR_NO_CONN	if the client isn’t connected to a broker.
   *           MOSQ_ERR_CONN_LOST	if the connection to the broker was lost.
   *           MOSQ_ERR_PROTOCOL	if there is a protocol error communicating with the broker.
   *           MOSQ_ERR_ERRNO	if a system call returned an error.
   */
  int loop_forever(int timeout = -1);

  /**
   * @brief The main network loop for the client.
   *        This must be called frequently to keep communications between the client and broker working.
   *
   * @param timeout Maximum number of milliseconds to wait for network activity in the select() call before timing out.
   *                Set to 0 for instant return.
   *                Set negative to use the default of 1000ms.
   * @return  MOSQ_ERR_SUCCESS	on success.
   *          MOSQ_ERR_INVAL	if the input parameters were invalid.
   *          MOSQ_ERR_NOMEM	if an out of memory condition occurred.
   *          MOSQ_ERR_NO_CONN	if the client isn’t connected to a broker.
   *          MOSQ_ERR_CONN_LOST	if the connection to the broker was lost.
   *          MOSQ_ERR_PROTOCOL	if there is a protocol error communicating with the broker.
   *          MOSQ_ERR_ERRNO	if a system call returned an error.
   */
  int loop(int timeout = -1);

  /**
   * @brief Connection state
   *
   * @return true if client is connected to the broker and false if not connected
   */
  bool isConnected() { return __isconnected; };

  /**
   * @brief Sets delegate implementing IMQTTCallbacks
   *
   * @param callbacks pointer to delegate object
   */
  void setDelegate(IMqttCallbacks* callbacks) { _callbacks = callbacks; };

  /**
   * @brief  Gets delegate object or nullptr if not assigned.
   *
   * @return pointer to delegate object
   */
  IMqttCallbacks* getDelegate() { return _callbacks; };

  /**
   * @brief Prints string message contaent and params.
   *
   * @param message string message
   * @return true if client is connected to the broker and false if not connected
   */
  static void printStringMessage(const struct mosquitto_message* message);

/**
 * @brief Initialize mosquitto library. In normal workflow should not be invoked.
 *        Default initialization of mosquitto library is done in MQTTClient constructor.
 *        Could be used in exceptional cases when mosquitto library reset is needed.
 *
 * @param force If true, invokes library initialization even if it is already initialized.
 */
  static void lib_init(bool force = false);

/**
 * @brief Deinitialize mosquitto library.
 *        Could be used in exceptional cases when mosquitto library reset is needed.
 */
  static void lib_deinit();

 protected:
  IMqttCallbacks* _callbacks{nullptr};

 private:
  bool __isconnected;
  struct mosquitto* __msqt;
};
}  // namespace NGGW_MQTT

#endif  // MQTTCLIENT_H
