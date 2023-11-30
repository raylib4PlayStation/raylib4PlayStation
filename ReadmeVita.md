# Install for Vita
- Install last vitasdk from https://vitasdk.org/
- it is using pib library so check first that you have all needed to pib to run
- clone this repository and execute:
```
vita-makepkg
```
- install the library and includes generated in your vitasdk

# Samples
- change ip to receive udp logs in debugNetInit call in each sample for vita
- in each sample for example for models_mesh_generation 
```
% cd samples
% ls
orbis  vita
% cd vita
% ls
models	shapes
% cd models
% ls
models_mesh_generation
% cd models_mesh_generation
% ls
CMakeLists.txt	main.c
% mkdir build
% cd build
% cmake ..
% make
```
- install vpk generated in your Vita
- for udp logs you can use debug.sh script
```
echo [HOST] debugnet listener up
echo [HOST] ready to have a lot of fun!!!
socat udp-recv:18194 stdout
```







