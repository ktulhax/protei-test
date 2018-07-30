
set(CLIENT_HEADER
	client.h
)

set(CLIENT_SOURCE
	main.cpp
	client.cpp
)


add_executable(client
	${CLIENT_HEADER}
	${CLIENT_SOURCE}
)