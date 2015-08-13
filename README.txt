SUMMARY

I created these 2 Qt projects to quickly prototype/test Qt's networking facilities for an actual
project at work which I designed and developed.

The client GUI is used to initiate a TCP connection to the server which is expected to be listening
on the default port 7755. The port can be set on both the client and server GUI's. The tcp server 
accepts multiple persistent connections concurrently and keeps track of them. The actual project
which this modelled had a network manager object which took on this task as well displaying the details
off each connection. Also missing is securing the connection with SSL.

RUNNING

The bin folder contains the 2 pre-built executables, test-tcpclient.exe and test-tcpsrv.exe, as well
as the runtime environment necessary for running both. To run them, download the whole bin folder. 
Navigate to it through windows explorer or command prompt and launch both.

BUILDING

The executables can also be build from the Qt source code contained in the src folder. I tested this
with Qt 4.8 and 5.4 on Windows and Linux. I lost the Linux version.

PLATFORM

Any Windows OS >= Windows XP. I'm working on getting this to run on Linux.
