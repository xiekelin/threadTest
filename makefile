OBJS =test.o ThreadPool.o Worker.o Runnable.o SimpleSpinLock.o TestThreadPool.o
#OBJS =pay.o
EXE=thread

#DBPATH=../../../db
#COMPATH=../../../lib

#ACE_FRAME_PATH=../../../aceframe
#ACE_FRAME_INC=-I $(ACE_FRAME_PATH)/include -I $(ACE_FRAME_PATH)/busiframe/include
#ACE_FRAME_LIBS=-L $(ACE_FRAME_PATH)/lib  -Wl,-Bstatic -lbusiframe -lwpaybl -Wl,-Bdynamic

G_INC=/usr/include/pay
G_LIBS=/usr/libpay

#compile with RUN_WITH_TIME_CONSUME will show time consume
CPPFLAGS=-g -std=c++11 -lpthread -DRUN_WITH_TIME_CONSUME -I$(G_INC)/g3log -I$(G_INC)/wpaybl -I/home/suq/working/busiframe -I$(G_INC)/newdbdal/include -I$(G_INC)/newlibmemcached/include -I$(G_INC) -L$(G_LIBS) -DACE_NTRACE=1 -fPIC -lg3logger_shared -ljson_linux-gcc-4.1.2_libmt -lACE -lcurl -lprotobuf -llog4cpp -lcrypto -lssl -Wl,-Bstatic -I$(G_INC)/route -I$(G_INC)/log4cpp -lcpproute -Wl,-Bstatic -lbusiframe -llog4cplus -lwpaybl -L$(G_LIBS)/newlibmemcached -lmemcached -Wl,-Bdynamic -I$(G_INC)/newdbdal/include/oracle -lnewdal -lclntsh

#if you want to open ACE_TRACE,please compile with -DACE_NTRACE=0. default value is 1
$(EXE):$(OBJS)
	gcc  $^  -o $@ $(CPPFLAGS) -std=gnu++11 -D_GLIBCXX_USE_NANOSLEEP

clean:
	rm -rf *.o