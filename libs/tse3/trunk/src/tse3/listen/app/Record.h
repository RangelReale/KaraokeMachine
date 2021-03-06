/*
 * @(#)listen/app/Record.h 1.00 18 June 2000
 *
 * Copyright (c) 2000 Pete Goodliffe (pete@cthree.org)
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

#ifndef TSE3_LISTEN_APP_RECORD_H
#define TSE3_LISTEN_APP_RECORD_H

namespace TSE3
{
    class Track;
    class Song;

    namespace App
    {
        class Record;

        /**
         * @ref Record listener interface.
         *
         * @author  Pete Goodliffe
         * @version 3.00
         * @see     Listener
         */
        class RecordListener
        {
            public:
                typedef Record notifier_type;

                /**
                 * The @ref Song and @ref Track parameters are those that were
                 * passed to Record.
                 */
                virtual void Record_RecordingEnded(Record *, Song *, Track *) {}
        };
    }
}

#endif
