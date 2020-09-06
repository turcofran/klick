/*
 * klick - an advanced metronome for jack
 *
 * Copyright (C) 2007-2013  Dominic Sacré  <dominic.sacre@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "metronome.hh"
#include "audio_interface_jack.hh"
#include <algorithm>
#include "util/debug.hh"
#include <sys/time.h>
#include <time.h>

Metronome::Metronome(AudioInterface & audio)
  : _audio(audio)
  , _active(false)
{
}

void Metronome::set_active(bool b)
{
    if (b) {
        do_start();
    } else {
        do_stop();
    }

    _active = b;
}


void Metronome::set_sound(AudioChunkConstPtr emphasis, AudioChunkConstPtr normal)
{
    _click_emphasis = emphasis;
    _click_normal = normal;
}


void Metronome::tap()
{
    ::timeval tv;
    ::gettimeofday(&tv, NULL);
    double now = tv.tv_sec + 1.e-6 * tv.tv_usec;
    tap(now);
}

void Metronome::update_taps(double now){
  if (_taps.size() && now < _taps.back()) {
      // distortion in space-time continuum
      _taps.clear();
  }
  _taps.push_back(now);
  if (static_cast<int>(_taps.size()) > MAX_TAPS) {
      _taps.pop_front();
  }
  // forget taps which happened too long ago
  _taps.erase(
      std::remove_if(_taps.begin(), _taps.end(),
                     [&](double t){ return t < now - MAX_TAP_AGE; }),
      _taps.end()
  );
}

void Metronome::play_click(bool emphasis, nframes_t offset, float volume)
{
    ASSERT(_click_emphasis);
    ASSERT(_click_normal);

    AudioChunkConstPtr click = emphasis ? _click_emphasis : _click_normal;

    _audio.play(click, offset, volume);
}
