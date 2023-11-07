#ifndef _SLROS_INITIALIZE_H_
#define _SLROS_INITIALIZE_H_

#include "slros_busmsg_conversion.h"
#include "slros_generic.h"
#include "odometer_types.h"

extern ros::NodeHandle * SLROSNodePtr;
extern const std::string SLROSNodeName;

// For Block odometer/Subscribe
extern SimulinkSubscriber<std_msgs::Float64, SL_Bus_odometer_std_msgs_Float64> Sub_odometer_1;

// For Block odometer/Publish
extern SimulinkPublisher<std_msgs::Float64, SL_Bus_odometer_std_msgs_Float64> Pub_odometer_4;

// For Block odometer/Get Parameter1
extern SimulinkParameterGetter<real64_T, double> ParamGet_odometer_10;

void slros_node_init(int argc, char** argv);

#endif
