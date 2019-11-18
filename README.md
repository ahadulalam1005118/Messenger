

**client folder contains client without gui using qt
**project2_client folder contains client with gui
To build the server:
cd server
cmake .
make
run the server using following command
./server user_credentials configurations
open configuration to change the port number
**don't change the ip:123 or something
**add any by default user or friendlist in user_credentials files as per as template.

To build the client:
cd client
cmake .
make
open configurations file to change the ip and port number
run the client using following command
./client configurations

Some specification:
When register and login from a machine. do this at a time. otherwise sometimes same ip address assigned from different machine.
The i and ia command specification:
i:friend_name:message
ia:friend_name:message

The m command specification:
first type m
then type friend name
then type the message

logout command specification:
type logout: 

In server side, the file are only saved if INT signal is typed
**If you want to see the saved file at any state:
type save: from any client

Restriction:
Sometimes even though i send message it's not recieved by some clients. may be it's for port conflict.

I also added a gui version in my submission whose gui is developed by using qt.
But only the message command are not handled in that version.

**NO validation is handled
say, user1 is not friend of user2 but sends message to user1
user1 invites himself
