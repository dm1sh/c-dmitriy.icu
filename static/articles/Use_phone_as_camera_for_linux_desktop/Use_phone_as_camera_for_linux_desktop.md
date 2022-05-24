First, we need to install an application, that'll provide MJPEG video stream. There is IP Webcam, but it is closed source, so it doesn't fit our requirements, through it could be a simplier solution. We will use [ScreenStream](https://github.com/dkrivoruchko/ScreenStream) instead, that is also [available](https://f-droid.org/packages/info.dvkr.screenstream) on F-Droid. With it we can share anything displayed on the phone screen, so, to stream video from camera, we can use any camera solution.

Next, we need to connect phone with our linux PC. For it you can connect them to the same network. But to lower thresold, you can connect them with a wire, enable USB debugging and forward port with adb:

```
adb forward tcp:8000 tcp:8080
```

Prior command will set up PC' port 8000 to phone's 8080. Also note, then if you use ScreenStream, you need to enable local host in its settings.

Next step is to set up a dummy interface for our camera. For it, install and enable v4l2loopback kernel module:

```
sudo pacman -S v4l2loopback-dkms
```

It will create a dummy video interface on `/dev/video0`. We'll assume, it is enough, but if you need more then one device, you can easely google, what params need to be passed while manually enabling the module.

Then, if you don't have ffmpeg, install it:

```
sudo pacman -S ffmpeg
```

And finally:

```
sudo ffmpeg -i http://[localhost или ip адресс телефона]:8000/stream.mjpeg -fflags nobuffer -pix_fmt yuv420p -r 30 -f v4l2 /dev/video0
```

That's all, now you can use your phone camera (or, any other app) just like your PC camera.