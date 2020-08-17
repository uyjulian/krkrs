cmake_minimum_required(VERSION 3.0.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{DEVKITPRO})
        set(DEVKITPRO $ENV{DEVKITPRO})
        set(CMAKE_TOOLCHAIN_FILE "./cmake/switch.cmake" CACHE PATH "toolchain file")
    else()
        message(FATAL_ERROR "Please define DEVKITPRO to point to your SDK path!")
    endif()
endif()

project(krkrs)

set(SWITCH_APP_NAME "Kirikiri for Nintendo Switch")
set(SWITCH_AUTHOR   "W.Dee")
set(SWITCH_VERSION  "v1.1.0")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set(C_FLAGS
    -DTVP_ARCHIVE_ENABLE_XP3
    -DTVP_AUDIO_ENABLE_FFMPEG
    -DTVP_AUDIO_ENABLE_OPUS
    -DTVP_AUDIO_ENABLE_VORBIS
    -DTVP_AUDIO_ENABLE_WAVE
    -DTVP_COMPRESSION_ENABLE_ZLIB
    -DTVP_IMAGE_ENABLE_BMP
    -DTVP_IMAGE_ENABLE_JPEG
    -DTVP_IMAGE_ENABLE_PNG
    -DTVP_IMAGE_ENABLE_TLG
    -DTVP_IMAGE_ENABLE_WEBP
    -DTVP_RENDERER_ENABLE_FFMPEG
    -DTVP_TEXT_READ_ANSI_MBCS
)

set(CXX_FLAGS ${C_FLAGS}
    -fno-delete-null-pointer-checks
    -fpermissive
)

set(OPTIMIZATION_FLAGS
    -Ofast
)

set(INCLUDES
    source
    source/core
    source/core/base
    source/core/base/android
    source/core/base/win32
    source/core/environ
    source/core/environ/android
    source/core/environ/win32
    source/core/extension
    source/core/movie
    source/core/movie/ffmpeg
    source/core/msg
    source/core/msg/win32
    source/core/sdl2
    source/core/sound
    source/core/sound/win32
    source/core/tjs2
    source/core/utils
    source/core/utils/encoding
    source/core/utils/win32
    source/core/visual
    source/core/visual/gl
    source/core/visual/win32
    source/plugins
    source/plugins/ncbind
)

