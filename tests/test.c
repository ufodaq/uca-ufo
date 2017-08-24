#include <stdlib.h>
#include <string.h>
#include <uca/uca-camera.h>
#include <uca/uca-plugin-manager.h>

UcaPluginManager *manager;
UcaCamera *camera;

static void
dispose (void)
{
    g_object_unref (camera);
    g_object_unref (manager);
}

static void
pass_or_die (GError *error)
{
    if (error == NULL)
        return;

    g_print ("Error: %s\n", error->message);
    dispose ();
    exit (1);
}

static void
print_header (const gchar *header)
{
    guint remaining;

    remaining = 79 - strlen (header);
    g_print ("%s ", header);

    for (guint i = 0; i < remaining; i++)
        g_print ("-");

    g_print ("\n");
}

static void
check_roi_size (UcaCamera *camera, guint x, guint y, guint width, guint height, GError **error)
{
    guint roi_height;
    guint16 *frame;

    g_object_set (camera, "roi-height", height, NULL);
    g_object_get (camera, "roi-height", &roi_height, NULL);

    if (height != roi_height) {
        g_print (" Error: ROI is %u x %u pixels\n", width, roi_height);
        return;
    }

    frame = g_malloc0 (2 * width * height);

    uca_camera_start_recording (camera, error);

    if (*error != NULL)
        goto exit_check_roi_size;

    for (guint i = 0; i < 30; i++) {
        uca_camera_grab (camera, frame, error);

        if (*error != NULL) {
            uca_camera_stop_recording (camera, NULL);
            goto exit_check_roi_size;
        }
    }

    uca_camera_stop_recording (camera, error);

    if (*error != NULL)
        goto exit_check_roi_size;

exit_check_roi_size:
    g_free (frame);
}

static void
test_roi_change (UcaCamera *camera)
{
    guint sensor_width;
    guint sensor_height;
    guint roi_width;
    guint roi_height;

    print_header ("Check different region-of-interest sizes");

    g_object_get (camera,
                  "sensor-width", &sensor_width,
                  "sensor-height", &sensor_height,
                  "roi-width", &roi_width,
                  "roi-height", &roi_height,
                  NULL);

    g_print (" Sensor size: %u x %u pixels\n", sensor_width, sensor_height);

    for (roi_height = sensor_height; roi_height > 8; roi_height /= 2) {
        GError *error = NULL;

        g_print (" Test ROI %u x %u pixels: ", sensor_width, roi_height);

        check_roi_size (camera, 0, 0, sensor_width, sensor_height, &error);

        if (error == NULL) {
            g_print ("PASS\n");
        }
        else {
            g_print ("FAIL (%s)\n", error->message);
            g_error_free (error);
        }
    }
}

int
main (int argc, char **argv)
{
    GError *error = NULL;

    manager = uca_plugin_manager_new ();
    camera = uca_plugin_manager_get_camera (manager, "ufo", &error, NULL);

    pass_or_die (error);

    test_roi_change (camera);

    dispose ();
}
