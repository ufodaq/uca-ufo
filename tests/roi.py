import argparse
import PyTango
import tifffile


def grab(camera, height=3840, offset=0):
    if height < 3840:
        camera.roi_y0 = offset
        camera.roi_height = height
    else:
        camera.roi_height = height
        camera.roi_y0 = offset

    try:
        camera.Start()
    except:
        camera.Stop()
        camera.Start()

    frame = camera.image
    tifffile.imsave('frame-{}-{}.tif'.format(offset, height), frame)
    camera.Stop()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--device', '-d', type=str, required=True,
                        help="TANGO device path")

    args = parser.parse_args()

    camera = PyTango.DeviceProxy(args.device)
    camera.trigger_source = 0

    grab(camera)
    grab(camera, height=3640)
    grab(camera, height=3640, offset=200)
