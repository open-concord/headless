default: spawn

CC = g++ -g -std=c++20
LINK = -lpthread -lrt -Wall #-lconcord

src = $(CC) \
			inc/hdls.hpp \
			src/filler.cpp \
			src/hdls.cpp \
			src/api/linux.cpp # remove in production
			# linux || windows || posix depending on host

linux:
	$(src) \
	src/linux.cpp

windows:
	$(src) \
	src/windows.cpp

posix:
	$(src) \
	src/posix.cpp

kill:
	$(src) \
	test/tkill.cpp \
	tk.out \
	$(LINK)

spawn:
	$(src) \
	test/tspawn.cpp \
	-o ts.out \
	$(LINK)
