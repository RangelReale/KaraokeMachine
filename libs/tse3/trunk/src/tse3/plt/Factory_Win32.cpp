/*
 * @(#)Factry_Win32.cpp 3.00 5 February 2002
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

#include "tse3/plt/Factory.h"

#ifdef CONFIG_H
#include "config.h"
#endif

#include "tse3/plt/Win32.h"

#include "tse3/util/MidiScheduler.h"

#include "tse3/Error.h"

using namespace std;
using namespace TSE3;
using namespace TSE3::Plt;

/******************************************************************************
 * Win32 MidiSchedulerFactory class
 *****************************************************************************/

MidiSchedulerFactory::MidiSchedulerFactory(bool b)
: _canReturnNull(b)
{
}


MidiSchedulerFactory::~MidiSchedulerFactory()
{
}


MidiScheduler *MidiSchedulerFactory::createScheduler()
{
    try
    {
        Win32MidiScheduler *ms = new Win32MidiScheduler();
        cout << "Created new Win32MidiScheduler seccussfully\n";
        return ms;
    }
    catch (MidiSchedulerError)
    {
        cout << "Failed to create a Win32MidiScheduler\n";
        if (_canReturnNull)
        {
            return new TSE3::Util::NullMidiScheduler();
        }
        else
        {
            throw;
        }
    }
}

/*

TSE3::MidiSchedulerFactory::MidiSchedulerFactory(bool c)
: _canReturnNull(c)
{
}


TSE3::MidiSchedulerFactory::~MidiSchedulerFactory()
{
}


TSE3::MidiScheduler *TSE3::MidiSchedulerFactory::createScheduler()
{
    try
    {
        Win32MidiScheduler *ms = new Win32MidiScheduler();
        return ms;
    }
    catch (...)
    {
        cout << "Failed to create a Win32MidiScheduler\n";
        throw;
    }
}

*/
