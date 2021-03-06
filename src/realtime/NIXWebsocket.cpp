/*
 * Copyright 2019 The Nakama Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef BUILD_IXWEBSOCKET

#include "NIXWebsocket.h"
#include "nakama-cpp/log/NLogger.h"

#undef NMODULE_NAME
#define NMODULE_NAME "NIXWebsocket"

namespace Nakama {

using namespace std;

NIXWebsocket::NIXWebsocket()
{
    _wsMessageQueue.bindWebsocket(&_ixWebSocket);

    // Setup a callback to be fired when a message or an event (open, close, error) is received
    _wsMessageQueue.setOnMessageCallback(
        bind(
            &NIXWebsocket::onSocketMessage,
            this,
            placeholders::_1,
            placeholders::_2,
            placeholders::_3,
            placeholders::_4,
            placeholders::_5,
            placeholders::_6
        )
    );

    setPingSettings(NRtPingSettings(30, 5));
}

NIXWebsocket::~NIXWebsocket()
{
    disconnect();
    _wsMessageQueue.bindWebsocket(nullptr);
}

void NIXWebsocket::setPingSettings(const NRtPingSettings & settings)
{
    _ixWebSocket.setPingInterval(static_cast<int>(settings.intervalSec));
    _ixWebSocket.setPingTimeout(static_cast<int>(settings.timeoutSec));
}

NRtPingSettings NIXWebsocket::getPingSettings() const
{
    NRtPingSettings settings;

    int pingInterval = _ixWebSocket.getPingInterval();
    int pingTimeout  = _ixWebSocket.getPingTimeout();

    if (pingInterval > 0) settings.intervalSec = static_cast<uint32_t>(pingInterval);
    if (pingTimeout  > 0) settings.timeoutSec  = static_cast<uint32_t>(pingTimeout);

    return settings;
}

void NIXWebsocket::tick()
{
    _wsMessageQueue.poll();
}

void NIXWebsocket::connect(const string& url, NRtTransportType type)
{
    _type = type;

    NLOG_DEBUG("...");
    _ixWebSocket.setUrl(url);
    _ixWebSocket.start();
}

void NIXWebsocket::onSocketMessage(
    ix::WebSocketMessageType messageType,
    const string& str,
    size_t wireSize,
    const ix::WebSocketErrorInfo& error,
    const ix::WebSocketOpenInfo& openInfo,
    const ix::WebSocketCloseInfo& closeInfo)
{
    if (messageType == ix::WebSocketMessageType::Message)
    {
        NRtTransportInterface::fireOnMessage(str);
    }
    else if (messageType == ix::WebSocketMessageType::Open)
    {
        NRtTransportInterface::fireOnConnected();
    }
    else if (messageType == ix::WebSocketMessageType::Close)
    {
        NRtClientDisconnectInfo info;

        info.code   = closeInfo.code;
        info.remote = closeInfo.remote;
        info.reason = closeInfo.reason;

        NRtTransportInterface::fireOnDisconnected(info);
    }
    else if (messageType == ix::WebSocketMessageType::Error)
    {
        NRtTransportInterface::fireOnError(error.reason);
    }
    else if (messageType == ix::WebSocketMessageType::Ping)
    {
    }
    else if (messageType == ix::WebSocketMessageType::Pong)
    {
    }
    else if (messageType == ix::WebSocketMessageType::Fragment)
    {
    }
}

bool NIXWebsocket::send(const NBytes & data)
{
    NLOG(NLogLevel::Debug, "sending %d bytes...", data.size());

    ix::WebSocketSendInfo info;

    if (_type == NRtTransportType::Binary)
    {
        info = _ixWebSocket.send(data);
    }
    else
    {
        info = _ixWebSocket.sendText(data);
    }

    if (!info.success)
    {
        NLOG_ERROR("send failed");
    }

    return info.success;
}

void NIXWebsocket::disconnect()
{
    NLOG_DEBUG("...");

    _ixWebSocket.stop();
}

} // Nakama

#endif // BUILD_IXWEBSOCKET