set(SOURCES
    source/core/base/BinaryStream.cpp
    source/core/base/CharacterSet.cpp
    source/core/base/EventIntf.cpp
    source/core/base/PluginIntf.cpp
    source/core/base/ScriptMgnIntf.cpp
    source/core/base/StorageIntf.cpp
    source/core/base/SysInitIntf.cpp
    source/core/base/SystemIntf.cpp
    source/core/base/TextStream.cpp
    source/core/base/UtilStreams.cpp
    source/core/base/XP3Archive.cpp
    source/core/base/android/GetLocalFileListAt.cpp
    source/core/base/android/StorageImpl.cpp
    source/core/base/win32/EventImpl.cpp
    source/core/base/win32/FileSelector.cpp
    source/core/base/win32/NativeEventQueue.cpp
    source/core/base/win32/PluginImpl.cpp
    source/core/base/win32/ScriptMgnImpl.cpp
    source/core/base/win32/SysInitImpl.cpp
    source/core/base/win32/SystemImpl.cpp
    source/core/environ/Application.cpp
    source/core/environ/android/SystemControl.cpp
    source/core/extension/Extension.cpp
    source/core/movie/ffmpeg/krffmpeg.cpp
    source/core/movie/krmovie.cpp
    source/core/msg/MsgIntf.cpp
    source/core/msg/win32/MsgImpl.cpp
    source/core/sdl2/stubs.cpp
    source/core/sound/CDDAIntf.cpp
    source/core/sound/FFWaveDecoder.cpp
    source/core/sound/MIDIIntf.cpp
    source/core/sound/MathAlgorithms.cpp
    source/core/sound/PhaseVocoderDSP.cpp
    source/core/sound/PhaseVocoderFilter.cpp
    source/core/sound/RealFFT.cpp
    source/core/sound/SoundBufferBaseImpl.cpp
    source/core/sound/SoundBufferBaseIntf.cpp
    source/core/sound/VorbisWaveDecoder.cpp
    source/core/sound/WaveFormatConverter.cpp
    source/core/sound/WaveIntf.cpp
    source/core/sound/WaveLoopManager.cpp
    source/core/sound/WaveSegmentQueue.cpp
    source/core/sound/win32/CDDAImpl.cpp
    source/core/sound/win32/MIDIImpl.cpp
    source/core/sound/win32/WaveImpl.cpp
    source/core/sound/win32/WaveMixer.cpp
    source/core/tjs2/tjs.cpp
    source/core/tjs2/tjs.tab.cpp
    source/core/tjs2/tjsArray.cpp
    source/core/tjs2/tjsBinarySerializer.cpp
    source/core/tjs2/tjsByteCodeLoader.cpp
    source/core/tjs2/tjsCompileControl.cpp
    source/core/tjs2/tjsConfig.cpp
    source/core/tjs2/tjsConstArrayData.cpp
    source/core/tjs2/tjsDate.cpp
    source/core/tjs2/tjsDateParser.cpp
    source/core/tjs2/tjsDebug.cpp
    source/core/tjs2/tjsDictionary.cpp
    source/core/tjs2/tjsDisassemble.cpp
    source/core/tjs2/tjsError.cpp
    source/core/tjs2/tjsException.cpp
    source/core/tjs2/tjsGlobalStringMap.cpp
    source/core/tjs2/tjsInterCodeExec.cpp
    source/core/tjs2/tjsInterCodeGen.cpp
    source/core/tjs2/tjsInterface.cpp
    source/core/tjs2/tjsLex.cpp
    source/core/tjs2/tjsMT19937ar-cok.cpp
    source/core/tjs2/tjsMath.cpp
    source/core/tjs2/tjsMessage.cpp
    source/core/tjs2/tjsNamespace.cpp
    source/core/tjs2/tjsNative.cpp
    source/core/tjs2/tjsObject.cpp
    source/core/tjs2/tjsObjectExtendable.cpp
    source/core/tjs2/tjsOctPack.cpp
    source/core/tjs2/tjsRandomGenerator.cpp
    source/core/tjs2/tjsRegExp.cpp
    source/core/tjs2/tjsScriptBlock.cpp
    source/core/tjs2/tjsScriptCache.cpp
    source/core/tjs2/tjsSnprintf.cpp
    source/core/tjs2/tjsString.cpp
    source/core/tjs2/tjsUtils.cpp
    source/core/tjs2/tjsVariant.cpp
    source/core/tjs2/tjsVariantString.cpp
    source/core/tjs2/tjsdate.tab.cpp
    source/core/tjs2/tjspp.tab.cpp
    source/core/utils/ClipboardIntf.cpp
    source/core/utils/DebugIntf.cpp
    source/core/utils/KAGParser.cpp
    source/core/utils/MiscUtility.cpp
    source/core/utils/PadIntf.cpp
    source/core/utils/Random.cpp
    source/core/utils/TVPTimer.cpp
    source/core/utils/ThreadIntf.cpp
    source/core/utils/TickCount.cpp
    source/core/utils/TimerIntf.cpp
    source/core/utils/TimerThread.cpp
    source/core/utils/VelocityTracker.cpp
    source/core/utils/cp932_uni.cpp
    source/core/utils/encoding/gbk2unicode.c
    source/core/utils/encoding/jis2unicode.c
    source/core/utils/md5.c
    source/core/utils/uni_cp932.cpp
    source/core/utils/win32/ClipboardImpl.cpp
    source/core/visual/BitmapIntf.cpp
    source/core/visual/BitmapLayerTreeOwner.cpp
    source/core/visual/CharacterData.cpp
    source/core/visual/ComplexRect.cpp
    source/core/visual/FontSystem.cpp
    source/core/visual/FreeType.cpp
    source/core/visual/FreeTypeFontRasterizer.cpp
    source/core/visual/GraphicsLoadThread.cpp
    source/core/visual/GraphicsLoaderIntf.cpp
    source/core/visual/ImageFunction.cpp
    source/core/visual/LayerBitmapIntf.cpp
    source/core/visual/LayerIntf.cpp
    source/core/visual/LayerManager.cpp
    source/core/visual/LayerTreeOwnerImpl.cpp
    source/core/visual/LoadJPEG.cpp
    source/core/visual/LoadPNG.cpp
    source/core/visual/LoadTLG.cpp
    source/core/visual/LoadWEBP.cpp
    source/core/visual/MenuItemIntf.cpp
    source/core/visual/PrerenderedFont.cpp
    source/core/visual/RectItf.cpp
    source/core/visual/RenderManager.cpp
    source/core/visual/SaveTLG5.cpp
    source/core/visual/SaveTLG6.cpp
    source/core/visual/TransIntf.cpp
    source/core/visual/VideoOvlIntf.cpp
    source/core/visual/WindowIntf.cpp
    source/core/visual/gl/ResampleImage.cpp
    source/core/visual/gl/WeightFunctor.cpp
    source/core/visual/gl/blend_function.cpp
    source/core/visual/tvpgl.cpp
    source/core/visual/win32/BasicDrawDevice.cpp
    source/core/visual/win32/BitmapBitsAlloc.cpp
    source/core/visual/win32/BitmapInfomation.cpp
    source/core/visual/win32/DInputMgn.cpp
    source/core/visual/win32/DrawDevice.cpp
    source/core/visual/win32/GraphicsLoaderImpl.cpp
    source/core/visual/win32/LayerBitmapImpl.cpp
    source/core/visual/win32/LayerImpl.cpp
    source/core/visual/win32/MenuItemImpl.cpp
    source/core/visual/win32/PassThroughDrawDevice.cpp
    source/core/visual/win32/TVPScreen.cpp
    source/core/visual/win32/VideoOvlImpl.cpp
    source/core/visual/win32/WindowImpl.cpp
    source/plugins/InternalPlugins.cpp
    source/plugins/LayerExBase.cpp
    source/plugins/csvParser.cpp
    source/plugins/dirlist.cpp
    source/plugins/fftgraph.cpp
    source/plugins/getSample.cpp
    source/plugins/layerExPerspective.cpp
    source/plugins/ncbind/ncbind.cpp
    source/plugins/saveStruct.cpp
    source/plugins/varfile.cpp
    source/plugins/win32dialog.cpp
    source/plugins/wutcwf.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FREETYPE2 REQUIRED IMPORTED_TARGET freetype2)
