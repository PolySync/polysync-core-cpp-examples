
#include <iostream>

#include <PolySyncLogFile.hpp>
#include <PolySyncNode.hpp>


using namespace std;
using namespace polysync;


LogfileAttributes::LogfileAttributes()
{
    _logfileAttributes = new ps_logfile_attributes;

    memset( _logfileAttributes, 0, sizeof(*_logfileAttributes) );
}


LogfileAttributes::LogfileAttributes( const ps_logfile_attributes & attributes )
{
    _logfileAttributes = new ps_logfile_attributes;

    memset( _logfileAttributes, 0, sizeof(*_logfileAttributes) );

    // copy:
    _logfileAttributes->start_time = attributes.start_time;

    _logfileAttributes->end_time = attributes.end_time;

    _logfileAttributes->duration = attributes.duration;

    _logfileAttributes->file_size = attributes.file_size;

    _logfileAttributes->data_offset = attributes.data_offset;

    _logfileAttributes->data_count = attributes.data_count;

    _logfileAttributes->avg_data_size = attributes.avg_data_size;
}


LogfileAttributes::LogfileAttributes( const LogfileAttributes & attributes )
{
    _logfileAttributes = new ps_logfile_attributes;

    memset( _logfileAttributes, 0, sizeof(*_logfileAttributes) );

    // copy:
    _logfileAttributes->start_time = attributes.getStartTime();

    _logfileAttributes->end_time = attributes.getEndTime();

    _logfileAttributes->duration = attributes.getDuration();

    _logfileAttributes->file_size = attributes.getFileSize();

    _logfileAttributes->data_offset = attributes.getDataOffset();

    _logfileAttributes->data_count = attributes.getDataCount();

    _logfileAttributes->avg_data_size = attributes.getAvgDataSize();
}


LogfileAttributes::~LogfileAttributes()
{
    delete _logfileAttributes;
}


void LogfileAttributes::setLogHeader(ps_rnr_log_header logHeader)
{
    _logfileAttributes->log_header = logHeader;
}


void LogfileAttributes::setStartTime(ps_timestamp startTime)
{
    _logfileAttributes->start_time = startTime;
}


void LogfileAttributes::setDuration(ps_timestamp duration)
{
    _logfileAttributes->duration = duration;
}


void LogfileAttributes::setFileSize(ulong fileSize)
{
    _logfileAttributes->file_size = fileSize;
}


void LogfileAttributes::setDataOffset(ulong dataOffset)
{
    _logfileAttributes->data_offset = dataOffset;
}


void LogfileAttributes::setDataCount(ulong dataCount)
{
    _logfileAttributes->data_count = dataCount;
}


void LogfileAttributes::setAvgDataSize(ulong avgDataSize)
{
    _logfileAttributes->avg_data_size = avgDataSize;
}


ps_rnr_log_header LogfileAttributes::getLogHeader() const
{
    return _logfileAttributes->log_header;
}


ps_timestamp LogfileAttributes::getStartTime() const
{
    return _logfileAttributes->start_time;
}


ps_timestamp LogfileAttributes::getEndTime() const
{
    return _logfileAttributes->end_time;
}


ps_timestamp LogfileAttributes::getDuration() const
{
    return _logfileAttributes->duration;
}


ulong LogfileAttributes::getFileSize() const
{
    return _logfileAttributes->file_size;
}


ulong LogfileAttributes::getDataOffset() const
{
    return _logfileAttributes->data_offset;
}


ulong LogfileAttributes::getDataCount() const
{
    return _logfileAttributes->data_count;
}


ulong LogfileAttributes::getAvgDataSize() const
{
    return _logfileAttributes->avg_data_size;
}


ps_logfile_attributes * LogfileAttributes::getFileAttributes() const
{
    return _logfileAttributes;
}


RnrLogRecord::RnrLogRecord()
{}


RnrLogRecord::RnrLogRecord( const ps_rnr_log_record & )
{}


RnrLogRecord::RnrLogRecord( const RnrLogRecord & )
{}


RnrLogRecord::~RnrLogRecord()
{}

