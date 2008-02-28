/*
 * @(#)TextTrack.cpp 3.00 17 Feb 2008
 *
 * Copyright (c) 2008 Rangel Reale (rreale@bol.com.br)
 *
 * This file is part of TSE3 - the Trax Sequencer Engine version 3.00.
 *
 * This library is modifiable/redistributable under the terms of the GNU
 * General Public License.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; see the file COPYING. If not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include "tse3/TextTrack.h"
#include "tse3/FileBlockParser.h"

#include <sstream>

using namespace TSE3;

/******************************************************************************
 * TextTrackIterator class
 *****************************************************************************/

namespace TSE3
{
    /**
     * The @ref PlayableIterator for the @ref TextTrack class.
     *
     * @short   TextTrack PlayableIterator.
     * @author  Rangel Reale
     * @version 3.00
     * @see     PlayableIterator
     * @see     TextTrack
     */
    class TextTrackIterator : public PlayableIterator,
                               public Listener<TextTrackListener>
    {
        public:

            TextTrackIterator(TextTrack *t, Clock c);
            virtual ~TextTrackIterator();
            virtual void moveTo(Clock c);

            virtual void EventTrack_EventAltered(EventTrack<Text> *);
            virtual void EventTrack_EventInserted(EventTrack<Text> *);
            virtual void EventTrack_EventErased(EventTrack<Text> *);
            virtual void Notifier_Deleted(EventTrack<Text> *);

        protected:

            virtual void getNextEvent();

            size_t      _pos;
            TextTrack *_ttrack;

        private:

            TextTrackIterator &operator=(const TextTrackIterator &);
            TextTrackIterator(const TextTrackIterator &);
    };
}


TextTrackIterator::TextTrackIterator(TextTrack *t, Clock c)
: _pos(0), _ttrack(t)
{
    moveTo(c);
    attachTo(_ttrack);
}


TextTrackIterator::~TextTrackIterator()
{
}


void TextTrackIterator::moveTo(Clock c)
{
    if (_ttrack) _pos = _ttrack->index(c);
    if (!_ttrack || _pos == _ttrack->size() || !_ttrack->status())
    {
        _more = false;
        _next = MidiEvent();
    }
    else
    {
        _more = true;
        _next = MidiEvent(MidiCommand(MidiCommand_TSE_Meta, 0, 0,
                                      MidiCommand_TSE_Meta_Text, 0,
                                      (*_ttrack)[_pos].data.text),
                         (*_ttrack)[_pos].time);
    }
}


void TextTrackIterator::getNextEvent()
{
    ++_pos;
    if (_pos == _ttrack->size())
    {
        _more = false;
        _next = MidiEvent();
    }
    else
    {
        _more = true;
        _next = MidiEvent(MidiCommand(MidiCommand_TSE_Meta, 0, 0,
                                      MidiCommand_TSE_Meta_Text, 0,
                                      (*_ttrack)[_pos].data.text),
                         (*_ttrack)[_pos].time);
    }
}


void TextTrackIterator::EventTrack_EventAltered(EventTrack<Text> *)
{
    moveTo(_next.time);
}


void TextTrackIterator::EventTrack_EventInserted(EventTrack<Text> *)
{
    moveTo(_next.time);
}


void TextTrackIterator::EventTrack_EventErased(EventTrack<Text> *)
{
    moveTo(_next.time);
}


void TextTrackIterator::Notifier_Deleted(EventTrack<Text> *)
{
    _ttrack = 0;
    _more   = false;
    _next   = MidiEvent();
}


/******************************************************************************
 * TextTrack class
 *****************************************************************************/

TextTrack::TextTrack()
: _status(true)
{
    insert(Event<Text>(Text(), 0));
}


TextTrack::~TextTrack()
{
}


PlayableIterator *TextTrack::iterator(Clock index)
{
    return new TextTrackIterator(this, index);
}


Clock TextTrack::lastClock() const
{
    return (!data.empty()) ? data[size()-1].time : Clock(0);
}

/******************************************************************************
 * TextTrack Serializable interface
 *****************************************************************************/

void TextTrack::save(std::ostream &o, int i) const
{

}

void TextTrack::load(std::istream &in, SerializableLoadInfo &info)
{

}

#if 0
void TextTrack::save(std::ostream &o, int i) const
{
    o << indent(i)   << "{\n";
    o << indent(i+1) << "Status:";
    if (_status)
        o << "On\n";
    else
        o << "Off\n";
    o << indent(i+1) << "Events\n";
    o << indent(i+1) << "{\n";
    for (size_t n = 0; n < size(); ++n)
    {
        o << indent(i+2) << data[n].time << ":" << data[n].data.text << "\n";
    }
    o << indent(i+1) << "}\n";
    o << indent(i) << "}\n";
}


namespace
{
    /**
     * A catch-all FileItemParser to handle data items in the TextTrack
     * Events sub-block.
     */
    class FileItemParser_Events : public FileItemParser
    {
        public:
            FileItemParser_Events(TextTrack *tt, int PPQN)
            : tt(tt), PPQN(PPQN) {}
            virtual void parse(const std::string &line)
            {
                int time, tempo;
                std::istringstream si(line);
                si >> time;
                si.ignore(1); // skip colon
                si >> tempo;
                time = Clock::convert(time, PPQN);
                tt->insert(Event<Text>(Tempo(tempo), time));
            }
        private:
            TextTrack *tt;
            int         PPQN;
    };

    /**
     * A simple Serializable class that creates a simple FileBlockParser
     * to load the Events sub-block of the TextTrack block.
     */
    class Events : public Serializable
    {
        public:
            Events(TextTrack *tt)
            : tt(tt) {}
            virtual void load(std::istream &in, SerializableLoadInfo &info)
            {
                FileItemParser_Events events(tt, info.PPQN);
                FileBlockParser parser;
                parser.add(&events);
                parser.parse(in, info);
            }
        private:
            TextTrack *tt;
    };
}


void TextTrack::load(std::istream &in, SerializableLoadInfo &info)
{
    FileItemParser_OnOff<TextTrack> status(this, &TextTrack::setStatus);
    Events events(this);
    FileBlockParser parser;
    parser.add("Status", &status);
    parser.add("Events", &events);
    parser.parse(in, info);
}
#endif
