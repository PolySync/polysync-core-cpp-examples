#include "Subscriber.hpp"

template<>
void Subscriber<polysync::datamodel::RadarTargetsMessage>::handleEvents( 
        std::shared_ptr<polysync::Message> message )
{
    if( auto inMessage = polysync::datamodel::getSubclass< 
            polysync::datamodel::RadarTargetsMessage >( message ) )
    {
        std::stringstream repeatStream;

        outputHeaderData( repeatStream, inMessage );

        //Wrap all output around Targets/Objects so header and sensor descriptor
        //data repeats
        auto outputTargets = inMessage->getTargets();
        
        std::vector< polysync::datamodel::RadarTarget >::iterator 
            outputTargetsIterator;
      

        for( outputTargetsIterator = outputTargets.begin();
                outputTargetsIterator != outputTargets.end();
                outputTargetsIterator++ )
        {
            auto outputPositions = outputTargetsIterator->getPosition();
            auto outputSize = outputTargetsIterator->getSize();
            auto outputVelocity = outputTargetsIterator->getVelocity();

            _outFile 
                << repeatStream.str()
                << outputTargetsIterator->getId() << ","
                << outputTargetsIterator->getTimestamp() << ","
                << ( uint ) outputTargetsIterator->getNativeTimestampFormat() << ","
                << outputTargetsIterator->getNativeTimestampValue() << ","
                << outputPositions[ 0 ] << ","
                << outputPositions[ 1 ] << ","
                << outputPositions[ 2 ] << ","
                << outputSize[ 0 ] << ","
                << outputSize[ 1 ] << ","
                << outputSize[ 2 ] << ","
                << outputVelocity[ 0 ] << ","
                << outputVelocity[ 1 ] << ","
                << outputVelocity[ 2 ] << ","
                << outputTargetsIterator->getRangeRate() << ","
                << outputTargetsIterator->getTrackStatus() << ","
                << outputTargetsIterator->getRangeType() << ","
                << outputTargetsIterator->getZoneType() << ","
                << outputTargetsIterator->getQuality() << ","
                << outputTargetsIterator->getAmplitude() << ","
                << outputTargetsIterator->getMagnitude() << ","
                << outputTargetsIterator->getAlias() << ","
                << outputTargetsIterator->getCrossSection() << ","
                << outputTargetsIterator->getScanIndex() << "\n";
        }
    }

    return;
}

template<>
void Subscriber< polysync::datamodel::RadarTargetsMessage >::outputCSVHeader()
{
    _outFile << "HeaderType" << ","
        << "HeaderTimestamp" << ","
        << "HeaderSrcGuid" << ","
        << "SensorDescriptorId" << ","
        << "SensorDescriptorType" << ","
        << "SensorDescriptorTransformParentId" << ","
        << "SensorDescriptorTransformTimestamp" << ","
        << "SensorDescriptorTransformStack0Id" << ","
        << "SensorDescriptorTransformStack0Timestamp" << ","
        << "SensorDescriptorTransformStack0Origin0" << ","
        << "SensorDescriptorTransformStack0Origin1" << ","
        << "SensorDescriptorTransformStack0Origin2" << ","
        << "SensorDescriptorTransformStack0Orientation0" << ","
        << "SensorDescriptorTransformStack0Orientation1" << ","
        << "SensorDescriptorTransformStack0Orientation2" << ","
        << "SensorDescriptorTransformStack0Orientation3" << ","
        << "RadarTargetId" << ","
        << "RadarTargetTimestamp" << ","
        << "RadarTargetNativeTimestampFormat" << ","
        << "RadarTargetNativeTimestampValue" << ","
        << "RadarTargetPosition0" << ","
        << "RadarTargetPosition1" << ","
        << "RadarTargetPosition2" << ","
        << "RadarTargetSize0" << ","
        << "RadarTargetSize1" << ","
        << "RadarTargetSize2" << ","
        << "RadarTargetVelocity0" << ","
        << "RadarTargetVelocity1" << ","
        << "RadarTargetVelocity2" << ","
        << "RadarTargetRangeRate" << ","
        << "RadarTargetTrackStatus" << ","
        << "RadarTargetRangeType" << ","
        << "RadarTargetZoneType" << ","
        << "RadarTargetQuality" << ","
        << "RadarTargetAmplitude" << ","
        << "RadarTargetMagnitude" << ","
        << "RadarTargetAlias" << ","
        << "RadarTargetCrossSection" << ","
        << "RadarTargetScanIndex" << "\n";
    
    return;
}

