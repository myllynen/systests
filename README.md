# Trivial system test utilities

[![License: GPLv3](https://img.shields.io/badge/license-GPLv3-brightgreen.svg)](https://www.gnu.org/licenses/gpl-3.0)

Trivial, single-purpose utilities for simple system tests.

For more serious testing, please refer to the usual, well-known tools.

## Contents

* [cpu-busy.c](cpu-busy.c)
  * A program that keeps one CPU busy
* [cpu-idle.c](cpu-idle.c)
  * A program that does nothing (idles)
* [disk-read.c](disk-read.c)
  * A program that reads in a file
* [disk-write.c](disk-write.c)
  * A program that writes to a file
* [mem-res-rss.c](mem-res-rss.c)
  * A program that reserves memory (RSS)
* [mem-res-virt.c](mem-res-virt.c)
  * A program that reserver memory (VIRT)
* [net-read.c](net-read.c)
  * A program that reads from UDP socket
* [net-write.c](net-write.c)
  * A program that write to UDP socket
* [proc-abort.c](proc-abort.c)
  * A program that aborts
* [proc-segfault.c](proc-segfault.c)
  * A program that segfaults

## License

GPLv3
