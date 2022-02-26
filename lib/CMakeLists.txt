target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "C:/dev/utils/ffmpeg/include")
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
	C:/dev/utils/ffmpeg/lib/avcodec.lib
	C:/dev/utils/ffmpeg/lib/avdevice.lib
	C:/dev/utils/ffmpeg/lib/avfilter.lib
	C:/dev/utils/ffmpeg/lib/avutil.lib
	C:/dev/utils/ffmpeg/lib/avformat.lib
	C:/dev/utils/ffmpeg/lib/swscale.lib
	C:/dev/utils/ffmpeg/lib/swresample.lib
)