#pragma once

#include "MediaEngine.h"

#if defined(__APPLE__)

#    import <AVFoundation/AVFoundation.h>
#    import <AVKit/AVPlayerViewController.h>

@class AVMediaSessionHandler;

NS_AX_BEGIN

class AvfMediaEngine : public MediaEngine
{
public:
    void fireMediaEvent(MEMediaEventType event)
    {
        if (_onMediaEvent)
            _onMediaEvent(event);
    }
    void setCallbacks(std::function<void(MEMediaEventType)> onMediaEvent,
                      std::function<void(const MEVideoFrame&)> onVideoFrame) override
    {
        _onMediaEvent = std::move(onMediaEvent);
        _onVideoFrame = std::move(onVideoFrame);
    }
    void setAutoPlay(bool bAutoPlay) override;
    bool open(std::string_view sourceUri) override;
    bool close() override;
    bool setLoop(bool bLooping) override;
    bool setRate(double fRate) override;
    bool setCurrentTime(double fSeekTimeInSec) override;
    bool play() override;
    bool pause() override;
    bool stop() override;
    bool isPlaybackEnded() const override { return _playbackEnded; }
    MEMediaState getState() const override;
    bool transferVideoFrame() override;

    void onStatusNotification(void* context);
    void onPlayerEnd();
    bool isPlaying() const {
        return _state == MEMediaState::Playing;
    }

    void internalPlay(bool replay = false);
    void internalPause();

    void setViewRect(int left, int top, int width, int height) override;
    void showPlaybackControls(bool value) override;
    void setUserInteractionEnabled(bool enabled) override;
    
private:
    std::function<void(MEMediaEventType)> _onMediaEvent;
    std::function<void(const MEVideoFrame&)> _onVideoFrame;
    MEVideoPixelFormat _videoPF = MEVideoPixelFormat::INVALID;
    MEMediaState _state = MEMediaState::Closed;
    int _videoRotation{0};
    MEIntPoint _videoExtent;
    AVPlayerViewController* _playerController = nil;
    AVPlayer* _player = nil;
    AVPlayerItem* _playerItem = nil;
    AVPlayerItemOutput* _playerOutput = nil;
    AVMediaSessionHandler* _sessionHandler = nil;

    bool _bAutoPlay = false;
    bool _repeatEnabled = false;
    bool _playbackEnded = false;

    int _left = 0;
    int _top = 0;
    int _width = 0;
    int _height = 0;
    bool _showPlaybackControls = true;
    bool _userInteractionEnabled = true;
    
    /*
    true: luma=[0,255] chroma=[1,255]
    false: luma=[16,235] chroma=[16,240]
    */
    bool _bFullColorRange = false;
};

struct AvfMediaEngineFactory : public MediaEngineFactory
{
    MediaEngine* createMediaEngine() override
    {
        return new AvfMediaEngine();
    }
    void destroyMediaEngine(MediaEngine* me) override { delete static_cast<AvfMediaEngine*>(me); }
};

NS_AX_END

#endif
