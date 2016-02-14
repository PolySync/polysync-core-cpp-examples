#include <map>
#include <future>
#include <memory>

#include <PolySyncNode>
#include <PolySyncMessage>
#include <PolySyncDataModel>

using namespace std;

/**
 * @brief The LidarReader class
 * Subclass of polysync::Node. The base class handles registering to the bus.
 * Dynamically gather type data. Register to types. Print data as it comes in.
 *
 */
class LidarReader : public polysync::Node
{
public:
    /**
     * @brief LidarReader constructor
     */
    explicit                                LidarReader();

    /**
     * @brief exec Begins the Backend execution loop
     */
    void                                    execute();

private:
    /**
     * @brief _sleepInterval Sleep between loop iterations.
     */
    static const uint                       _sleepInterval  { 100000 };

    /**
     * @brief _typeNameMap
     */
    map< string, polysync::MessageType  >   _typeNameMap;

    /**
     * @brief buildTypeMap
     * Map MessageType values to string names in a std::map
     */
    void                                    buildTypeList();

    /**
     * @brief registerListeners
     * Register polysync::Node::polysyncCallback() to receive ps_lidar_points_msg
     */
    void                                    registerListeners();

    /**
     * @brief printTypes
     * List each registered message type in the PolySync Data Model.
     */
    void                                    printTypes();


    /**
     * @brief filterLidarPoints
     * Filter LiDAR data for points of interest
     */
    template< typename T >
    void                                    filterLidarPoints(
                                                polysync::Message *message );
};
