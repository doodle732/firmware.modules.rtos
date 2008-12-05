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

/** \brief TestSuite Implementation File
 **
 ** \file TestSuite.c
 **/

/** \addtogroup TestSuite
 ** @{ */
/** \addtogroup TestSuite_Global
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * v0.1.0 20080713 MaCe	initial version
 */  

/*==================[inclusions]=============================================*/
#include "TS_Internal.h"
#include "Osek_Internal.h"
#include "stdio.h"

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/
TS_ResultType TS_TestResults[(TS_TESTS_COUNT)/4];

TS_ChecksumType TS_Checksum;

/*==================[external functions declaration]=========================*/
#define TestSuite_START_SEC_CODE
#include "MemMap.h"

void TS_PrintResult(unsigned int tc, unsigned char result)
{
	TS_PrintResult_Arch(tc, result);
}

void TS_RunTestSuite(void)
{
	unsigned int loopi;
	unsigned char result;

	for(loopi = 0; loopi < TS_TESTS_COUNT; loopi++)
	{
		result = TestCases[loopi]();
		TS_TestResults[(loopi/sizeof(TS_TestResults))] |= (0x3 & result) << (loopi*2 % 4);
		PreCallService();
		TS_PrintResult(loopi, result);
		PostCallService();	
	}
}

TS_ResultType TS_GetTestResult(TS_TestIDType TestID)
{
	return ( (TS_TestResults[(TestID/sizeof(TS_TestResults))] >> (TestID % 4) ) & 0x3);
}

#define TestSuite_STOP_SEC_CODE
#include "MemMap.h"

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

