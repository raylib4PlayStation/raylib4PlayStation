# Install for Orbis
- Install last orbisdev from https://github.com/orbisdev/orbisdev
- Install orbislink in your Playstation 4 from https://github.com/orbisdev/orbisdev-orbislink
- clone this repository and execute:
```
./orbisdevbuild.sh
```

# Samples
- in each sample for example for models_mesh_generation can be loaded from nfs with orbislink
```
% cd samples
% ls
orbis  vita
% cd orbis
% ls
models	shapes
% cd models
% ls
models_mesh_generation
% cd models_mesh_generation
% ls
Makefile  source
% make
% make oelf
% make eboot
% make install
```
- After orbislink is installed in your console and you configured a nfs server, open orbislink and it will try to load homebrew.self from your configured nfs export
- For logs check scripts described in https://github.com/orbisdev/orbisdev-orbislink
