# QT GPS TO SYSTEM TIME

This application (should compile everywhere but has time changing code only for windows)
allows you to set system time using a current time source connected via a serial port.

For example you can connect a GPS receiver that outputs NMEA sentences to it's port, or you can add the time output
capability to any of your devices in the following format:

`
===RRR DATEPACK: D-M-YYYY H_M_S RRR=== \r\n
`

Since this app was written for personal use it has no port options select controls. So, it expects the 8-N-1
configuration.

Release available in two flavors:

- MSVC 2019 Static (about 11M)
- MSVC 2019 Static with UPX (about 4M, smaller exe eats more RAM)

<a href="https://www.flaticon.com/free-icons/clock" title="clock icons">Clock icons created by Freepik - Flaticon</a>