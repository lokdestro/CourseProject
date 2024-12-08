#include <iostream>
#include <stdio.h>
#include <string.h>
#include "h_cerver.h"

void sys_err (char *message){
    puts (message);
    exit (1);
}

int main (){
    int id_semf;
    int shmid;    // идентификатор разделяемой памяти
    shared_msg *msg_view; // адрес сообщения в разделяемой памяти

    // создание массива семафоров
    if ((id_semf = semget (SEM_ID, 1, PERMS | IPC_CREAT)) < 0)
        sys_err ("server: can not create semaphore");

    // создание сегмента разделяемой памяти
    if ((shmid = shmget (SHM_ID, sizeof (shared_msg), PERMS | IPC_CREAT)) < 0)
        sys_err ("server: can not create shared memory segment");

    // подключение сегмента к адресному пространству процесса
    if ((msg_view = (shared_msg *) shmat (shmid, 0, 0)) == NULL)
        sys_err ("server: shared memory attach error");

    semctl (id_semf, 0, SETVAL, 0); // установка семафора
    msg_view->msg_type = MSG_TYPE_EMPTY;
    std::cout<<"server is waiting...\n";

    while (1){
            if (semctl (id_semf, 0, GETVAL, 0)) // блокировка - ждать
                continue;

            semctl (id_semf, 0, SETVAL, 1);
            if (msg_view->msg_type == MSG_TYPE_STRING) {
                std::cout << "client says: " << msg_view->string << "\n";
            }
            if (msg_view->msg_type == MSG_TYPE_FINISH)
                break;

        msg_view->msg_type = MSG_TYPE_EMPTY; // сообщение обработано
        semctl (id_semf, 0, SETVAL, 0); // снять блокировку
    }

    // удаление массива семафоров
    if (semctl (id_semf, 0, IPC_RMID, (struct semid_ds *) 0) < 0)
        sys_err ("server: semaphore remove error");

    std::cout<<"server finishes work... ";
    shmdt (msg_view);
    // удаление сегмента разделяемой памяти
    if (shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0) < 0)
        sys_err ("server: shared memory remove error");

    exit (0);
}