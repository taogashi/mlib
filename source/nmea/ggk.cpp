#include "nmeac.h"
#include <mlib/convert.h>

/**
	GGK sentence.

  $ttGGK,hhmmss,mmddyy,xxxx.xx,N,xxxxx.xx,W,q,s,dop,geo,M

  \return
    - 1 if valid statement
    - 0 if invalid statement

  \note
    This is not a true NMEA standard sentence, however it is generated by quite a few
    GPS units.
*/
int ggk (const char *buf, double *lat, double *lon, double *time, double *height,
         double *dop, int *sat, int *mode)
{
  parse_context ctx(buf);

  char *tok = token (ctx);
  if (!tok || memcmp (tok+3, "GGK", 3))
    return 0;

  NEXT_VALIDTOKEN (tok, 0);
  IFPAR (time, atof (tok));
  NEXT_TOKEN (tok, 0);        //date
  NEXT_TOKEN (tok, 0);
  IFPAR (lat, DMD2rad (atof (tok)));
  NEXT_TOKEN (tok, 0);
  if (lat && *tok && *tok == 'S')
    *lat *= -1.;
  NEXT_TOKEN (tok, 0);
  IFPAR (lon, DMD2rad(atof (tok)));
  NEXT_TOKEN (tok, 0);
  if (lon && *tok && *tok == 'W' )
    *lon *= -1.;
  NEXT_TOKEN (tok, 0);
  IFPAR (mode, atoi (tok));
  NEXT_TOKEN (tok, 0);
  IFPAR (sat, atoi (tok));
  NEXT_TOKEN (tok, 0);
  IFPAR (dop, atof (tok));
  NEXT_TOKEN (tok, 1);            //height is optional
  if (height)
  {
    //POS MV sends ellipsoidal height prefixed with "EHT"
    if (!strncmp (tok, "EHT", 3))
      tok += 3;
    *height = atof (tok);
  }
  return 1;
}

