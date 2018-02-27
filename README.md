# MySQL performance tests

This repository was created to easily test the performance issue of `mysql_connect` appeared between libmysqlclient 18 and 20.

It compiles the latest versions MySQL 5.6 and 5.7 without the servers and during runtime, it installs them separately and recompiles the test. Execution times will be printed by the application itself.

The test executes 4 simple connection tests: multiple connections (measures open only) at the same time with a 100 and 1000, and individual connections (measures open/close) 100 and 1000 times respectively. Depending on the hardware, a huge difference in execution time can be observed.
