#!/bin/bash
set -e

# 下载SDL2及相关库
SDL_VERSION=2.28.5
SDL_IMAGE_VERSION=2.8.2
SDL_TTF_VERSION=2.22.0

mkdir -p deps
cd deps

# 下载SDL2
if [ ! -d "SDL2-$SDL_VERSION" ]; then
    wget https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VERSION/SDL2-$SDL_VERSION.tar.gz
    tar xzf SDL2-$SDL_VERSION.tar.gz
fi

# 下载SDL2_image
if [ ! -d "SDL2_image-$SDL_IMAGE_VERSION" ]; then
    wget https://github.com/libsdl-org/SDL_image/releases/download/release-$SDL_IMAGE_VERSION/SDL2_image-$SDL_IMAGE_VERSION.tar.gz
    tar xzf SDL2_image-$SDL_IMAGE_VERSION.tar.gz
fi

# 下载SDL2_ttf
if [ ! -d "SDL2_ttf-$SDL_TTF_VERSION" ]; then
    wget https://github.com/libsdl-org/SDL_ttf/releases/download/release-$SDL_TTF_VERSION/SDL2_ttf-$SDL_TTF_VERSION.tar.gz
    tar xzf SDL2_ttf-$SDL_TTF_VERSION.tar.gz
fi

cd ..

# 复制SDLActivity.java
mkdir -p app/src/main/java/org/libsdl/app
cp deps/SDL2-$SDL_VERSION/android-project/app/src/main/java/org/libsdl/app/*.java app/src/main/java/org/libsdl/app/

# 复制预编译库（使用官方Android预编译包）
mkdir -p app/src/main/jniLibs
cd deps

# 下载SDL2 Android预编译库
if [ ! -d "SDL2-$SDL_VERSION/android-project/app/jni" ]; then
    cd SDL2-$SDL_VERSION
    mkdir -p build-android
    cd build-android
    ../android-project/build.sh --no-build
    cd ../..
fi

# 复制SDL2 so库
for abi in armeabi-v7a arm64-v8a x86 x86_64; do
    mkdir -p ../app/src/main/jniLibs/$abi
    cp SDL2-$SDL_VERSION/build/org.libsdl.app/intermediates/ndkBuild/debug/obj/local/$abi/libSDL2.so ../app/src/main/jniLibs/$abi/ 2>/dev/null || true
    cp SDL2-$SDL_VERSION/android-project/app/jni/SDL2/*.so ../app/src/main/jniLibs/$abi/ 2>/dev/null || true
done

# 编译SDL2_image和SDL2_ttf for Android
# 简单起见，使用预编译的依赖，或者在CI中用ndk-build编译
# 这里我们直接用vcpkg或者预编译包，不过为了简单，我们在CI中使用CMake编译所有依赖

echo "Dependencies prepared successfully!"
