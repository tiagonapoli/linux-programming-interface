# Chapter 10: Time

## Question 10-1
------------

**Question:**

Assume a system where the value returned by the call sysconf(_SC_CLK_TCK) is 100. Assuming that the clock_t value returned by times() is an unsigned 32-bit integer, how long will it take before this value cycles so that it restarts at 0? Perform the same calculation for the CLOCKS_PER_SEC value returned by clock().

**Answer:**

`_SC_CLK_TCK`: It will take `(2^32-1)/100` seconds to cycle (approximately 497 days).
`CLOCKS_PER_SEC`: It will take `(2^32-1)/CLOCKS_PER_SEC`, since `CLOCKS_PER_SEC` is fixed at `1e6` by POSIX then the time to cycle will be `(2^32-1)/1e6 = 4294.967295` seconds, which is 71 Minutes and 35 Seconds.