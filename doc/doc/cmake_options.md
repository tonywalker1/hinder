# CMake Options

Two goals for the hinder library are: 1) flexibility and 2) to do as much as possible at compile
time. As a result, hinder has several compile-time options. The following options control how the
hinder library is built. The default setting is in **bold**.

To use these options, do the following from your build directory:

```shell
cmake -D<OPTION_1> -D<OPTION_2> -D<OPTION_N> <path_to_source>
```

For example:

```shell
cd build
cmake -DHINDER_WITH_TESTS=OFF -DHINDER_EXCEPTION_MESSAGE_FORMAT=Structured ..
```

### General Options

* **HINDER_WITH_TESTS:** Build tests [**ON** | OFF].

### Exception Options

* **HINDER_WITH_EXCEPTION_SOURCE:** Add ____FILE____ and ____LINE____ to exception messages.
  [**ON** | OFF]

* **HINDER_EXCEPTION_MESSAGE_FORMAT:** Select the format of exception messages.
  [**Default** | User | Structured]

See the [exception module documentation](./doc/exception.md) for more details.
