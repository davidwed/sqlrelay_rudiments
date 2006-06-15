top_builddir = ..

include ../config.mk

.PHONY: all clean rebuild

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include
LIBS=-L../src -lrudiments $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(SHMOPENLIB) $(CRYPTLIB) $(INETATONLIB) $(DLLIB)

#ALLSTATIC=-all-static

.SUFFIXES: .lo

all: svr sslsvr lsnr inetclnt sslclnt unixclnt dir dmn group cmdline paramstr host log oneshotclnt oneshotsvr passwd service shadow permissions protocol regexp rpc sem2 shm2 text datetime tzf dict dtd env file filesystem list lock1 lock2 mem random sem1 shm1 signal string strbuf handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient vbuf xattr xmls xmld timer sleep inetclntbench inetsvrbench server.pem

clean:
	$(LIBTOOL) --mode=clean $(RM) ca.pem
	$(LIBTOOL) --mode=clean $(RM) server.pem
	$(LIBTOOL) --mode=clean $(RM) client.pem
	$(LIBTOOL) --mode=clean $(RM) *.lo
	$(LIBTOOL) --mode=clean $(RM) *.o
	$(LIBTOOL) --mode=clean $(RM) *.exe
	$(LIBTOOL) --mode=clean $(RM) lsnr svr sslsvr inetclnt sslclnt unixclnt dir dmn group cmdline paramstr host log oneshotclnt oneshotsvr passwd service shadow permissions protocol regexp rpc sem1 sem2 shm1 shm2 text datetime tzf dict dtd env file filesystem list lock1 lock2 mem random signal string strbuf handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient vbuf xattr xmls xmld timer sleep inetclntbench inetsvrbench
	$(RM) test.log
	$(RMTREE) .libs
	$(RM) testfile
	$(MAKE) -C certificates clean

rebuild: clean all

.C.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -g -c $< -o $@

svr: svr.C svr.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ svr.lo $(ALLSTATIC) $(LIBS)

sslsvr: sslsvr.C sslsvr.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslsvr.lo $(ALLSTATIC) $(LIBS)

lsnr: lsnr.C lsnr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lsnr.lo $(ALLSTATIC) $(LIBS)

inetclnt: inetclnt.C inetclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetclnt.lo $(ALLSTATIC) $(LIBS)

sslclnt: sslclnt.C sslclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslclnt.lo $(ALLSTATIC) $(LIBS)

unixclnt: unixclnt.C unixclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ unixclnt.lo $(ALLSTATIC) $(LIBS)

dir: dir.C dir.lo 
	#$(LIBTOOL) --mode=link $(CXX) -o $@ dir.lo $(ALLSTATIC) $(LIBS)
	$(LIBTOOL) --mode=link $(CXX) -o $@ dir.lo $(LIBS) -lefence

dmn: dmn.C dmn.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ dmn.lo $(ALLSTATIC) $(LIBS)

cmdline: cmdline.C cmdline.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ cmdline.lo $(ALLSTATIC) $(LIBS)

paramstr: paramstr.C paramstr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ paramstr.lo $(ALLSTATIC) $(LIBS)

log: log.C log.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ log.lo $(ALLSTATIC) $(LIBS)

env: env.C env.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ env.lo $(ALLSTATIC) $(LIBS)

file: file.C file.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ file.lo $(ALLSTATIC) $(LIBS)

filesystem: filesystem.C filesystem.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ filesystem.lo $(ALLSTATIC) $(LIBS)

group: group.C group.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ group.lo $(ALLSTATIC) $(LIBS)

host: host.C host.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ host.lo $(ALLSTATIC) $(LIBS)

list: list.C list.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ list.lo $(ALLSTATIC) $(LIBS)

lock1: lock1.C lock1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lock1.lo $(ALLSTATIC) $(LIBS)

lock2: lock2.C lock2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lock2.lo $(ALLSTATIC) $(LIBS)

