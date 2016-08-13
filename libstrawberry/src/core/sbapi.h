/*****************************************************************************
**                                                                          **
**  This file is part of libstrawberry.                                     **
**                                                                          **
**  libstrawberry is free software: you can redistribute it and/or modify   **
**  it under the terms of the GNU General Public License as published by    **
**  the Free Software Foundation, either version 3 of the License, or       **
**  (at your option) any later version.                                     **
**                                                                          **
**  libstrawberry is distributed in the hope that it will be useful,        **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of          **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           **
**  GNU General Public License for more details.                            **
**                                                                          **
**  You should have received a copy of the GNU General Public License       **
**  along with libstrawberry.  If not, see <http://www.gnu.org/licenses/>.  **
**                                                                          **
******************************************************************************
**
**  Notes:
**    -
**
*/

#ifndef __SB_CORE_SBAPI_H
#define __SB_CORE_SBAPI_H


#include "platform.h"


#if (SB_PLATFORM == SB_PLATFORM_ID_WINDOWS)
#	ifdef LIBSTRAWBERRY_EXPORTS
#		define SBAPI						__declspec(dllexport)
#	else
#		define SBAPI						__declspec(dllimport)
#	endif
#else
#	define SBAPI
#endif


#endif