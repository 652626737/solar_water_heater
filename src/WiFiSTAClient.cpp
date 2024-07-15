#include "WiFiSTAClient.h"

/**
 * @brief WiFiSTAClient 类构造函数
 *
 * 用于创建一个新的 WiFiSTAClient 对象，并设置相关的 Wi-Fi 参数和服务器信息。
 *
 * @param ssid Wi-Fi 的 SSID 名称
 * @param password Wi-Fi 的密码
 * @param server_ip 服务器的 IP 地址
 * @param server_port 服务器的端口号
 */
WiFiSTAClient::WiFiSTAClient(const char* ssid, const char* password, const char* server_ip, int server_port)
    : ssid(ssid), password(password), server_ip(server_ip), server_port(server_port), reconnectAttempts(3) {}

/**
 * @brief 连接到WiFi接入点
 *
 * 尝试连接到指定的WiFi接入点（Access Point）。
 * 如果连接成功，将打印出连接的IP地址。
 */
void WiFiSTAClient::connectToAP() {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.print("Connected to WiFi. IP address: ");
    Serial.println(WiFi.localIP());
}

/**
 * @brief 断开与 AP 的连接
 *
 * 该函数用于断开当前 Wi-Fi STA 客户端与已连接的接入点（AP）的连接。
 * 断开连接后，会向串行端口输出一条消息，表明已成功断开连接。
 */
void WiFiSTAClient::disconnectFromAP() {
    WiFi.disconnect();
    Serial.println("Disconnected from WiFi");
}

/**
 * @brief 连接到服务器
 *
 * 尝试使用指定的 IP 地址和端口连接到服务器。
 *
 * @return 如果成功连接到服务器，则返回 true；否则返回 false。
 */
bool WiFiSTAClient::connectToServer() {
    if (client.connect(server_ip, server_port)) {
        Serial.println("Connected to server.");
        return true;
    } else {
        Serial.println("Connection to server failed.");
        return false;
    }
}

/**
 * @brief 从服务器断开连接
 *
 * 断开与服务器的连接。如果当前已经连接，则执行断开操作，并通过串口打印断开消息。
 */
void WiFiSTAClient::disconnectFromServer() {
    if (client.connected()) {
        client.stop();
        Serial.println("Disconnected from server.");
    }
}

/**
 * @brief 发送数据到WiFi STA客户端
 *
 * 如果客户端已经连接到服务器，则向服务器发送指定的数据。
 * 如果未连接，则在串行端口上打印错误消息。
 *
 * @param data 要发送的数据
 */
void WiFiSTAClient::sendData(const String& data) {
    if (client.connected()) {
        client.print(data);
    } else {
        Serial.println("Not connected to server. Unable to send data.");
    }
}

/**
 * @brief 接收数据
 *
 * 从 WiFi STA 客户端接收数据。
 *
 * @return 返回接收到的字符串，如果未连接或没有可用数据则返回空字符串。
 */
String WiFiSTAClient::receiveData() {
    if (client.connected() && client.available()) {
        return client.readStringUntil('\n');
    }
    return "";
}

/**
 * @brief 判断是否连接到AP
 *
 * 检查WiFi客户端是否已成功连接到AP（接入点）。
 *
 * @return 如果成功连接到AP，则返回true；否则返回false。
 */
bool WiFiSTAClient::isAPConnected() {
    return WiFi.status() == WL_CONNECTED;
}

/**
 * @brief 判断是否已连接到服务器
 *
 * 检查 WiFi 客户端是否已成功连接到指定的服务器。
 *
 * @return 如果已连接到服务器，则返回 true；否则返回 false
 */
bool WiFiSTAClient::isServerConnected() {
    return client.connected();
}

/**
 * @brief 设置重新连接尝试次数
 *
 * 设置 WiFiSTA 客户端在连接失败后的重新连接尝试次数。
 *
 * @param attempts 重新连接尝试次数
 *
 * @note 修改此值将影响 WiFiSTA 客户端在连接失败后重新连接的次数。
 */
void WiFiSTAClient::setReconnectAttempts(int attempts) {
    reconnectAttempts = attempts;
}

/**
 * @brief 处理重新连接
 *
 * 处理WiFi STA客户端重新连接到服务器的逻辑。
 * 在重新连接次数不超过reconnectAttempts的前提下，持续尝试连接到服务器。
 * 每次尝试失败后，等待1秒，然后输出重新连接的信息。
 */
void WiFiSTAClient::handleReconnect() {
    int attempts = 0;
    while (attempts < reconnectAttempts && !connectToServer()) {
        attempts++;
        delay(1000);
        Serial.println("Reconnecting to server...");
    }
}
