#include <iostream>
#include <fstream>
#include <ctime>
#include <yaml-cpp/yaml.h> //as yaml-cpp's library is outside of code base
#include "ros/ros.h"
#include <sensor_msgs/SetCameraInfo.h>

using namespace std;

string PATH = "/home/osrc/catkin_ws/src/ovc3/param/";

bool add(sensor_msgs::SetCameraInfo::Request  &req,
         sensor_msgs::SetCameraInfo::Response &res) {
   time_t t = time(0); //t is an integer type

   string filename = PATH + to_string(t) + "_left.yaml";
   ofstream newfile(filename);

   YAML::Emitter out;
   out << YAML::BeginMap;

   out << YAML::Key << "image_width" << YAML::Value << req.camera_info.width;
   out << YAML::Key << "image_height" << YAML::Value << req.camera_info.height;
   out << YAML::Key << "camera_name" << YAML::Value << "narrow_stereo/left";

   out << YAML::Key << "camera_matrix" << YAML::Value << YAML::BeginMap;
      out << YAML::Key << "rows" << YAML::Value << 3;
      out << YAML::Key << "cols" << YAML::Value << 3;
      out << YAML::Key << "data";
      out << YAML::Value << YAML::Flow << YAML::BeginSeq;
        for (int i = 0; i < 9; i++) out << req.camera_info.K[i];
        out << YAML::EndSeq;
      out << YAML::EndMap;

   out << YAML::Key << "distortion_model" << YAML::Value << "plumb_bob";

   out << YAML::Key << "distortion_coefficients" << YAML::Value << YAML::BeginMap;
      out << YAML::Key << "rows" << YAML::Value << 1;
      out << YAML::Key << "cols" << YAML::Value << 5;
      out << YAML::Key << "data";
      out << YAML::Value << YAML::Flow << YAML::BeginSeq;
        for (int i = 0; i < 5; i++) out << req.camera_info.D[i];
        out << YAML::EndSeq;
      out << YAML::EndMap;

   out << YAML::Key << "rectification_matrix" << YAML::Value << YAML::BeginMap;
      out << YAML::Key << "rows" << YAML::Value << 3;
      out << YAML::Key << "cols" << YAML::Value << 3;
      out << YAML::Key << "data";
      out << YAML::Value << YAML::Flow << YAML::BeginSeq;
        for (int i = 0; i < 9; i++) out << req.camera_info.R[i];
        out << YAML::EndSeq;
      out << YAML::EndMap;

   out << YAML::Key << "distortion_coefficients" << YAML::Value << YAML::BeginMap;
      out << YAML::Key << "rows" << YAML::Value << 3;
      out << YAML::Key << "cols" << YAML::Value << 4;
      out << YAML::Key << "data";
      out << YAML::Value << YAML::Flow << YAML::BeginSeq;
        for (int i = 0; i < 12; i++) out << req.camera_info.P[i];
        out << YAML::EndSeq;
      out << YAML::EndMap;

   out << YAML::EndMap;

   newfile << out.c_str() << endl;
   newfile.close();
   newfile.clear();

   //2nd file
   string leftfile = PATH + "left_latest.yaml";
   ofstream file(leftfile);
   file << out.c_str() << endl;

   res.success = true;
   res.status_message = "Calibration Successful";

   ROS_INFO("sending back response: %d", res.success);
   ROS_INFO("sending back response: %s", res.status_message.c_str());
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "set_left_camera_info_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("left_camera/set_camera_info", add);
  ROS_INFO("Left Server is Up");
  ros::spin();

  return 0;
}
