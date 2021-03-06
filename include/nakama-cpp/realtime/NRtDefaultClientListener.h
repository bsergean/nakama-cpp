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

#pragma once

#include "nakama-cpp/realtime/NRtClientListenerInterface.h"
#include <functional>

namespace Nakama {

    /**
     * A default listener for receiving <c>NRtClientInterface</c> events.
     * It allows to set callbacks for needed events without creating own class.
     */
    class NAKAMA_API NRtDefaultClientListener : public NRtClientListenerInterface
    {
    public:
        using ConnectCallback = std::function<void()>;
        using DisconnectCallback = std::function<void(const NRtClientDisconnectInfo& info)>;
        using ErrorCallback = std::function<void(const NRtError&)>;
        using ChannelMessageCallback = std::function<void(const NChannelMessage&)>;
        using ChannelPresenceCallback = std::function<void(const NChannelPresenceEvent&)>;
        using MatchmakerMatchedCallback = std::function<void(NMatchmakerMatchedPtr)>;
        using MatchDataCallback = std::function<void(const NMatchData&)>;
        using MatchPresenceCallback = std::function<void(const NMatchPresenceEvent&)>;
        using NotificationsCallback = std::function<void(const NNotificationList&)>;
        using StatusPresenceCallback = std::function<void(const NStatusPresenceEvent&)>;
        using StreamPresenceCallback = std::function<void(const NStreamPresenceEvent&)>;
        using StreamDataCallback = std::function<void(const NStreamData&)>;

        void setConnectCallback(ConnectCallback callback) { _connectCallback = callback; }
        void setDisconnectCallback(DisconnectCallback callback) { _disconnectCallback = callback; }
        void setErrorCallback(ErrorCallback callback) { _errorCallback = callback; }
        void setChannelMessageCallback(ChannelMessageCallback callback) { _channelMessageCallback = callback; }
        void setChannelPresenceCallback(ChannelPresenceCallback callback) { _channelPresenceCallback = callback; }
        void setMatchmakerMatchedCallback(MatchmakerMatchedCallback callback) { _matchmakerMatchedCallback = callback; }
        void setMatchDataCallback(MatchDataCallback callback) { _matchDataCallback = callback; }
        void setMatchPresenceCallback(MatchPresenceCallback callback) { _matchPresenceCallback = callback; }
        void setNotificationsCallback(NotificationsCallback callback) { _notificationsCallback = callback; }
        void setStatusPresenceCallback(StatusPresenceCallback callback) { _statusPresenceCallback = callback; }
        void setStreamPresenceCallback(StreamPresenceCallback callback) { _streamPresenceCallback = callback; }
        void setStreamDataCallback(StreamDataCallback callback) { _streamDataCallback = callback; }

    protected:
        void onConnect() override;
        void onDisconnect(const NRtClientDisconnectInfo& info) override;
        void onError(const NRtError& error) override;
        void onChannelMessage(const NChannelMessage& message) override;
        void onChannelPresence(const NChannelPresenceEvent& presence) override;
        void onMatchmakerMatched(NMatchmakerMatchedPtr matched) override;
        void onMatchData(const NMatchData& matchData) override;
        void onMatchPresence(const NMatchPresenceEvent& matchPresence) override;
        void onNotifications(const NNotificationList& notifications) override;
        void onStatusPresence(const NStatusPresenceEvent& presence) override;
        void onStreamPresence(const NStreamPresenceEvent& presence) override;
        void onStreamData(const NStreamData& data) override;

    protected:
        ConnectCallback _connectCallback;
        DisconnectCallback _disconnectCallback;
        ErrorCallback _errorCallback;
        ChannelMessageCallback _channelMessageCallback;
        ChannelPresenceCallback _channelPresenceCallback;
        MatchmakerMatchedCallback _matchmakerMatchedCallback;
        MatchDataCallback _matchDataCallback;
        MatchPresenceCallback _matchPresenceCallback;
        NotificationsCallback _notificationsCallback;
        StatusPresenceCallback _statusPresenceCallback;
        StreamPresenceCallback _streamPresenceCallback;
        StreamDataCallback _streamDataCallback;
    };
}
