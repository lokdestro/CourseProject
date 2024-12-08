#ifndef SHM_CERVER_H_CERVER_H
#define SHM_CERVER_H_CERVER_H

#endif //SHM_CERVER_H_CERVER_H


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#define SEM_ID	2001  // ключ массива семафоров
#define SHM_ID	2002  // ключ разделяемой памяти
#define PERMS	0666  // права доступа

// коды сообщений

#define MSG_TYPE_EMPTY  0 // пустое сообщение
#define MSG_TYPE_STRING 1 /* тип сообщения о том, что
                             передана непустая строка */
#define MSG_TYPE_FINISH 2 /* тип сообщения о том, что
                             пора завершать обмен */
#define MAX_STRING	120

// структура сообщения, помещаемого в разделяемую память
typedef struct{
    int msg_type;
    char string [MAX_STRING];
} shared_msg;