template<>
void Subscriber< polysync::datamodel::LidarPointsMessage >::handleEvents( 
        std::shared_ptr<polysync::Message> message )
{
    if( auto inMessage = 
            polysync::datamodel::getSubclass< 
                polysync::datamodel::LidarPointsMessage >( message ) )
    {
        std::stringstream repeatStream;

        outputHeaderData( repeatStream, inMessage );
        
        repeatStream 
            << inMessage->getStartTimestamp() << ","
            << inMessage->getEndTimestamp() << ","
            << ( uint ) inMessage->getNativeStartTimestampFormat() << ","
            << inMessage->getNativeStartTimestampValue() << ",";

        //Wrap all output around Targets/Objects so header and sensor descriptor 
        //data repeats
        auto outputPoints = inMessage->getPoints();
        
        std::vector<polysync::datamodel::LidarPoint>::iterator 
            outputPointsIterator;
      

        for( outputPointsIterator = outputPoints.begin();
                outputPointsIterator != outputPoints.end();
                outputPointsIterator++ )
        {
            auto position = outputPointsIterator->getPosition();
            
            //Output each target
            if( _outFile.good() )
            {
                _outFile 
                    << repeatStream.str()
                    << position[ 0 ] << ","
                    << position[ 1 ] << ","
                    << position[ 2 ] << ","
                    << ( uint ) outputPointsIterator->getIntensity()
                    << "\n";
            }
            else
            {
                std::cout << "Error writing to file stream..." << std::endl;
            }    
        }
    }

    return;
}

template<>
void Subscriber< polysync::datamodel::LidarPointsMessage >::outputCSVHeader()
{
    _outFile << "HeaderType" << ","
        << "HeaderTimestamp" << ","
        << "HeaderSrcGuid" << ","
        << "SensorDescriptorId" << ","
        << "SensorDescriptorType" << ","
        << "SensorDescriptorTransformParentId" << ","
        << "SensorDescriptorTransformTimestamp" << ","
        << "SensorDescriptorTransformStack0Id" << ","
        << "SensorDescriptorTransformStack0Timestamp" << ","
        << "SensorDescriptorTransformStack0Origin0" << ","
        << "SensorDescriptorTransformStack0Origin1" << ","
        << "SensorDescriptorTransformStack0Origin2" << ","
        << "SensorDescriptorTransformStack0Orientation0" << ","
        << "SensorDescriptorTransformStack0Orientation1" << ","
        << "SensorDescriptorTransformStack0Orientation2" << ","
        << "SensorDescriptorTransformStack0Orientation3" << ","
        << "StartTimestamp" << "," 
        << "EndTimestamp" << ","
        << "NativeStartTimestampFormat" << ","
        << "NativeStartTimestampValue" << ","
        << "PointsPosition0" << ","
        << "PointsPosition1" << ","
        << "PointsPosition2" << ","
        << "PointsIntensity" << "\n";

    return;
}

template<>
void Subscriber< polysync::datamodel::LaneModelMessage >::handleEvents( 
        std::shared_ptr<polysync::Message> message )
{
    if( auto inMessage = 
            polysync::datamodel::getSubclass< 
                polysync::datamodel::LaneModelMessage >( message ) )
    {
        std::stringstream repeatStream;

        outputHeaderData( repeatStream, inMessage );
 
        //Wrap all output around Lanes so header and sensor descriptor 
        //data repeats
        auto outputLanes = inMessage->getLanes();
        
        std::vector<polysync::datamodel::LaneModel>::iterator 
            outputLanesIterator;
      
        for( outputLanesIterator = outputLanes.begin();
                outputLanesIterator != outputLanes.end();
                outputLanesIterator++ )
        {
            _outFile 
                << repeatStream.str()
                << outputLanesIterator->getTimestamp() << ","
                << ( uint ) outputLanesIterator->getNativeTimestampFormat() << ","
                << outputLanesIterator->getNativeTimestampValue() << ","
                << outputLanesIterator->getQuality() << ","
                << outputLanesIterator->getMarkerType() << ","
                << outputLanesIterator->getModelType() << ","
                << outputLanesIterator->getMarkerWidth() << ","
                << outputLanesIterator->getHeadingAngle() << ","
                << outputLanesIterator->getViewRange() << ","
                << outputLanesIterator->getMarkerOffset() << ","
                << outputLanesIterator->getCurvature() << ","
                << outputLanesIterator->getCurvatureDerivative() 
                << ","
                << outputLanesIterator->getTimeToCrossing() << "\n";
        }
    }

    return;
}

