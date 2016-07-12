import time
import argparse
import PyTango


def grab(camera, timeout, do_trigger):
    camera.timeout = timeout
    camera.trigger_source = 1

    try:
        camera.Start()
    except:
        camera.Stop()
        camera.Start()

    start = time.time()

    try:
        if do_trigger:
            camera.Trigger()

        frame = camera.image
        end = time.time()
        print("Success after {} s".format(end -start))
    except PyTango.DevFailed:
        end = time.time()
        print("Timeout after {} s".format(end - start))

    camera.Stop()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--device', '-d', type=str, help="TANGO device path")

    args = parser.parse_args()

    camera = PyTango.DeviceProxy(args.device)
    grab(camera, 100000, False)
    grab(camera, 100000, True)
    grab(camera, 300000, False)
    grab(camera, 300000, True)
