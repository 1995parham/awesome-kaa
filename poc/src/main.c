#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <kaa/kaa_error.h>
#include <kaa/platform/kaa_client.h>
#include <kaa/utilities/kaa_log.h>
#include <kaa/kaa_notification_manager.h>
#include <kaa/kaa_profile.h>
#include <kaa/gen/kaa_profile_gen.h>

#define KAA_RETURN_IF_ERROR(error, message) \
	do { \
		if (error) { \
			printf(message ", error code %d\n", (error)); \
				return error; \
		} \
	} while (0)

static kaa_client_t *kaa_client;

void on_notification(void *context, uint64_t *topic_id, kaa_notification_t *notification)
{
	int interval = notification->interval;

	printf("Notification for topic id '%lu' received\n", *topic_id);
	printf("Notification interval: %d\n", interval);


	pid_t pid = fork();
	switch (pid) {
	case -1:
		perror("fork()");
		return;
	case 0:
		setsid();
		system("adb shell bootanimation" );
		return;
	default:
		sleep(interval);
		kill(pid * -1, SIGTERM);
		return;
	}
}

void on_topics_received(void *context, kaa_list_t *topics)
{
	printf("Topic list was updated\n");
	if (!topics || !kaa_list_get_size(topics)) {
		printf("Topic list is empty");
		return;
	}

	kaa_list_node_t *it = kaa_list_begin(topics);

	while (it) {
		kaa_topic_t *topic = (kaa_topic_t *)kaa_list_get_data(it);

		printf("Topic: id '%lu', name: %s\n", topic->id, topic->name);
		it = kaa_list_next(it);
	}
}

int main(int argc, char *argv[])
{
	printf("Kaa SDK client started\n");
	kaa_error_t error_code;
	
	/* Build Kaa client */

	error_code = kaa_client_create(&kaa_client, NULL);
	KAA_RETURN_IF_ERROR(error_code, "Failed create Kaa client");

	/* Build notification related things */

	kaa_topic_listener_t topic_listener = {
		.callback = on_topics_received,
		.context = NULL,
	};
	kaa_notification_listener_t notification_listener = {
		.callback = on_notification,
		.context = NULL,
	};

	uint32_t topic_listener_id = 0;
	uint32_t notification_listener_id = 0;


	error_code = kaa_add_topic_list_listener(
			kaa_client_get_context(kaa_client)->notification_manager,
			&topic_listener,
			&topic_listener_id);
	KAA_RETURN_IF_ERROR(error_code, "Failed add topic listener");

	error_code = kaa_add_notification_listener(
			kaa_client_get_context(kaa_client)->notification_manager,
			&notification_listener,
			&notification_listener_id);
	KAA_RETURN_IF_ERROR(error_code, "Failed add notification listener");

	/* Build profile related things */

	kaa_profile_t *profile = kaa_profile_parham_create();
	profile->grade = 20;
	profile->message = kaa_string_move_create("Hello world", NULL);
	
	error_code = kaa_profile_manager_update_profile(
			kaa_client_get_context(kaa_client)->profile_manager,
			profile);
	KAA_RETURN_IF_ERROR(error_code, "Failed to update profile");

	error_code = kaa_client_start(kaa_client, NULL, NULL, 0);
	KAA_RETURN_IF_ERROR(error_code, "Failed to start Kaa main loop");

	kaa_client_destroy(kaa_client);
	return error_code;
}
