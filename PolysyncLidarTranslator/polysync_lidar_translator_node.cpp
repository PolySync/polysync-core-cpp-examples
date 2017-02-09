#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/PointField.h>
#include <polysync_ros_bridge/ps_lidar_points_msg.h>


using namespace sensor_msgs;




//
void addFieldToPointCloud(
        PointCloud2 * cloud,
        std::string name,
        int offset,
        int dataType,
        int count )
{
    PointField field;

    field.name = name;

    field.offset = offset;

    field.datatype = dataType;

    field.count = count;

    cloud->fields.push_back( field );
}


//
void publishLidarTransform(
        std::string frame_id,
        double x,
        double y,
        double z,
        double roll,
        double pitch,
        double yaw )
{
    static tf::TransformBroadcaster br;

    tf::Transform transform;

    transform.setOrigin( tf::Vector3( x, y, z ) );

    tf::Quaternion q;

    q.setRPY( roll, pitch, yaw );

    transform.setRotation(q);

    br.sendTransform( tf::StampedTransform(
            transform,
            ros::Time::now(),
            "world",
            frame_id ) );
}


// CallBack for PolySync Lidar Points
void lidarCallback(
        const polysync_ros_bridge::ps_lidar_points_msg::ConstPtr& msg,
        ros::Publisher * lidar_pub )
{
    PointCloud2 cloud;

    ros::Time timer;

    // Get the source GUID as a string
    std::ostringstream src_guid_ss;
    src_guid_ss << msg->ps_header.src_guid;
    std::string src_guid = src_guid_ss.str();

    // get PS timestamp in nanoseconds
    unsigned long long ps_nanoseconds = msg->ps_header.timestamp*1000;

    //Consistant timestamp for all lidar points in this single message
    uint headerSize = sizeof( msg->points[0].header );
    uint dataPointSize = sizeof( msg->points[0] ) - headerSize;
    uint pointSize = sizeof(msg->points[0].position) / 3;

    uint dataPointsSize = msg->points.size() * ( headerSize + dataPointSize );

    cloud.header.frame_id = src_guid;
    cloud.header.stamp = timer.fromNSec( ps_nanoseconds );

    cloud.is_bigendian = false;
    cloud.is_dense = true;

    cloud.height = 1;

    // number of points in message
    cloud.width = msg->points.size();

    cloud.point_step = headerSize + dataPointSize;
    cloud.row_step = dataPointsSize;


    addFieldToPointCloud(
            &cloud,
            "x",
            headerSize,
            PointField::FLOAT32,
            msg->points.size() );

    addFieldToPointCloud(
            &cloud,
            "y",
            headerSize + pointSize*1,
            PointField::FLOAT32,
            msg->points.size() );

    addFieldToPointCloud(
            &cloud,
            "z",
            headerSize + pointSize*2,
            PointField::FLOAT32,
            msg->points.size() );

    // intensity
    addFieldToPointCloud(
            &cloud,
            "i",
            headerSize + pointSize*3,
            PointField::UINT8,
            msg->points.size() );

    cloud.data.resize( dataPointsSize );

    std::memcpy(
            cloud.data.data(),
            msg->points.data(),
            dataPointsSize );

    //Publish array of markers as a single message
    lidar_pub->publish( cloud );

    // TODO : get transform values from PolySync SDF, for a specific GUID, and
    // load them in here
    publishLidarTransform( src_guid, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 );
}




//
int main( int argc, char** argv )
{
    ros::init( argc, argv, "polysync_lidar_translator" );

    ros::NodeHandle rosnode;

    ros::Publisher lidar_pub;

    ros::Subscriber lidar_sub;

    lidar_pub =
            rosnode.advertise < sensor_msgs::PointCloud2 >
                    ( "polysync/lidar_points",
                    1 );

    lidar_sub =
            rosnode.subscribe < polysync_ros_bridge::ps_lidar_points_msg >
                    ( "polysync/ps_lidar_points_msg",
                    1000,
                    boost::bind( lidarCallback, _1, &lidar_pub ) );

    ros::spin();

    return 0;

}
