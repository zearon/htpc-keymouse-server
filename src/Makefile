CXX? = g++
CFLAGS=`pkg-config --cflags --libs gtk+-3.0 gthread-2.0` -lxdo -lX11
#/usr/lib/x86_64-linux-gnu/
#/usr/lib/x86_64-linux-gnu/
OBJS=tvAsistant.o gui.o log.o command.o network.o

main: $(OBJS)
	$(CXX) -o tvAsistant $(OBJS) $(CFLAGS)
	
# -c directive indicates the compile only compiles but not links.
tvAsistant.o: tvAsistant.cpp gui.h
	$(CXX) -c tvAsistant.cpp $(CFLAGS)
	
gui.o: gui.cpp gui.h
	$(CXX) -c gui.cpp $(CFLAGS)

log.o: log.cpp log.h
	$(CXX) -c log.cpp $(CFLAGS)
	
command.o: command.cpp command.h log.h
	$(CXX) -c command.cpp $(CFLAGS)
	
network.o: network.cpp network.h log.h
	$(CXX) -c network.cpp $(CFLAGS)
	
clean:
	rm -f tvAsistant $(OBJS)



