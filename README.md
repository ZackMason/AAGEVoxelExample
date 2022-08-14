# AAGEVoxelExample
This template was created using `cpp_init.py` https://github.com/ZackMason/cpp_init

![image](https://user-images.githubusercontent.com/3623261/184466155-ece421bd-5dd7-47e4-8409-f406e148a74e.png)

# How To Build 

```
mkdir build
cd build
conan install .. -s build_type=Release
cmake ..
cmake --build . --config Release
```

follow conan instructions if this is your first time building AAGE, you probably need to add --build=missing
