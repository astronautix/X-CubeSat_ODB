# Manuel utilisater

Ce document devrait donner les information pour compiler, tester, comprendre le soft.

# Compilation

La compilation peut être faite sur n'importe quel ordi, puis ensuite flashé sur la carte. 
C'est ce qu'on appelle la **Cross-compilation**.

## Dépendances

Il faut la commande `arm-none-eabi-as` pour pouvoir cross-compiler du ARM.
Pour Debian:
```
sudo apt install gcc-arm-none-eabi
```
Pour Fedora:
```
sudo dnf install arm-none-eabi-binutils-cs arm-none-eabi-gcc-cs arm-none-eabi-newlib arm-none-eabi-gcc-c++
```

## Lancement de la compilation
```
make DEBUG=1
```

## Flash sur la carte ADCS
```
st-flash write ./sat/ADCS/ADCS.bin 0x08000000
```


# Documentation

The projet uses `Doxygen` to generate the documentation by parsing the comments of the code.
The configuration file is `sat/rules/Doxygen`.