template<>
void Subscriber< polysync::datamodel::LaneModelMessage >::outputCSVHeader()
{
    _outFile << "HeaderType" << ","
        << "HeaderTimestamp" << ","
        << "HeaderSrcGuid" << ","
        << "SensorDescriptorId" << ","
        << "SensorDescriptorType" << ","
        << "SensorDescriptorTransformParentId" << ","
        << "SensorDescriptorTransformTimestamp" << ","
        << "SensorDescriptorTransformStack0Id" << ","
        << "SensorDescriptorTransformStack0Timestamp" << ","
        << "SensorDescriptorTransformStack0Origin0" << ","
        << "SensorDescriptorTransformStack0Origin1" << ","
        << "SensorDescriptorTransformStack0Origin2" << ","
        << "SensorDescriptorTransformStack0Orientation0" << ","
        << "SensorDescriptorTransformStack0Orientation1" << ","
        << "SensorDescriptorTransformStack0Orientation2" << ","
        << "SensorDescriptorTransformStack0Orientation3" << ","
        << "LanesTimestamp" << ","
        << "LanesNativeTimestampFormat" << ","
        << "LanesNativeTimestampValue" << ","
        << "LanesQuality" << ","
        << "LanesMarkerType" << ","
        << "LanesModelType" << ","
        << "LanesMarkerWidth" << ","
        << "LanesHeadingAngle" << ","
        << "LanesViewRange" << ","
        << "LanesMarkerOffset" << ","
        << "LanesCurvature" << ","
        << "LanesCurvatureDerivative" << ","
        << "LanesTimeToCrossing" << "\n";

    return;
}

template<>
void Subscriber< polysync::datamodel::ObjectsMessage >::handleEvents( 
        std::shared_ptr<polysync::Message> message )
{
    
    if( auto inMessage = 
            polysync::datamodel::getSubclass< 
                polysync::datamodel::ObjectsMessage >( message ) )
    {
        std::stringstream repeatStream;

        outputHeaderData( repeatStream, inMessage );

        //Wrap all output around Objects so header and sensor descriptor 
        //data repeats
        auto outputObjects = inMessage->getObjects();
        
        std::vector<polysync::datamodel::Object>::iterator 
            outputObjectsIterator;
      
        for( outputObjectsIterator = outputObjects.begin();
                outputObjectsIterator != outputObjects.end();
                outputObjectsIterator++ )
        {
            auto outputPositions = outputObjectsIterator->getPosition();
            auto outputSize = outputObjectsIterator->getSize();
            auto outputVelocity  = outputObjectsIterator->getVelocity();

            _outFile 
                << repeatStream.str()
                << outputObjectsIterator->getId() << ","
                << outputObjectsIterator->getTimestamp() << ","
                << ( uint ) outputObjectsIterator->getNativeTimestampFormat() 
                << ","
                << outputObjectsIterator->getNativeTimestampValue() << ","
                << outputPositions[ 0 ] << ","
                << outputPositions[ 1 ] << ","
                << outputPositions[ 2 ] << ","
                << outputSize[ 0 ] << ","
                << outputSize[ 1 ] << ","
                << outputSize[ 2 ] << ","
                << outputVelocity[ 0 ] << ","
                << outputVelocity[ 1 ] << ","
                << outputVelocity[ 2 ] << ","
                << outputObjectsIterator->getCourseAngle() << ","
                << outputObjectsIterator->getClassification() << ","
                << outputObjectsIterator->getClassificationQuality() << "\n";
        }
    }
    
    return;
}

template<>
void Subscriber< polysync::datamodel::ObjectsMessage >::outputCSVHeader()
{
    _outFile 
        << "HeaderType" << ","
        << "HeaderTimestamp" << ","
        << "HeaderSrcGuid" << ","
        << "SensorDescriptorId" << ","
        << "SensorDescriptorType" << ","
        << "SensorDescriptorTransformParentId" << ","
        << "SensorDescriptorTransformTimestamp" << ","
        << "SensorDescriptorTransformStack0Id" << ","
        << "SensorDescriptorTransformStack0Timestamp" << ","
        << "SensorDescriptorTransformStack0Origin0" << ","
        << "SensorDescriptorTransformStack0Origin1" << ","
        << "SensorDescriptorTransformStack0Origin2" << ","
        << "SensorDescriptorTransformStack0Orientation0" << ","
        << "SensorDescriptorTransformStack0Orientation1" << ","
        << "SensorDescriptorTransformStack0Orientation2" << ","
        << "SensorDescriptorTransformStack0Orientation3" << ","
        << "ObjectsId" << ","
        << "ObjectsTimestamp" << ","
        << "ObjectsNativeTimestampFormat" << ","
        << "ObjectsNativeTimestampValue" << ","
        << "ObjectsPosition0" << ","
        << "ObjectsPosition1" << ","
        << "ObjectsPosition2" << ","
        << "ObjectsSize0" << ","
        << "ObjectsSize1" << ","
        << "ObjectsSize2" << ","
        << "ObjectsVelocity0" << ","
        << "ObjectsVelocity1" << ","
        << "ObjectsVelocity2" << ","
        << "ObjectsCourseAngle" << ","
        << "ObjectsClassification" << ","
        << "ObjectsClassificationQuality" << "\n";

    return;
}

