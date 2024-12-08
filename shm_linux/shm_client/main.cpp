#include <stdio.h>
#include <string.h>
#include <iostream>
#include "h_client.h"

void sys_err (char *message){
    puts (message);
    exit (1);
}

int main (){
    int id_semf;
    int shmid;  // идентификатор разделяемой памяти
    shared_msg *msg_view;  //адрес сообщения в разделяемой памяти
    char msg[MAX_STRING];

    // получение доступа к массиву семафоров
    if ((id_semf = semget (SEM_ID, 1, 0)) < 0)
        sys_err ("client: can not get semaphore");

    // получение доступа к сегменту разделяемой памяти
    if ((shmid = shmget (SHM_ID, sizeof (shared_msg), 0)) < 0)
        sys_err ("client: can not get shared memory segment");

    // получение адреса сегмента
    if ((msg_view = (shared_msg *) shmat (shmid, 0, 0)) == NULL)
        sys_err ("client: shared memory attach error");

    while (1){
        std::cout<<"type message to server to exit type 1 character: ";
        std::cin >> msg;
        //если сообщение не обработано или сегмент блокирован - ждать
        while (semctl (id_semf, 0, GETVAL, 0) || msg_view->msg_type != MSG_TYPE_EMPTY);
        semctl (id_semf, 0, SETVAL, 1);

        if (strlen (msg) > 1){
            msg_view->msg_type = MSG_TYPE_STRING;
            strncpy (msg_view->string, msg, MAX_STRING);
        }
        else {
            msg_view->msg_type = MSG_TYPE_FINISH;

        };
        semctl (id_semf, 0, SETVAL, 0);// отменить блокировку
        if (strlen (msg) == 1)break;

    }
    std::cout<<"client finishes work... ";
    shmdt (msg_view);  // отсоединить сегмент разделяемой памяти
    exit (0);
}