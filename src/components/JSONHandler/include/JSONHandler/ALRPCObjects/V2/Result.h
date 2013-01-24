#ifndef NSAPPLINKRPCV2_RESULT_INCLUDE
#define NSAPPLINKRPCV2_RESULT_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class Result
  {
  public:
    enum ResultInternal
    {
      INVALID_ENUM=-1,

///  The request succeeded
      SUCCESS=0,

/**
     The data sent is invalid. For example
     Invalid Json syntax
     Parameters out of bounds (number or enum range)
     Mandatory parameters not provided
     Parameter provided with wrong type
     Invalid characters
     Empty string
*/
      INVALID_DATA=1,

///  The request is not supported by Sync
      UNSUPPORTED_REQUEST=2,

///  The system could not process the request because the necessary memory couldn't be allocated
      OUT_OF_MEMORY=3,

///  There are too many requests pending (means, that the response has not been delivered, yet).
      TOO_MANY_PENDING_REQUESTS=4,

/**
     One of the provided IDs is not valid. For example
     This applies to CorrelationID, SubscriptionID [@TODO if SubscriptionID is used], CommandID, MenuID, [@TODO: missed one?]
*/
      INVALID_ID=5,

///  There was a conflict with an registered name (application or menu item) or vr command
      DUPLICATE_NAME=6,

///  There are already too many registered applications
      TOO_MANY_APPLICATIONS=7,

///  RegisterApplication has been called again, after a RegisterApplication was successful before.
      APPLICATION_REGISTERED_ALREADY=8,

///  Sync doesn't support the protocol that is requested by the mobile application
      UNSUPPORTED_VERSION=9,

/**
     The requested language is currently not supported.
     Might be because of a mismatch of the currently active language on Sync and the requested language
*/
      WRONG_LANGUAGE=10,

///  An command can not be executed because no application has been registered with RegisterApplication.
      APPLICATION_NOT_REGISTERED=11,

/**
     The data may not be changed, because it is currently in use.
     For example when trying to delete a command set that is currently involved in an interaction.
*/
      IN_USE=12,

///  The item to subscribe to is already subscribed to.
      SUBSCRIBED_ALREADY=13,

///  The user has turned off access to vehicle data, and it is globally unavailable to mobile applications.
      VEHICLE_DATA_NOT_ALLOWED=14,

///  The requested vehicle data is not available on this vehicle or is not published.
      VEHICLE_DATA_NOT_AVAILABLE=15,

/**
     The requested command was rejected, e.g. because mobile app is in background and cannot perform any HMI commands.
     Or an HMI command (e.g. Speak) is rejected because a higher priority HMI command (e.g. Alert) is playing.
*/
      REJECTED=16,

/**
     A command was aborted, for example due to user interaction (e.g. user pressed button).
     Or an HMI command (e.g. Speak) is aborted because a higher priority HMI command (e.g. Alert) was requested.
*/
      ABORTED=17,

/**
     A command was ignored, because the intended result is already in effect.
     For example, SetMediaClockTimer was used to pause the media clock although the clock is paused already.
     NOTE: potentially replaces SUBSCRIBED_ALREADY
*/
      IGNORED=18,

/**
     A button that was requested for subscription is not supported under the current system.
     NOTE: could become a more generic UNSUPPORTED_RESOURCE by merging with VEHIVLE_DATA_NOT_AVAILABLE.
*/
      UNSUPPORTED_BUTTON=19,

///  A specified file could not be found on Sync.
      FILE_NOT_FOUND=20,

///  Provided data is valid but something went wrong in the lower layers.
      GENERIC_ERROR=21,

///  RPC is not authorized in local policy table.
      DISALLOWED=22,

///  RPC is included in a functional group explicitly blocked by the user.
      USER_DISALLOWED=23,

///  Overlay reached the maximum timeout and closed.
      TIMED_OUT=24,

///  User selected to Cancel Route.
      CANCEL_ROUTE=25,

///  User chose a Close command for the overlay.
      CLOSED=26,

/**
     The data is being returned through an OnEncodedSYNCPData notification.
     The data is intended to be passed by the proxy to the designated server URL.
*/
      ENCRYPTED=27,

///  The RPC (e.g. SubscribeVehicleData) executed successfully but one or more items have a warning or failure.
      WARNINGS=28
    };
  
    Result() : mInternal(INVALID_ENUM)				{}
    Result(ResultInternal e) : mInternal(e)		{}
  
    ResultInternal get(void) const	{ return mInternal; }
    void set(ResultInternal e)		{ mInternal=e; }
  
  private:
    ResultInternal mInternal;
    friend class ResultMarshaller;
  };
  
}

#endif
