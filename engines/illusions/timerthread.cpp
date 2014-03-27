/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "illusions/illusions.h"
#include "illusions/timerthread.h"
#include "illusions/input.h"
#include "illusions/time.h"

namespace Illusions {

// TimerThread

TimerThread::TimerThread(IllusionsEngine *vm, uint32 threadId, uint32 callingThreadId, uint notifyFlags,
	uint32 duration, bool isAbortable)
	: Thread(vm, threadId, callingThreadId, notifyFlags), _duration(duration), _isAbortable(isAbortable) {
	_type = kTTTimerThread;
	_startTime = getCurrentTime();
	_endTime = _startTime + _duration;
	// TODO _tag = *(_DWORD *)(krndictGetIDValue(callingThreadId) + 20);
}

int TimerThread::onUpdate() {
	if (isTimerExpired(_startTime, _endTime) ||
		(_isAbortable && _vm->_input->pollButton(8)))
		return kTSTerminate;
	return kTSYield;
}

void TimerThread::onSuspend() {
	_durationElapsed = getDurationElapsed(_startTime, _endTime);
}

void TimerThread::onNotify() {
	uint32 currTime = getCurrentTime();
	_startTime = currTime;
	if (_duration <= _durationElapsed)
		_endTime = currTime;
	else
		_endTime = currTime + _duration - _durationElapsed;
	_durationElapsed = 0;
}

void TimerThread::onPause() {
	onSuspend();
}

void TimerThread::onResume() {
	onNotify();
}

void TimerThread::onTerminated() {
	// Empty
}

} // End of namespace Illusions