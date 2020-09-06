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

#ifndef KLICK_METRONOME_HH
#define KLICK_METRONOME_HH

#include "audio_interface.hh"
#include "audio_chunk.hh"
#include "main.hh"

#include <boost/noncopyable.hpp>

#include <vector>
#include <deque>

/*
 * abstract metronome base class
 */
class Metronome
  : boost::noncopyable
{
  public:

    Metronome(AudioInterface & audio);
    virtual ~Metronome() { }

    void set_sound(AudioChunkConstPtr emphasis, AudioChunkConstPtr normal);

    void set_active(bool b);
    void start() { set_active(true); }
    void stop() { set_active(false); }

    bool active() const { return _active; }

    virtual void do_start() { }
    virtual void do_stop() { }

    virtual void process_callback(sample_t *, nframes_t) REALTIME = 0;
    virtual void timebase_callback(position_t *) REALTIME { }

    virtual bool running() const = 0;

    void tap();
    void update_taps(double );
    virtual void tap(double /*now*/)  { }

  protected:

    void play_click(bool emphasis, nframes_t offset, float volume = 1.0f);

    AudioInterface & _audio;

    AudioChunkConstPtr _click_emphasis;
    AudioChunkConstPtr _click_normal;

    static int const MAX_TAPS = 5;
    static float constexpr MAX_TAP_AGE = 3.0f;
    static float constexpr TAP_DIFF = 0.2f;
    std::deque<double> _taps;

  private:

    bool _active;
};


#endif // KLICK_METRONOME_HH
