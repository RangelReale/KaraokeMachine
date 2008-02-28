/*
 * @(#)TextTrack.h 3.00 17 Feb 2008
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

#ifndef TSE3_TEXTTRACK_H
#define TSE3_TEXTTRACK_H

#include "tse3/listen/TextTrack.h"

#include "tse3/Notifier.h"
#include "tse3/Playable.h"
#include "tse3/Serializable.h"
#include "tse3/EventTrack.h"

#include <string>

namespace TSE3
{
    /**
     * Text objects are song lyrics
     *
     * This class defines the type that is used to create text events,
     * which are of type @ref Event<@ref Text>.
     *
     * Text is a value type.
     *
     * @short   A text (lyrics) event
     * @author  Rangel Reale
     * @version 3.00
     * @see     TextTrack
     * @see     Event
     */
    class Text
    {
        public:

            /**
             * Create a Text.
             *
             * @param t Text value
             */
            Text(std::string t = "") : text(t) {}

            std::string text;

            int operator==(const Text &f) const { return text == f.text; }
    };

    /**
     * The TextTrack provides a simple time ordered list of lyrics
     * in the @ref Song.
     *
     * There is one TextTrack per @ref Song.
     *
     * @short   A list of Text events
     * @author  Rangel Reale
     * @version 3.00
     * @see     Event
     * @see     Text
     */
    class TextTrack : public EventTrack<Text>,
                       public Serializable
    {
        public:

            /**
             * Creates an empty TextTrack.
             */
            TextTrack();
            virtual ~TextTrack();

            /**
             * Returns the status of the TextTrack.
             *
             * A value of true means text events are generated, a value of
             * false means no text events are generated
             *
             * @return TextTrack status
             * @see    setStatus
             */
            bool status() const { return _status; }

            /**
             * Set the TextTrack status value.
             *
             * @param s New status
             * @see   status
             */
            void setStatus(bool s) { _status = s; }

            /**
             * @reimplemented
             */
            virtual PlayableIterator *iterator(Clock index);

            /**
             * @reimplemented
             */
            virtual Clock lastClock() const;

            /**
             * @reimplemented
             */
            virtual void save(std::ostream &o, int i) const;

            /**
             * @reimplemented
             */
            virtual void load(std::istream &i, SerializableLoadInfo &info);

        private:

            TextTrack &operator=(const TextTrack &);
            TextTrack(const TextTrack &);

            /**
             * Used by load to read events.
             */
            void loadEvents(std::istream &i, int filePPQN);

            bool _status;
    };
}

#endif