unsigned int RnrLogRecord::getIndex()
{
    return _rnrLogRecord->index;
}


unsigned int RnrLogRecord::getSize()
{
    return _rnrLogRecord->size;
}

unsigned int RnrLogRecord::getPreviousSize()
{
    return _rnrLogRecord->prev_size;
}


unsigned int RnrLogRecord::getTimestamp()
{
    return _rnrLogRecord->timestamp;
}


ps_rnr_log_record * RnrLogRecord::getRnrLogRecord() const
{
    return _rnrLogRecord;
}


Logfile::Logfile ( Node & node ) : _node ( node.getNodeReference() )
{
    // initialize a node's logfile API resources
      checkDTC( psync_logfile_init( _node ),
                  "Failed to initialize LogFile Class." );
}


Logfile::~Logfile()
{
    // releasing a node's logfile API resources
    checkDTC( psync_logfile_release( _node ),
                "Failed to release node's logfile resources." );
}


void Logfile::setModeRead()
{
    setMode( LOGFILE_MODE_READ, _sessionId );
}


void Logfile::setModeWrite()
{
    setMode( LOGFILE_MODE_WRITE, _sessionId );
}


void Logfile::setModeOff()
{
    setMode( LOGFILE_MODE_OFF, _sessionId );
}


void Logfile::setStateEnabled( ps_timestamp offsetToStart )
{
    setState( LOGFILE_STATE_ENABLED, offsetToStart );
}


void Logfile::setStateDisabled()
{
    setState( LOGFILE_STATE_DISABLED, 1000 );
}


void Logfile::setStateInvalid()
{
    setState( LOGFILE_STATE_INVALID, 1000 );
}


void Logfile::setMode( const ps_logfile_mode_kind mode,
                       const ps_rnr_session_id sessionId )
{
    checkDTC( psync_logfile_set_mode( _node, mode, sessionId ),
                "Failed to set mode." );
}


void Logfile::setFilePath( const std::string & filePath )
{
    if( filePath.empty() )
    {
        checkDTC( psync_logfile_set_file_path( _node, nullptr ),
                  "Failed to set filePath, because it was not specified." );
    }
    else
    {
        psync_logfile_set_file_path( _node, filePath.data() );

        _filePath = filePath;
    }
}


std::string Logfile::getFilePath() const
{
    return _filePath;
}


void Logfile::setState(
        const ps_logfile_state_kind state, const ps_timestamp startTimeOffSet)
{
    checkDTC( psync_logfile_set_state( _node, state, startTimeOffSet ),
                "Failed to set state." );
}


void Logfile::writeMessage( const Message & message )
{
    checkDTC( psync_logfile_write_message( _node, message.getMessageReference() ),
                "Failed to write message." );
}


void Logfile::enableOutputQueue( const uint enableReplayMessageQueue )
{
    checkDTC( psync_logfile_enable_output_queue
              ( _node, enableReplayMessageQueue ),
                "Failed to enable LogFile Replay Message Queue." );
}


void Logfile::getThreadStatus()
{
    checkDTC( psync_logfile_get_thread_status( _node ),
                "Failed to get/check LogFile thread status." );
}


void Logfile::setReplayClockScaler(const double scale)
{
    checkDTC( psync_logfile_set_replay_clock_scaler( _node, scale ),
                "Failed to set LogFile Replay Clock Scaler." );
}


double * const Logfile::getReplayClockScaler()
{
    double * scale = nullptr;

    checkDTC( psync_logfile_get_replay_clock_scaler( _node, scale ),
                "Failed to get LogFile Replay Clock Scaler." );

    return scale;
}


void Logfile::setMessageTypeFilters(
    ps_msg_type * const writerDisabledTypes,
    const unsigned long writerDisabledTypesLen,
    ps_msg_type * const readerDisabledTypes,
    const unsigned long readerDisabledTypesLen)
{
    checkDTC( psync_logfile_set_message_type_filters
              ( _node, writerDisabledTypes, writerDisabledTypesLen,
                readerDisabledTypes, readerDisabledTypesLen),
                "Failed to set LogFile Message Type Filters." );
}


