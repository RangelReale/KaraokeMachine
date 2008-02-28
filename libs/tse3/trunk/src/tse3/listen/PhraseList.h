/*
 * @(#)listen/PhraseList.h 3.00 14 May 1999
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

#ifndef TSE3_LISTEN_PHRASELIST_H
#define TSE3_LISTEN_PHRASELIST_H

namespace TSE3
{
    class PhraseList;
    class Phrase;

    /**
     * @ref PhraseList listener interface.
     *
     * @author  Pete Goodliffe
     * @version 3.00
     * @see     Listener
     */
    class PhraseListListener
    {
        public:
            typedef PhraseList notifier_type;

            virtual void PhraseList_Inserted(PhraseList *, Phrase * /*phr*/) {}
            virtual void PhraseList_Removed(PhraseList *, Phrase * /*phr*/)  {}
    };
}

#endif
