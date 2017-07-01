#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <kaa.h>
#include <platform/kaa_client.h>
#include <kaa_configuration_manager.h>
#include <kaa_logging.h>
#include <utilities/kaa_log.h>
#include <kaa/platform-impl/common/ext_log_upload_strategies.h>

#include <pthread.h>
#include <unistd.h>
#include <kaa/gen/kaa_plate_detection_event_definitions.h>
#include <kaa/gen/kaa_plate_detection_event.h>
#include <extensions/user/kaa_user.h>


extern kaa_error_t ext_unlimited_log_storage_create(void **log_storage_context_p, kaa_logger_t *logger);

static void plate_callback(void *context)
{

        kaa_string_t plate;
        kaa_user_log_record_t *log_record = kaa_logging_data_collection_create();
        plate.data = "parham";
        log_record->plate = &plate;
        kaa_logging_add_record(kaa_client_get_context(context)->log_collector, log_record, NULL);
        printf("%s uploaded\n", plate.data);

        kaa_plate_detection_event_plate_event_t *plate_event = kaa_plate_detection_event_plate_event_create();
        plate_event->plate = &plate;
        kaa_error_t error_code = kaa_event_manager_send_kaa_plate_detection_event_plate_event(
                kaa_client_get_context(context)->event_manager, plate_event, NULL);
        //plate_event->destroy(plate_event);
        printf("%s event sent\n", plate.data);
}

void *kaa_thread(void *context)
{
    printf("Hello kaa ...");
    kaa_client_t *kaa_client = (kaa_client_t *) context;
    /* Start Kaa SDK's main loop. example_callback is called once per second. */
    kaa_client_start(kaa_client, NULL, NULL, 1);
    kaa_user_manager_attach_to_user((kaa_user_manager_t *) kaa_client, "user", "", "");
    /* Should get here only after Kaa stops. */
    kaa_client_destroy(kaa_client);
}

int main(int argc, char *argv[])
{

    /* Prepare Kaa client. */
    kaa_client_t *kaa_client = NULL;
    kaa_error_t error = kaa_client_create(&kaa_client, NULL);
    if (error) {
        return EXIT_FAILURE;
    }

    void *log_storage_context         = NULL;
    void *log_upload_strategy_context = NULL;
    /* The internal memory log storage distributed with Kaa SDK. */
    error = ext_unlimited_log_storage_create(&log_storage_context,
        kaa_client_get_context(kaa_client)->logger);
    if (error) {
        return EXIT_FAILURE;
    }
    /* Create a strategy based on timeout. */
    error = ext_log_upload_strategy_create(
        kaa_client_get_context(kaa_client), &log_upload_strategy_context,
        KAA_LOG_UPLOAD_BY_TIMEOUT_STRATEGY);
    if (error) {
        return EXIT_FAILURE;
    }
    /* Strategy will upload logs every 5 seconds. */
    error = ext_log_upload_strategy_set_upload_timeout(log_upload_strategy_context, 5);
    if (error) {
        return EXIT_FAILURE;
    }
    /* Specify log bucket size constraints. */
    kaa_log_bucket_constraints_t bucket_sizes = {
         .max_bucket_size       = 32,   /* Bucket size in bytes. */
         .max_bucket_log_count  = 2,    /* Maximum log count in one bucket. */
    };
    /* Initialize the log storage and strategy (by default, they are not set). */
    error = kaa_logging_init(kaa_client_get_context(kaa_client)->log_collector,
        log_storage_context, log_upload_strategy_context, &bucket_sizes);
    if (error) {
        return EXIT_FAILURE;
    }

    pthread_t thread;
    pthread_create(&thread, NULL, kaa_thread, kaa_client);
    sleep(1);
    plate_callback(kaa_client);


    return EXIT_SUCCESS;
}
