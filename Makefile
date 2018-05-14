GCC:= g++
FLAGS = -std=c++11 -g -Wall
NDEBUG = -D NDEBUG
INCLUDE:= -I./unpv13e/lib
# LIBS = ./unpv13e/libunp.a -lresolv -lpthread
LIBS = -lresolv -lpthread -L./unpv13e/ -lunp
EXE_SUFFIX = .out

OUTPUT = client$(EXE_SUFFIX) server$(EXE_SUFFIX) test$(EXE_SUFFIX) echoclient$(EXE_SUFFIX) echoserver$(EXE_SUFFIX) main$(EXE_SUFFIX)
OUTPUT+= im_server$(EXE_SUFFIX) im_client$(EXE_SUFFIX) clitest$(EXE_SUFFIX)

.cpp.o :
	$(GCC) $(FLAGS) $(INCLUDE) -o $@ -c $< $(LIBS)
.hpp.o :
	$(GCC) -x c++ $(FLAGS) $(INCLUDE) -o $@ -c $< $(LIBS)

main: main.o
	$(GCC) $(FLAGS) $(INCLUDE) -o $@$(EXE_SUFFIX) $^ $(LIBS)

server : client server
	
client : client.o
	$(GCC) $(FLAGS) $(INCLUDE) -o $@$(EXE_SUFFIX) $^ $(LIBS)

server : server.o
	$(GCC) $(FLAGS) $(INCLUDE) -o $@$(EXE_SUFFIX) $^ $(LIBS)

## 测试文件编译 make test
test : test.o
	$(GCC) $(FLAGS) $(INCLUDE) -o $@$(EXE_SUFFIX) test.o $(LIBS)

## 回射服务器编写 make echo
echoserver : echoserver.o
	$(GCC) $(FLAGS) $(INCLUDE) -o $@$(EXE_SUFFIX) $^ $(LIBS)

echoclient : echoclient.o
	$(GCC) $(FLAGS) $(INCLUDE) -o $@$(EXE_SUFFIX) $^ $(LIBS)

clitest : cli_test.o
	$(GCC) $(FLAGS) $(INCLUDE) -o $@$(EXE_SUFFIX) $^ $(LIBS)

echo : echoclient echoserver

## 文件清理 make clean
clean :
	rm *.o $(OUTPUT);rm -R main.dSym;rm im_select/*.o;


## IM_SELECT 文件编译
im: imserver imclient

imserver: im_select/im_server.o
	$(GCC) $(FLAGS) $(INCLUDE) -o $@$(EXE_SUFFIX) $^ $(LIBS)

imclient: im_select/im_client.o