mem: mem.C mem.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ mem.lo $(ALLSTATIC) $(LIBS)

oneshotclnt: oneshotclnt.C oneshotclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ oneshotclnt.lo $(ALLSTATIC) $(LIBS)

oneshotsvr: oneshotsvr.C oneshotsvr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ oneshotsvr.lo $(ALLSTATIC) $(LIBS)

passwd: passwd.C passwd.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ passwd.lo $(ALLSTATIC) $(LIBS)

service: service.C service.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ service.lo $(ALLSTATIC) $(LIBS)

shadow: shadow.C shadow.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shadow.lo $(ALLSTATIC) $(LIBS)

permissions: permissions.C permissions.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ permissions.lo $(ALLSTATIC) $(LIBS)

protocol: protocol.C protocol.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ protocol.lo $(ALLSTATIC) $(LIBS)

regexp: regexp.C regexp.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ regexp.lo $(ALLSTATIC) $(LIBS)

rpc: rpc.C rpc.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ rpc.lo $(ALLSTATIC) $(LIBS)

sem1: sem1.C sem1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sem1.lo $(ALLSTATIC) $(LIBS)

sem2: sem2.C sem2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sem2.lo $(ALLSTATIC) $(LIBS)

shm1: shm1.C shm1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shm1.lo $(ALLSTATIC) $(LIBS)

shm2: shm2.C shm2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shm2.lo $(ALLSTATIC) $(LIBS)

text: text.C text.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ text.lo $(ALLSTATIC) $(LIBS)

datetime: datetime.C datetime.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ datetime.lo $(ALLSTATIC) $(LIBS)

tzf: tzf.C tzf.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ tzf.lo $(ALLSTATIC) $(LIBS)

dict: dict.C dict.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dict.lo $(ALLSTATIC) $(LIBS)

dtd: dtd.C dtd.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dtd.lo $(ALLSTATIC) $(LIBS)

random: random.C random.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ random.lo $(ALLSTATIC) $(LIBS)

signal: signal.C signal.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ signal.lo $(ALLSTATIC) $(LIBS)

string: string.C string.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ string.lo $(ALLSTATIC) $(LIBS)

strbuf: strbuf.C strbuf.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ strbuf.lo $(ALLSTATIC) $(LIBS)

handoff1: handoff1.C handoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoff1.lo $(ALLSTATIC) $(LIBS)

handoff2: handoff2.C handoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoff2.lo $(ALLSTATIC) $(LIBS)

handoffclient: handoffclient.C handoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoffclient.lo $(ALLSTATIC) $(LIBS)

nonhandoff1: nonhandoff1.C nonhandoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoff1.lo $(ALLSTATIC) $(LIBS)

nonhandoff2: nonhandoff2.C nonhandoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoff2.lo $(ALLSTATIC) $(LIBS)

nonhandoffclient: nonhandoffclient.C nonhandoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoffclient.lo $(ALLSTATIC) $(LIBS)

vbuf: vbuf.C vbuf.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ vbuf.lo $(ALLSTATIC) $(LIBS)

xattr: xattr.C xattr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xattr.lo $(ALLSTATIC) $(LIBS)

xmls: xmls.C xmls.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmls.lo $(ALLSTATIC) $(LIBS)

xmld: xmld.C xmld.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmld.lo $(ALLSTATIC) $(LIBS)

timer: timer.C timer.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ timer.lo $(ALLSTATIC) $(LIBS)

sleep: sleep.C sleep.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sleep.lo $(ALLSTATIC) $(LIBS)

inetclntbench: inetclntbench.C inetclntbench.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetclntbench.lo $(ALLSTATIC) $(LIBS)

inetsvrbench: inetsvrbench.C inetsvrbench.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetsvrbench.lo $(ALLSTATIC) $(LIBS)

server.pem:
	$(MAKE) -C certificates
	cp certificates/certificates/ca.pem .
	cp certificates/certificatechains/server.pem .
	cp certificates/certificatechains/client.pem .
