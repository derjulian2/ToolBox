various experiments around networking and socket-programming in C++

asyncIO: simple TCP server/client to transfer files from A to B
-> tried to use async sockets, but kind of reintroduced blocking behaviour because calling io_context.run() each operation
-> io_context needs to run in a seperate thread to fully utilize async sockets
