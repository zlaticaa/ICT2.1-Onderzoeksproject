to repeat what we did you must follow these steps
1) open a hotspot on the laptop with the server
2) connect to the hotspot
3) change the ip-address define the client to the hotspot ip adress with 255 as the last byte (eg. 198.168.8.255)
4) use netsh to change the mtu of the hotspot on both sides ('wifi' for client and 'local area connection' for the server)
5) go to your time and language settings and then into date and time to sync your laptops internal clocks
6) change the define of the MTU on the servers side to whatever mtu used
7) remove any data from the data.csv (anything that isn't the column names)
8) open the server and AFTER it started up open the client on the client pc
9) repeat as often as wanted