pkg_check_modules(LIBAVCODEC REQUIRED IMPORTED_TARGET libavcodec)
pkg_check_modules(LIBAVFILTER REQUIRED IMPORTED_TARGET libavfilter)
pkg_check_modules(LIBAVFORMAT REQUIRED IMPORTED_TARGET libavformat)
pkg_check_modules(LIBAVUTIL REQUIRED IMPORTED_TARGET libavutil)
pkg_check_modules(LIBPNG REQUIRED IMPORTED_TARGET libpng)
pkg_check_modules(LIBSWSCALE REQUIRED IMPORTED_TARGET libswscale)
pkg_check_modules(LIBTURBOJPEG REQUIRED IMPORTED_TARGET libturbojpeg)
pkg_check_modules(LIBWEBP REQUIRED IMPORTED_TARGET libwebp)
pkg_check_modules(OGG REQUIRED IMPORTED_TARGET ogg)
pkg_check_modules(ONIGURUMA REQUIRED IMPORTED_TARGET oniguruma)
pkg_check_modules(OPUS REQUIRED IMPORTED_TARGET opus)
pkg_check_modules(OPUSFILE REQUIRED IMPORTED_TARGET opusfile)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(VORBIS REQUIRED IMPORTED_TARGET vorbis)
pkg_check_modules(VORBISFILE REQUIRED IMPORTED_TARGET vorbisfile)
pkg_check_modules(ZLIB REQUIRED IMPORTED_TARGET zlib)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    PkgConfig::FREETYPE2
    PkgConfig::LIBAVCODEC
    PkgConfig::LIBAVFILTER
    PkgConfig::LIBAVFORMAT
    PkgConfig::LIBAVUTIL
    PkgConfig::LIBPNG
    PkgConfig::LIBSWSCALE
    PkgConfig::LIBTURBOJPEG
    PkgConfig::LIBWEBP
    PkgConfig::OGG
    PkgConfig::ONIGURUMA
    PkgConfig::OPUS
    PkgConfig::OPUSFILE
    PkgConfig::SDL2
    PkgConfig::VORBIS
    PkgConfig::VORBISFILE
    PkgConfig::ZLIB
)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})
target_compile_options(${PROJECT_NAME} PUBLIC $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>)
target_compile_options(${PROJECT_NAME} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>)
target_compile_options(${PROJECT_NAME} PUBLIC $<$<CONFIG:RELEASE>:${OPTIMIZATION_FLAGS}>)
target_compile_options(${PROJECT_NAME} PUBLIC $<$<CONFIG:RELWITHDEBINFO>:${OPTIMIZATION_FLAGS}>)

add_custom_target(${PROJECT_NAME}.nacp
    COMMAND ${DEVKITPRO}/tools/bin/nacptool
        --create
        ${SWITCH_APP_NAME}
        ${SWITCH_AUTHOR}
        ${SWITCH_VERSION}
        ${PROJECT_NAME}.nacp
)
add_custom_target(${PROJECT_NAME}.nro ALL
    COMMAND ${DEVKITPRO}/tools/bin/elf2nro
        ${PROJECT_NAME}
        ${PROJECT_NAME}.nro
        --icon=${CMAKE_SOURCE_DIR}/icon.jpg
        --nacp=${PROJECT_NAME}.nacp
    DEPENDS ${PROJECT_NAME}.nacp ${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/icon.jpg
)
