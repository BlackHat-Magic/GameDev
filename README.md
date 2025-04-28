# GameDev

I wanted to make games in C with SDL as a learning exercise. Ideally, I'd have a usable "Core Engine" that is reusable across projects. One giant mono repository seemed like the most straightforward way to do that.

I'm sure that there are some unholy bugs and issues present here. Reuse any code at your own risk; here be dragons.

## Testbed

Mostly a note to self

```bash
mkdir build
cd build

# basic configuration
cmake ..

# configuration specifying debug build type
# cmake .. -DCMAKE_BUILD_TYPE=Debug

# config for release build
# cmake .. -DCMAKE_BUILD_TYPE=Release

cmake --build .
```

