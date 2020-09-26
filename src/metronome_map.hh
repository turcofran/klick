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

#ifndef KLICK_METRONOME_MAP_HH
#define KLICK_METRONOME_MAP_HH

#include "metronome.hh"
#include "tempomap.hh"
#include "position.hh"
#include <mutex>

#include <string>

/*
 * plays a click track using a predefined tempomap
 */
class MetronomeMap
  : public Metronome
{
  public:
    MetronomeMap(
        AudioInterface & audio,
        TempoMapConstPtr tempomap,
        float tempo_multiplier,
        bool transport,
        int preroll,
        std::string const & start_label
    );
    virtual ~MetronomeMap();

    virtual void do_start();
    virtual void do_stop();

    bool running() const;

    nframes_t current_frame() const;
    nframes_t total_frames() const;

    virtual void process_callback(sample_t *, nframes_t);
    virtual void timebase_callback(position_t *);

    virtual void tap(double now);
    //virtual void tap();
    void set_map(int, int, float);
    std::string get_next_map_str();
    std::tuple<int, int, float> get_next_map() const;
    std::tuple<int, int, float> get_current_map() const;
    void set_tempo(float );
    void set_meter(int , int );

  private:
    static int const TICKS_PER_BEAT = 1920;
    float const TEMPO_MIN = 20;
    float const TEMPO_MAX = 280;
    float const MIN_TAP_TEMPO_DIFF = 1;

    // transport position
    nframes_t _frame;

    // position in tempomap
    Position _pos;
    Position _new_pos;
    bool _new_map_requested;
    std::mutex _newposrex_mtx;

    bool _transport_enabled;
};


#endif // KLICK_METRONOME_MAP_HH
