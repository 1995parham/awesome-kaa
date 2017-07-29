#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <kaa.h>
#include <platform/kaa_client.h>
#include <extensions/notification/kaa_notification_manager.h>
#include <kaa/platform/ext_notification_receiver.h>

#include <pthread.h>
#include <unistd.h>

static void on_notification(void *context, uint64_t *topic_id, kaa_notification_t *notification)
{
	char *message = (char *) notification->text->data;

	printf("Received notification on topic %llu: message=%s\n", *topic_id, message);
}


static void plate_callback()
{
	printf("Callback is here");
}

void *kaa_thread(void *context)
{
	printf("Hello kaa ...");
	
	kaa_client_t *kaa_client = (kaa_client_t *) context;

	kaa_notification_listener_t notification_listener = { on_notification, NULL };
	uint32_t notification_listener_id = 0;

	// Add listener
	kaa_add_notification_listener(kaa_client_get_context(kaa_client)->notification_manager, &notification_listener, &notification_listener_id);
	printf("%d\n", notification_listener_id);


	/* Start Kaa SDK's main loop. example_callback is called once per second. */
	kaa_client_start(kaa_client, NULL, NULL, 1);
	/* Should get here only after Kaa stops. */
	kaa_client_destroy(kaa_client);

	return NULL;
}

int main(int argc, char *argv[])
{
	/* Prepare Kaa client. */
	kaa_client_t *kaa_client = NULL;
	kaa_error_t error = kaa_client_create(&kaa_client, NULL);
	if (error) {
		return EXIT_FAILURE;
	}

	pthread_t thread;
	pthread_create(&thread, NULL, kaa_thread, kaa_client);
	sleep(1);
	plate_callback();
	
	while(1);
}
