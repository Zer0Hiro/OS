#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem_orders;
sem_t sem_cooked;
sem_t sem_packed;

int order_id = 1;

void* cashier_worker(void* arg) {
    while (1) {
        int current_order = order_id++;
        
        printf("[Cashier] Taking order #%d...\n", current_order);
        sleep(1);
        printf("[Cashier] Order #%d placed successfully.\n", current_order);

        sem_post(&sem_orders);
    }
    return NULL;
}

void* cook_worker(void* arg) {
    while (1) {
        sem_wait(&sem_orders);

        int current_order = order_id;
    
        printf("\t[Cook] Food for order #%d is ready.\n", current_order);

        sem_post(&sem_cooked);
    }
    return NULL;
}

void* packer_worker(void* arg) {
    while (1) {
        sem_wait(&sem_cooked);

        int current_order = order_id;
        
        printf("\t\t[Packer] Packaging order #%d...\n", current_order);


        sem_post(&sem_packed);
    }
    return NULL;
}

void* server_worker(void* arg) {
    while (1) {
        sem_wait(&sem_packed);

        int current_order = order_id;
        
        printf("\t\t\t[Server] Order #%d completed!\n", current_order);
        printf("--------------------------------------------------\n");
    }
    return NULL;
}

int main() {
    sem_init(&sem_orders, 0, 0);
    sem_init(&sem_cooked, 0, 0);
    sem_init(&sem_packed, 0, 0);

    pthread_t cashier_thread, cook_thread, packer_thread, server_thread;

    pthread_create(&cashier_thread, NULL, cashier_worker, NULL);
    pthread_create(&cook_thread, NULL, cook_worker, NULL);
    pthread_create(&packer_thread, NULL, packer_worker, NULL);
    pthread_create(&server_thread, NULL, server_worker, NULL);

    sleep(10);
    printf("Restaurant is closing.\n");
    exit(0);
}