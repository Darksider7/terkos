//======================================================================================================================
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the GNU General Public License v2
// (http://www.gnu.org/licenses/gpl-2.0.html). Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at telepresencerobotkit@cs.cmu.edu. Such licensing
// terms are available for all portions of the Terk and TerkOS codebase presented here.
//======================================================================================================================
// Class for reading the memory usage info.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
//
// Author: Chris Bartley (bartley@cmu.edu)
//======================================================================================================================

//======================================================================================================================
// VERIFY NAMESPACE
//======================================================================================================================
// Create the global symbol "edu" if it doesn't exist.  Throw an error if it does exist but is not an object.
var edu;
if (!edu)
   {
   edu = {};
   }
else if (typeof edu != "object")
   {
   var eduExistsMessage = "Error: failed to create edu namespace: edu already exists and is not an object";
   alert(eduExistsMessage);
   throw new Error(eduExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu)
   {
   edu.cmu = {};
   }
else if (typeof edu.cmu != "object")
   {
   var eduCmuExistsMessage = "Error: failed to create edu.cmu namespace: edu.cmu already exists and is not an object";
   alert(eduCmuExistsMessage);
   throw new Error(eduCmuExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri)
   {
   edu.cmu.ri = {};
   }
else if (typeof edu.cmu.ri != "object")
   {
   var eduCmuRiExistsMessage = "Error: failed to create edu.cmu.ri namespace: edu.cmu.ri already exists and is not an object";
   alert(eduCmuRiExistsMessage);
   throw new Error(eduCmuRiExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri.terk)
   {
   edu.cmu.ri.terk = {};
   }
else if (typeof edu.cmu.ri.terk != "object")
   {
   var eduCmuRiTerkExistsMessage = "Error: failed to create edu.cmu.ri.terk namespace: edu.cmu.ri.terk already exists and is not an object";
   alert(eduCmuRiTerkExistsMessage);
   throw new Error(eduCmuRiTerkExistsMessage);
   }
//======================================================================================================================

//======================================================================================================================
// DEPENDECIES
//======================================================================================================================
if (!window['$'])
   {
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.MemoryUsageManager.js";
   alert(noJQuery);
   throw new Error(noJQuery);
   }
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================
(function()
   {
   // ==================================================================================================================
   var jQuery = window['$'];
   jQuery.ajaxSetup({
      type: 'GET',
      dataType: 'jsonp',
      timeout: 10000,
      cache: false,
      global: false
   });

   edu.cmu.ri.terk.MemoryUsageManager = function(host)
      {
      var eventListeners = new Array();
      var json = null;

      var notifyEventListeners = function(eventName)
         {
         jQuery.each(eventListeners, function(i, listener)
            {
            if (listener && listener[eventName])
               {
               listener[eventName]();
               }
            });
         };

      this.getMemoryUsage = function()
         {
         notifyEventListeners('onBeforeLoad');

         // load the config
         jQuery.ajax(
         {
            url: host + '/cgi-bin/getMemoryUsageAsJSON.pl',
            success: function(jsonResponse)
               {
               json = jsonResponse;

               if (json)
                  {
                  notifyEventListeners('onLoadSuccess');
                  }
               else
                  {
                  notifyEventListeners('onLoadFailure');
                  }
               },
            error: function()
               {
               notifyEventListeners('onLoadFailure');
               json = null;
               }
         });
         };

      /**
       * Registers the given event listener.  The event listener should implement some or all of the following methods:
       *
       *    onBeforeLoad()
       *    onLoadSuccess()
       *    onLoadFailure()
       */
      this.addEventListener = function(listener)
         {
         if (listener)
            {
            eventListeners[eventListeners.length] = listener;
            }
         };

      var getJSONProperty = function(jsonRoot, propertyPath)
         {
         if (jsonRoot)
            {
            if (propertyPath)
               {
               var propertyNames = propertyPath.split('.');
               for (var i = 0; i < propertyNames.length; i++)
                  {
                  var propertyName = propertyNames[i];
                  if (typeof jsonRoot[propertyName] == 'undefined')
                     {
                     return null;
                     }
                  else
                     {
                     jsonRoot = jsonRoot[propertyName];
                     }
                  }
               return jsonRoot;
               }

            return jsonRoot;
            }

         return null;
         };

      this.getMainMemoryUsage = function()
         {
         return createCopyOfUsageJSON("memory-usage.memory");
         };

      this.getSwapMemoryUsage = function()
         {
         return createCopyOfUsageJSON("memory-usage.swap");
         };

      this.getTotalMemoryUsage = function()
         {
         return createCopyOfUsageJSON("memory-usage.total");
         };

      var createCopyOfUsageJSON = function(propertyPath)
         {
         var usage = getJSONProperty(json, propertyPath);
         var usageJSON = {
            "total" : usage['total'],
            "used" : usage['used'],
            "free" : usage['free']
         };

         return usageJSON;
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();
