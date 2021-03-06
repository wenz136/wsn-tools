/* File: atoi-gen.c

   Copyright (C) 2013 David Hauweele <david@hauweele.net>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <err.h>

#include "atoi-gen.h"

static unsigned int symbol_value(char c)
{
  /* digit */
  if(c <= '9')
    return c - '0';

  /* lowercase */
  if(c > 'Z')
    return c - 'a' + 10;

  /* uppercase */
  return c - 'A' + 10;
}

int atoi_gen(const char *s)
{
  int sgn = 1;
  int val = 0;
  unsigned int base = 10;

#define ZERO_END(c) if(c == '\0') goto RESULT

  /* Skip leading spaces */
  for(; isspace(*s) ; s++)
    ZERO_END(*s);

  ZERO_END(*s);

  if(*s == '-') {
    sgn = -1;
    ZERO_END(*++s);
  }

  if(*s == '0') {
    ZERO_END(*++s);

    switch(*s) {
    case 'x':
    case 'X':
      base = 16;
      s++;
      break;
    case 'b':
    case 'B':
      base = 2;
      s++;
      break;
    default:
      base = 8;
    }
  }

  /* Convert from base */
  for(; !isspace(*s) ; s++) {
    ZERO_END(*s);

    val *= base;
    val += symbol_value(*s);
  }

  for(; isspace(*s) ; s++)
    ZERO_END(*s);

RESULT:
  return val * sgn;
}

static bool ishex(char c)
{
  if((c >= '0' && c <= '9') ||
     (c >= 'A' && c <= 'F') ||
     (c >= 'a' && c <= 'f'))
    return true;
  return false;
}

const char * parse_hex_until(const char *s, const char *delim,
                             unsigned int *v, const char *error_message,
                             bool accept_zero)
{
  unsigned int val = 0;
  const char *d;

  if(!ishex(*s))
    errx(EXIT_FAILURE,"%s: expect an hexadecimal value", error_message);

  for(; *s != '\0' ; s++) {
    d = strchr(delim, *s);
    if(d)
      goto EXIT;
    if(!ishex(*s))
      errx(EXIT_FAILURE, "%s: expect an hexadecimal value", error_message);

    val <<= 4;
    val += symbol_value(*s);
  }

  if(!accept_zero)
    errx(EXIT_FAILURE, "%s: premature ending", error_message);

EXIT:
  *v = val;
  return s;
}
