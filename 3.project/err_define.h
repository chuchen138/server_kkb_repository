#ifndef _ERR_H
#define _ERR_H

#include <stdio.h>
#include <string.h>
#include "friend_info.h"
#include "user_info.h"

#define SUCCESS 0
#define USER_EXIST -1
#define USER_NOT_EXIST -2
#define ALREADY_FRIEND -3
#define ALREADY_BLACK -4
#define USER_NOT_EXIST -5
#define NOT_FRIEND -6

#define USER_TO_MUCH -7
#define BLACK_TO_MUCH -8

#define FRIEND_TYPE 0

#define USER_NEED_DELETE -100
#define IS_FRIEND -200
#define BLACK_NEED_DELETE -201
#define FRIEND_NEED_DELETE -300
#define FRIEND_TO_MUCH -301

#define BLACK_TYPE 1
#define IS_BLACK -302
#define NOT_BLACK -303

#define TYPE_NOT_DEFINE -500
#endif
