# Program Loader for cRTOS

## Requirements

### Toolchains

 * gcc (tested with version 9)

### Supporting Drivers

You should have also received a package of Linux drivers together with cRTOS.

For this Program Loader to work, those drivers are required.

Please reference the driver's README.md for more informations.

## To Build

Execute the following commands in the root directory.

```sh
make
```

## To Use

This loader expected to be run with either SCHED_FIFO or SCHED_RR.
Any other scheduling policy will cause it to fail to start.

The scheduling policy and priority are passed onto RTOS side for creating the process.

The most easy way to get a SCHED_FIFO or SCHED_RR policy is via chrt.
```
# SCHED_FIFO
chrt -f ./loader <executable path> <arguments for executable>
```

## LICENSE

This program is licensed under version 2 of Apache license.

You can find a copy of the license in the `LICENSE` file.

## Authors

This is an creation of ChungFan Yang <chungfan.yang@fixstars.com> 
from Fixstars corporation.
