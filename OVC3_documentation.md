## OVC3
### 1. Setup

#### Assigning IP to machine
OVC 3 is running a full Ubuntu distribution and a ROS node in itself. On startup, it will configure itself as a USB ethernet gadget with a static IP of 10.0.1.1 and ia DHCP server that will assign an IP of 10.0.1.2 to the connected host, it will then look for a ROS master in 10.0.1.2, once it is found it will start publishing the images and IMU data.

**The board should automatically assign an IP to your machine,** however if this doesn't work you can manually configure your PC to have a static IP. To do so, edit your */etc/network/interfaces* file and add the following:
```
allow-hotplug enp9s0u1
iface enp9s0u1 inet static
  address 10.0.1.2
  netmask 255.255.255.0
  mtu 13500
  pre-up sleep 5 # Hack to wait for ovc networking to restart
```
Please note, the name of the interface (enp9s0u1 in the example) might be different on your machine, to find out the interface name plug the OVC, wait for it to boot up and run `ip a` on your machine.

(You can run `watch -n 1 ip a` instead to make it check the ip for every second.)

#### Checking ROS topic
First, ensure `roscore` is running. Upon booting of the OVC, `ovc_bringup.sh` would automatically run, sourcing and running `rosrun ovc_embedded_driver ovc_embedded_driver`. In the user's PC, running `$ rostopic list` should display the following
```
osrc@beepboop:~$ rostopic list
/image
/ovc/imu
/ovc/left/image_metadata
/ovc/left/image_raw
/ovc/rgb/image_metadata
/ovc/rgb/image_raw
/ovc/right/image_metadata
/ovc/right/image_raw
/rosout
/rosout_agg
```
(There may be a slight variation in the topic names in future revisions.)

###### An additional debugging step:
To ensure that images are publishing on `/image_raw`, use `rostopic hz <topic_name/image_raw>`.

*Below is an example of topics that have images successfully published to.*
```
osrc@beepboop:~$ rostopic info /ovc/left/image_raw
Type: sensor_msgs/Image

Publishers:
 * /ovc_embedded_driver_node (http://10.0.1.1:42383/)

Subscribers:
 * /cameracalibrator (http://beepboop:40787/)

osrc@beepboop:~$ rostopic hz /ovc/left/image_raw
subscribed to [/ovc/left/image_raw]
average rate: 32.860
	min: 0.029s max: 0.031s std dev: 0.00028s window: 31
average rate: 32.841
	min: 0.029s max: 0.031s std dev: 0.00025s window: 63
average rate: 32.830
	min: 0.029s max: 0.032s std dev: 0.00030s window: 96
average rate: 32.824
	min: 0.029s max: 0.032s std dev: 0.00027s window: 129
average rate: 32.826
	min: 0.029s max: 0.032s std dev: 0.00032s window: 162
average rate: 32.824
```
If there are no `/image_raw` images being published onto the `/ovc/left/image_raw`topic, disconnecting and restarting the OVC connect is a possible fix.
`shh`-ing into the OVC, running `sudo bash` and ` rosrun ovc_embedded_driver ovc_embedded_driver_node` may be a possible fix as well.

### 2. ovc_bringup.sh
```
  #!/bin/bash

  # Start by checking if we need to go in recovery mode.
  # The following script will either return immediately or update and shut down the OVC

  sh /home/ubuntu/recovery/recovery.sh

  sleep 5
  source /opt/ros/melodic/setup.bash
  source /home/ubuntu/catkin_ws/devel/setup.bash
  export ROS_IP=10.0.1.1
  export ROS_MASTER_URI=http://10.0.1.2:11311

  rosrun ovc_embedded_driver ovc_embedded_driver_node
```
Added to
