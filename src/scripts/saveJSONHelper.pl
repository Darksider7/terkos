#!/usr/bin/perl

require "/opt/scripts/fileUtils.pl";
require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/jsonUtils.pl";
require "/opt/scripts/pathUtils.pl";

#===================================================================================================
sub saveJSON()
   {
   my ($pathToJsonConfigFile, $pathToProgram) = @_;

   my $jsonInput;

   if (&isHttpRequest())
      {
      # parse HTTP request params
      my %arguments = &parseHttpRequestParameters();

      # get the JSON input
      $jsonInput = $arguments{'json'};
      }
   else
      {
      # get JSON input from STDIN
      $jsonInput = $_=join "",<>;
      }

   # create the JSON output
   my %response = ();
   if ($jsonInput)
      {
      my $jsonBeforeUpdate = &readFileIntoString($pathToJsonConfigFile);

      # call the program and pipe the JSON input to it via its STDIN
      open(PROGRAM, "| $pathToProgram") or die "cannot open $pathToProgram: $!";
      print PROGRAM "$jsonInput\n";
      close(PROGRAM);

      my $jsonAfterUpdate = &readFileIntoString($pathToJsonConfigFile);

      # Compare the before and after JSON and prepare the response accordingly.  TODO:  It would be
      # nice to get a better status from the program rather than merely comparing the JSON output
      # (since the JSON might be invalid or maybe the tool just had a problem saving for some
      # reason), but unfortunately, there's no easy way to do bidirectional communication with
      # another process, so for now we'll just pretend this isn't a problem.  Possible fixes are:
      # 1) have the web site just call the program and
      #    rig it to handle HTTP requests, perhaps with cgicc (http://www.gnu.org/software/cgicc/).
      #    This will probably be impossible, though, unless there's some way to
      #    configure busybox httpd so that the LD_LIBRARY_PATH environment variable
      #    is set (the limited environment which CGIs get doesn't have it)
      # 2) Use Comm.pl to do bidirectional communication (see example at
      #    http://perldoc.perl.org/perlipc.html#Bidirectional-Communication-with-Another-Process)
      # 3) Use some bidirectional magic as shown in example 16-8 of the Perl Cookbook
      $response{'statusCode'} = '200';
      $response{'statusName'} = 'OK';
      if ($jsonBeforeUpdate eq $jsonAfterUpdate)
         {
         $response{'ok'} = 'false';
         $response{'message'} = 'configuration unchanged';
         }
      else
         {
         $response{'ok'} = 'true';
         $response{'message'} = 'configuration updated';
         }
      }
   else
      {
      $response{'statusCode'} = '400';
      $response{'statusName'} = 'Bad Request';
      $response{'ok'} = 'false';
      $response{'message'} = 'missing parameter: json';
      }

   my $jsonOutput = '';
   $jsonOutput .= "{\n";
   $jsonOutput .= '"ok" : '.$response{'ok'}.',' . "\n";
   $jsonOutput .= '"status_code" : '.$response{'statusCode'}.',' . "\n";
   $jsonOutput .= '"message" : "'.$response{'message'}.'"' . "\n";
   $jsonOutput .= "}\n";

   # write out the JSON
   if (&isHttpRequest())
      {
      print "HTTP/1.0 $response{'statusCode'} $response{'statusName'}\n";
      }
   &outputJson($jsonOutput);
   }
#===================================================================================================
return 1;