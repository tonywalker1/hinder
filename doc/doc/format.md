# hinder::core::format

Define the message formats that can be used by various modules (e.g., exceptions or asserts).

# Usage

The file defines a scoped enum that allows you to specify default, user, or structured messages 
for various components that produce messages (e.g., assert). The specific format of the message 
depends on the module. For example, the default exception message format is
```text
exception_type: message @file:line
```
while the default assert message is
```text
assertion 'condition' failed: message @file:line
```
See each applicable module for details.
