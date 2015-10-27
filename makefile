TARGET = shobocal_cpp
CC = g++
OBJS = shoboi_main.o shoboi_anime_list.o

$(TARGET): $(OBJS)
	$(CC) -O2 -o shobocal_cpp -lboost_system-mt -lboost_filesystem-mt -lrt $(OBJS)

shoboi_anime_list.o: shoboi_anime_list.cpp
	$(CC) -c shoboi_anime_list.cpp

shoboi_main.o: shoboi_main.cpp
	$(CC) -c shoboi_main.cpp

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)