GAsyncQueue * Logfile::getReplayMessageQueue()
{
    GAsyncQueue * queuePtr = nullptr;

    checkDTC( psync_logfile_get_replay_msg_queue( _node, &queuePtr ),
                "Failed to get LogFile Replay Message Queue." );

    return queuePtr;
}


ps_logfile_mode_kind Logfile::getMode()
{
    ps_logfile_mode_kind modeKind = LOGFILE_MODE_INVALID;

    checkDTC( psync_logfile_get_mode( _node, &modeKind ),
                "Failed to get LogFile mode." );

    return modeKind;
}


ps_logfile_state_kind Logfile::getState()
{
    ps_logfile_state_kind stateKind = LOGFILE_STATE_INVALID;

    checkDTC( psync_logfile_get_state( _node, &stateKind ),
                "Failed to get LogFile state." );

    return stateKind;
}


std::pair<ps_guid, ps_identifier> Logfile::getReplayClockOwner()
{
    std::pair<ps_guid, ps_identifier>getReplayClockOwnerPair;

    ps_guid ownerGUID;

    ps_identifier ownerPID;

        checkDTC( psync_logfile_get_replay_clock_owner
                  ( _node, &ownerGUID, &ownerPID ),
                    "Failed to get LogFile Replay Clock Owner." );

    return { ownerGUID, ownerPID };
}


bool Logfile::readerGetEofStatus()
{
    uint eofReached = 0;

    checkDTC( psync_logfile_reader_get_eof_status
              ( _node, &eofReached ),
                "Failed to get LogFile EOF status." );

    if ( eofReached )
    {
        return true;
    }
    return false;
}


void Logfile::readerResetToBeginning()
{
    checkDTC( psync_logfile_reader_reset_to_beginning( _node ),
                "Failed to reset LogFile Reader to beginning of file." );
}


void Logfile::readerSeekTo( const ps_timestamp timestamp )
{
    checkDTC( psync_logfile_reader_seek_to( _node, timestamp ),
                "Failed to seek LogFile Reader to timestamp  set in file." );
}


ps_logfile_attributes * Logfile::readerGetFileAttributes() const
{
    ps_logfile_attributes * attributes;

    checkDTC( psync_logfile_reader_get_file_attributes(
                 _node, attributes ),
              "Failed to get LogFile Reader file attributes." );

    return attributes;
}


void Logfile::fillRnRStatusMessage( ps_rnr_msg * const message )
{
    checkDTC( psync_logfile_fill_rnr_status_message(
                  _node, message),
        "Failed to fill Record and Replay message with current LogFile data." );
}


void Logfile::handleRnRCommand( const polysync::datamodel::RnrMessage & message )
{
    checkDTC( psync_logfile_handle_rnr_command(
                 _node, message.get_ps_rnr_msg() ),
              "Failed to handle LogFile Record and Replay message." );
}


void Logfile::forEachIterator(
    const std::string filePath,
    const psync_logfile_iterator_callback callback,
    void * const userData )
{
    checkDTC( psync_logfile_foreach_iterator(
                 _node, filePath.c_str(), callback, userData ),
              "Failed to iterate over data in a PolySync plog logfile." );
}


void Logfile::logfileIteratorCallback(
    const ps_logfile_attributes * const fileAttributes,
    const ps_msg_type msg_type,
    const ps_rnr_log_record * const logRecord,
    void * const userData)
{
    cout<<"\nLogfile iterator callback\n\n";

    if( logRecord )
    {
        cout <<"\nLog Record: Index: " << logRecord->index <<" - Size: "
             << logRecord->size <<" - Previous Size: "
             << logRecord->prev_size <<" - RnR Timestamp ( \nheader.timestamp):"
             << logRecord->timestamp;
    }

    cout << endl;
}

void Logfile::setSessionId( ps_timestamp currentTimeStamp )
{
    _sessionId = currentTimeStamp;
}


ps_timestamp Logfile::getSessionId() const
{
    return _sessionId;
}
