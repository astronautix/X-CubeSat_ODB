# Manuel utilisater

Ce document devrait donner les information pour compiler, tester, comprendre le soft.

# Compilation

La compilation peut être faite sur n'importe quel ordi, puis ensuite flashé sur la carte. 
C'est ce qu'on appelle la **Cross-compilation**.

Il faut la commande `arm-none-eabi-as` pour pouvoir cross-compiler du ARM.
Pour Debian:
```
sudo apt install gcc-arm-none-eabi
```
Pour Fedora:
```
sudo dnf install arm-none-eabi-binutils-cs arm-none-eabi-gcc-cs arm-none-eabi-newlib arm-none-eabi-gcc-c++
```


# Documentation

The projet uses `Doxygen` to generate the documentation by parsing the comments of the code.
The configuration file is `sat/rules/Doxygen`.
