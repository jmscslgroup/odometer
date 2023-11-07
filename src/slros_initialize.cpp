#include "slros_initialize.h"

ros::NodeHandle * SLROSNodePtr;
const std::string SLROSNodeName = "odometer";

// For Block odometer/Subscribe
SimulinkSubscriber<std_msgs::Float64, SL_Bus_odometer_std_msgs_Float64> Sub_odometer_1;

// For Block odometer/Publish
SimulinkPublisher<std_msgs::Float64, SL_Bus_odometer_std_msgs_Float64> Pub_odometer_4;

// For Block odometer/Get Parameter1
SimulinkParameterGetter<real64_T, double> ParamGet_odometer_10;

void slros_node_init(int argc, char** argv)
{
  ros::init(argc, argv, SLROSNodeName);
  SLROSNodePtr = new ros::NodeHandle();
}

