/* Copyright 2008, Mariano Cerdeiro
 *
 * This file is part of OpenSEK.
 *
 * OpenSEK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenSEK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenSEK. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/** \brief OpenSEK Internal Arch Implementation File
 **
 ** \file arm/Osek_Internal_Arch.c
 ** \arch ARM
 **/

/** \addtogroup OpenSEK
 ** @{ */
/** \addtogroup OpenSEK_Internal
 ** @{ */


/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20081116 v0.1.0 MaCe       - initial version
 */

/*==================[inclusions]=============================================*/
#include "Osek_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void CounterInterrupt(CounterType CounterID)
{
	uint8_least loopi;
	AlarmType AlarmID;

	/* increment counter */
	CountersVar[CounterID].Time = ( CountersVar[CounterID].Time + 1 ) % CountersConst[CounterID].MaxAllowedValue;

	/* for alarms on this counter */
	for(loopi = 0; loopi < CountersConst[CounterID].AlarmsCount; loopi++)
	{
		/* get alarm id */
		AlarmID = CountersConst[CounterID].AlarmRef[loopi];

		/* check if the alarm is eanble */
		if (AlarmsVar[AlarmID].AlarmState == 1)
		{
			/* decrement alarm time */
			AlarmsVar[AlarmID].AlarmTime--;

			/* check if alarm time was reached */
			if (AlarmsVar[AlarmID].AlarmTime == 0)
			{
				/* check if new alarm time has to be set */
				if(AlarmsVar[AlarmID].AlarmCycleTime == 0)
				{
					/* disable alarm if no cycle was configured */
					AlarmsVar[AlarmID].AlarmState = 0;
				}
				else
				{
					/* set new alarm cycle */
					AlarmsVar[AlarmID].AlarmTime = AlarmsVar[AlarmID].AlarmCycleTime;
				}

				/* check the alarm action */
				switch(AlarmsConst[AlarmID].AlarmAction)
				{
					case INCREMENT:
						/* call counter function */
						CounterInterrupt(AlarmsConst[AlarmID].AlarmActionInfo.Counter);
						break;
					case ACTIVATETASK:
						/* activate task */
						ActivateTask(AlarmsConst[AlarmID].AlarmActionInfo.TaskID);
						break;
					case ALARMCALLBACK:
						/* callback */
						if(AlarmsConst[AlarmID].AlarmActionInfo.CallbackFunction != NULL)
						{
							AlarmsConst[AlarmID].AlarmActionInfo.CallbackFunction();
						}
						break;
					case SETEVENT:
						/* set event */
						SetEvent(AlarmsConst[AlarmID].AlarmActionInfo.TaskID, AlarmsConst[AlarmID].AlarmActionInfo.Event);
						break;
					default:
						/* some error ? */
						break;
				}
			}
		}
	}
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