template<>
void Subscriber< polysync::datamodel::PlatformMotionMessage >::handleEvents(
        std::shared_ptr<polysync::Message> message )
{
    
    if( auto inMessage = 
            polysync::datamodel::getSubclass< 
                polysync::datamodel::PlatformMotionMessage >( message ) )
    {
        std::stringstream repeatStream;

        outputHeaderData( repeatStream, inMessage );

        auto outputPosition = inMessage->getPosition();
        auto outputOrientation = inMessage->getOrientation();
        auto outputRotationRate = inMessage->getRotationRate();
        auto outputVelocity = inMessage->getVelocity();
        auto outputAcceleration = inMessage->getAcceleration();

        _outFile
            << repeatStream.str()
            << inMessage->getTimestamp() << ","
            << ( uint ) inMessage->getNativeTimestampFormat() << ","
            << inMessage->getNativeTimestampValue() << ","
            << outputPosition[ 0 ] << ","
            << outputPosition[ 1 ] << ","
            << outputPosition[ 2 ] << ","
            << outputOrientation[ 0 ] << ","
            << outputOrientation[ 1 ] << ","
            << outputOrientation[ 2 ] << ","
            << outputOrientation[ 3 ] << ","
            << outputRotationRate[ 0 ] << ","
            << outputRotationRate[ 1 ] << ","
            << outputRotationRate[ 2 ] << ","
            << outputVelocity[ 0 ] << ","
            << outputVelocity[ 1 ] << ","
            << outputVelocity[ 2 ] << ","
            << outputAcceleration[ 0 ] << ","
            << outputAcceleration[ 1 ] << ","
            << outputAcceleration[ 2 ] << ","
            << inMessage->getHeading() << ","
            << inMessage->getLatitude() << ","
            << inMessage->getLongitude() << ","
            << inMessage->getAltitude() << "\n";
    }
    
    return;
}

template<>
void Subscriber< polysync::datamodel::PlatformMotionMessage >::outputCSVHeader()
{
    _outFile 
        << "HeaderType" << ","
        << "HeaderTimestamp" << ","
        << "HeaderSrcGuid" << ","
        << "SensorDescriptorId" << ","
        << "SensorDescriptorType" << ","
        << "SensorDescriptorTransformParentId" << ","
        << "SensorDescriptorTransformTimestamp" << ","
        << "SensorDescriptorTransformStack0Id" << ","
        << "SensorDescriptorTransformStack0Timestamp" << ","
        << "SensorDescriptorTransformStack0Origin0" << ","
        << "SensorDescriptorTransformStack0Origin1" << ","
        << "SensorDescriptorTransformStack0Origin2" << ","
        << "SensorDescriptorTransformStack0Orientation0" << ","
        << "SensorDescriptorTransformStack0Orientation1" << ","
        << "SensorDescriptorTransformStack0Orientation2" << ","
        << "SensorDescriptorTransformStack0Orientation3" << ","
        << "Timestamp" << ","
        << "NativeTimestampFormat" << ","
        << "NativeTimestampValue" << ","
        << "Position0" << ","
        << "Position1" << ","
        << "Position2" << ","
        << "Orientation0" << ","
        << "Orientation1" << ","
        << "Orientation2" << ","
        << "Orientation3" << ","
        << "RotationRate0" << ","
        << "RotationRate1" << ","
        << "RotationRate2" << ","
        << "Velocity0" << ","
        << "Velocity1" << ","
        << "Velocity2" << ","
        << "Acceleration0" << ","
        << "Acceleration1" << ","
        << "Acceleration2" << ","
        << "Heading" << ","
        << "Latitude" << ","
        << "Longitude" << ","
        << "Altitude" << "\n";

    return;
}

