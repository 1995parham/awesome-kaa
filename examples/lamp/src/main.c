#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPi.h>

#include <kaa.h>
#include <platform/kaa_client.h>
#include <extensions/notification/kaa_notification_manager.h>
#include <kaa/platform/ext_notification_receiver.h>


static void on_notification(void *context, uint64_t *topic_id, kaa_notification_t *notification)
{
	printf("Received notification on topic %llu: message=%d\n", *topic_id, notification->on);

	if (notification->on) {
		digitalWrite(8, HIGH);
	} else {
		digitalWrite(8, LOW);
	}
}

int main(int argc, char *argv[])
{
	wiringPiSetup();
	pinMode(8, OUTPUT);
	

	/* Prepare Kaa client. */
	kaa_client_t *kaa_client = NULL;
	kaa_error_t error = kaa_client_create(&kaa_client, NULL);
	if (error) {
		return EXIT_FAILURE;
	}
	printf("Hello kaa ...");
	
	kaa_notification_listener_t notification_listener = { on_notification, NULL };
	uint32_t notification_listener_id = 0;

	// Add listener
	kaa_add_notification_listener(kaa_client_get_context(kaa_client)->notification_manager, &notification_listener, &notification_listener_id);
	printf("%d\n", notification_listener_id);


	/* Start Kaa SDK's main loop. example_callback is called once per second. */
	kaa_client_start(kaa_client, NULL, NULL, 1);
	/* Should get here only after Kaa stops. */
	kaa_client_destroy(kaa_client);
}
