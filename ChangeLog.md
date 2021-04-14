# hinder Change Log

## 2021-04-10 unstable branch

* **hinder::assert**
    * New module.


* **hinder::core**
    * New module.
    * Moved compiler.h and platform.h to hinder::core from hinder::misc.
    * Move timestamp code to hinder::core from hinder::exception. Made code more broadly applicable.
    * Add a safe_format function that helps with the corner case mentioned below.

* **hinder::exception**
    * Moved timestamp code to hinder::core from hinder::exception.
    * Finally wrote proper tests. Yes, I am a bad, bad person for not writing the tests in parallel
      with the code changes!

* **hinder::misc**
    * Renamed portability.h to compiler.h and created platform.h to separate compiler and platform
      portability. Sorry for any breakage.
    * Moved compiler.h and platform.h to hinder.core. Sorry for any breakage.

