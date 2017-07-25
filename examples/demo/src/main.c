#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <kaa.h>
#include <platform/kaa_client.h>

#include <pthread.h>
#include <unistd.h>


static void plate_callback(void *context)
{
	printf("Callback is here")
}

void *kaa_thread(void *context)
{
    printf("Hello kaa ...");
    kaa_client_t *kaa_client = (kaa_client_t *) context;
    /* Start Kaa SDK's main loop. example_callback is called once per second. */
    kaa_client_start(kaa_client, NULL, NULL, 1);
    /* Should get here only after Kaa stops. */
    kaa_client_destroy(kaa_client);
}

int main(int argc, char *argv[])
{
    pthread_t thread;
    pthread_create(&thread, NULL, kaa_thread, kaa_client);
    sleep(1);
    plate_callback(kaa_client);


    return EXIT_SUCCESS;
}
