CXX? = g++
CFLAGS=`pkg-config --cflags --libs gtk+-3.0 gthread-2.0` -lxdo -lX11
#/usr/lib/x86_64-linux-gnu/
#/usr/lib/x86_64-linux-gnu/
OBJDIR=../build
OBJS=$(OBJDIR)/tvAsistant.o $(OBJDIR)/gui.o $(OBJDIR)/log.o $(OBJDIR)/command.o $(OBJDIR)/network.o

main: $(OBJS)
	$(CXX) -o $(OBJDIR)/tvAsistant $(OBJS) $(CFLAGS) && echo "*** Application is in build path: $(OBJDIR)"
	
# -c directive indicates the compile only compiles but not links.
$(OBJDIR)/tvAsistant.o: tvAsistant.cpp gui.h
	$(CXX) -c -o $(OBJDIR)/tvAsistant.o tvAsistant.cpp $(CFLAGS)
	
$(OBJDIR)/gui.o: gui.cpp gui.h
	$(CXX) -c -o $(OBJDIR)/gui.o gui.cpp $(CFLAGS)

$(OBJDIR)/log.o: log.cpp log.h
	$(CXX) -c -o $(OBJDIR)/log.o log.cpp $(CFLAGS)
	
$(OBJDIR)/command.o: command.cpp command.h log.h
	$(CXX) -c -o $(OBJDIR)/command.o command.cpp $(CFLAGS)
	
$(OBJDIR)/network.o: network.cpp network.h log.h
	$(CXX) -c -o $(OBJDIR)/network.o network.cpp $(CFLAGS)
	
clean:
	rm -f $(OBJDIR)/tvAsistant $(OBJS)



