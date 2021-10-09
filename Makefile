default: spawn

CC = g++ -g -std=c++20
LINK = -lpthread -lrt #-lconcord

src = $(CC) \
			src/hdls.hpp \
			src/filler.cpp \
			src/hdls.cpp

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
