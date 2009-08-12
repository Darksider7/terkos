/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "VersionInfoManager.h"

const string VersionInfoManager::UNKNOWN_VALUE = "???";

const string VersionInfoManager::getHardwareVersion() const
   {
   return hardwareVersion;
   }

const string VersionInfoManager::getFirmwareVersion() const
   {
   return firmwareVersion;
   }

const string VersionInfoManager::getOperatingSystemName() const
   {
   return unameOperatingSystemName;
   }

const string VersionInfoManager::getOperatingSystemVersion() const
   {
   return unameOperatingSystemVersion;
   }

const string VersionInfoManager::getOperatingSystemRelease() const
   {
   return unameOperatingSystemRelease;
   }

const string VersionInfoManager::getProcessorType() const
   {
   return unameProcessorType;
   }

const string VersionInfoManager::getMachineType() const
   {
   return unameHardwareType;
   }

const string VersionInfoManager::getHostname() const
   {
   return unameHostname;
   }

Json::Value VersionInfoManager::getJSON()
   {
   Json::Value operatingSystemElement;
   operatingSystemElement["name"] = getOperatingSystemName();
   operatingSystemElement["release"] = getOperatingSystemRelease();
   operatingSystemElement["version"] = getOperatingSystemVersion();
   operatingSystemElement["machine-type"] = getMachineType();
   operatingSystemElement["processor-type"] = getProcessorType();
   operatingSystemElement["hostname"] = getHostname();

   Json::Value systemInfoElement;
   systemInfoElement["hardware"] = getHardwareVersion();
   systemInfoElement["firmware"] = getFirmwareVersion();
   systemInfoElement["operating-system"] = operatingSystemElement;

   Json::Value rootElement;
   rootElement["version-info"] = systemInfoElement;

   return rootElement;
   }