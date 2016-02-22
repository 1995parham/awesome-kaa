#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include <kaa/kaa_error.h>
#include <kaa/platform/kaa_client.h>
#include <kaa/utilities/kaa_log.h>
#include <kaa/kaa_notification_manager.h>

#define KAA_DEMO_RETURN_IF_ERROR(error, message) \
    if ((error)) { \
        printf(message ", error code %d\n", (error)); \
        return (error); \
    }

static kaa_client_t *kaa_client = NULL;

void on_notification(void *context, uint64_t *topic_id, kaa_notification_t *notification)
{
    (void)context;
    kaa_string_t *path = (kaa_string_t *) notification->path;
    printf("Notification for topic id '%lu' received\n", *topic_id);
    printf("Notification path: %s\n", path->data);
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

int main()
{
    printf("Kaa SDK client started\n");

    kaa_error_t error_code = kaa_client_create(&kaa_client, NULL);
    KAA_DEMO_RETURN_IF_ERROR(error_code, "Failed create Kaa client");

    kaa_topic_listener_t topic_listener = { &on_topics_received, kaa_client };
    kaa_notification_listener_t notification_listener = { &on_notification, kaa_client };

    uint32_t topic_listener_id = 0;
    uint32_t notification_listener_id = 0;

    error_code = kaa_add_topic_list_listener(kaa_client_get_context(kaa_client)->notification_manager
                                           , &topic_listener
                                           , &topic_listener_id);
    KAA_DEMO_RETURN_IF_ERROR(error_code, "Failed add topic listener");

    error_code = kaa_add_notification_listener(kaa_client_get_context(kaa_client)->notification_manager
                                             , &notification_listener
                                             , &notification_listener_id);
    KAA_DEMO_RETURN_IF_ERROR(error_code, "Failed add notification listener");

    error_code = kaa_client_start(kaa_client, NULL, NULL, 0);
    KAA_DEMO_RETURN_IF_ERROR(error_code, "Failed to start Kaa main loop");

    kaa_client_destroy(kaa_client);

    return error_code;
}
