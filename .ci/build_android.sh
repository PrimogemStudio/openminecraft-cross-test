#!/bin/bash
set -e
export LIBFFI_VERSION=3.4.2
export ANDROID=1 LWJGL_BUILD_OFFLINE=1
#export LWJGL_BUILD_ARCH=arm64

# Setup env
if   [ "$LWJGL_BUILD_ARCH" == "arm64" ]; then
  export NDK_ABI=arm64-v8a NDK_TARGET=aarch64
elif [ "$LWJGL_BUILD_ARCH" == "arm32" ]; then
  export NDK_ABI=armeabi-v7a NDK_TARGET=armv7a NDK_SUFFIX=eabi
elif [ "$LWJGL_BUILD_ARCH" == "x86" ]; then
  export NDK_ABI=x86 NDK_TARGET=i686
  # Workaround: LWJGL 3 lacks of x86 Linux libraries
  mkdir -p bin/libs/native/linux/x86/org/lwjgl/glfw
  touch bin/libs/native/linux/x86/org/lwjgl/glfw/libglfw.so
elif [ "$LWJGL_BUILD_ARCH" == "x64" ]; then
  export NDK_ABI=x86_64 NDK_TARGET=x86_64
fi

export TARGET=$NDK_TARGET-linux-android$NDK_SUFFIX
export PATH=$PATH:$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin

LWJGL_NATIVE=bin/libs/native/linux/$LWJGL_BUILD_ARCH/org/lwjgl
mkdir -p $LWJGL_NATIVE

if [ "$SKIP_LIBFFI" != "1" ]; then
  # Get libffi
  if [ ! -d libffi ]; then
    wget https://github.com/libffi/libffi/releases/download/v$LIBFFI_VERSION/libffi-$LIBFFI_VERSION.tar.gz
    tar xvf libffi-$LIBFFI_VERSION.tar.gz
    mv libffi-$LIBFFI_VERSION libffi
  fi
  cd libffi

  # Build libffi
  bash configure --host=$TARGET --prefix=$PWD/$NDK_TARGET-unknown-linux-android$NDK_SUFFIX CC=${TARGET}21-clang CXX=${TARGET}21-clang++
  make -j4
  cd ..

  # Copy libffi
  cp libffi/$NDK_TARGET-linux-android$NDK_SUFFIX/.libs/libffi.a $LWJGL_NATIVE/
fi

# Download libraries
POJAV_NATIVES="https://github.com/PojavLauncherTeam/PojavLauncher/raw/v3_openjdk/app_pojavlauncher/src/main/jniLibs/$NDK_ABI"
wget -nc $POJAV_NATIVES/libopenal.so -P $LWJGL_NATIVE/openal
wget -nc "https://nightly.link/PojavLauncherTeam/shaderc/workflows/android/main/libshaderc-$NDK_ABI.zip"
unzip -o libshaderc-$NDK_ABI.zip -d $LWJGL_NATIVE/shaderc

# HACK: Skip compiling and running the generator to save time and keep LWJGLX functions
mkdir -p bin/classes/{generator,templates/META-INF}
touch bin/classes/{generator,templates}/touch.txt bin/classes/generator/generated-touch.txt

# Build LWJGL 3
ant -version
yes | ant -Dplatform.linux=true \
  -Djavadoc.skip=true \
  -Dnashorn.args="--no-deprecation-warning" \
  compile compile-native release

# Copy native libraries
rm -rf bin/out; mkdir bin/out
find $LWJGL_NATIVE -name 'liblwjgl*.so' -exec cp {} bin/out/ \;
cp $LWJGL_NATIVE/shaderc/libshaderc.so bin/out/

# Cleanup unused output jar files
find bin/RELEASE \( -name '*-natives-*' -o -name '*-sources.jar' \) -delete