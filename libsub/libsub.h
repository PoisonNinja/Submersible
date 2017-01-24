#ifndef _LIBSUB_H
#define _LIBSUB_H

#include "Arduino.h"

class SubUtils
{
public:
    static String getValue(String data, char separator, int index);
};

#endif
