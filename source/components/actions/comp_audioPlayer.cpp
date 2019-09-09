#include "mcv_platform.h"
#include "comp_audioPlayer.h"
#include "components/common/comp_transform.h"
#include "engine.h"

using namespace physx;

DECL_OBJ_MANAGER("comp_audioplayer", TCompAudioPlayer);

void TCompAudioPlayer::debugInMenu() {
}

void TCompAudioPlayer::load(const json& j, TEntityParseContext& ctx) {
    _audioEventString = j.value("_audioEventString", _audioEventString);
    _autoPlay = j.value("_autoPlay", _autoPlay);
    
    assert(_audioEventString != "");
    _audioEvent = EngineAudio.playEvent(_audioEventString);
    if (_audioEvent.is3D()) {
        TCompTransform* c_trans = get<TCompTransform>();
        _audioEvent.set3DAttributes(*c_trans);
    }
    if(!_autoPlay){
        _audioEvent.stop();
    }
}

void TCompAudioPlayer::registerMsgs() {
    DECL_MSG(TCompAudioPlayer, TMsgSoundRequest, onSoundRequest);
}

void TCompAudioPlayer::onSoundRequest(const TMsgSoundRequest& msg) {
    if (msg.name != "") {
        _audioEvent = EngineAudio.playEvent(msg.name);
    }
}

AudioEvent TCompAudioPlayer::getEvent() {
    return _audioEvent;
}


void TCompAudioPlayer::update(float dt) {
    if (_audioEvent.is3D()) {
        TCompTransform* c_trans = get<TCompTransform>();
        _audioEvent.set3DAttributes(*c_trans);
    }
}