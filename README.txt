SUMMARY

I created these 3 Qt projects to quickly prototype/test Qt's networking facilities for an actual project at work which I was designing and developing.

The client GUI is used to initiate a TCP connection to the link to the server which serves as the communication link to the 'embedded' system. The server listens on the default port 7755 for client console connnections. It also listens a connection from the 'embedded' system on port 7756.

The port can be set on both the client and server GUI's. The server can handle multiple client console connections concurrently and keeps track of them. The actual project which this modelled had a network manager object which took on this task as well displaying the details off each connection. Also missing is securing the connection with SSL.

RUNNING

The bin folder contains the 3 pre-built executables, client-console.exe, embeddedsys-link.exe and embeddedsys-emulator.exe, as well as the runtime environment necessary for running all. To run them, download the whole bin folder. Navigate to it through windows explorer or command prompt and launch both.

BUILDING

The executables can also be build from the Qt source code contained in the src folder. I tested this
with Qt 4.8 and 5.4 on Windows and Linux. I lost the Linux version. I will restore that source and uploaded it
when I have it.

PLATFORM

Any Windows OS >= Windows XP. I'm working on getting this to run on Linux.
