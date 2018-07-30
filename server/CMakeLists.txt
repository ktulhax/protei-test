
find_package (Threads)

set(SERVER_HEADER
	server.h
	private/p_server.h
)

set(SERVER_SOURCE
	main.cpp
	server.cpp
	private/p_server_tcp.cpp
	private/p_server_udp.cpp
	private/p_server.cpp
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	private
)

add_executable(server
	${SERVER_HEADER}
	${SERVER_SOURCE}
)

target_link_libraries(server
	${CMAKE_THREAD_LIBS_INIT}
)
