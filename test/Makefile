top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include
LIBS=-L../src -lrudiments $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS)

.SUFFIXES: .lo

all: svr sslsvr lsnr inetclnt sslclnt unixclnt dmn err group cmdline paramstr host log oneshotclnt oneshotsvr passwd service shadow permissions protocol regexp rpc sem2 shm2 text datetime tzf dict dtd env file filesystem list lock1 lock2 mem random sem1 shm1 signal string strbuf handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient vbuf xmls xmld server.pem

clean:
	$(LIBTOOL) --mode=clean $(RM) ca.pem
	$(LIBTOOL) --mode=clean $(RM) server.pem
	$(LIBTOOL) --mode=clean $(RM) client.pem
	$(LIBTOOL) --mode=clean $(RM) *.lo
	$(LIBTOOL) --mode=clean $(RM) *.o
	$(LIBTOOL) --mode=clean $(RM) lsnr svr sslsvr inetclnt sslclnt unixclnt dmn err group cmdline paramstr host log oneshotclnt oneshotsvr passwd service shadow permissions protocol regexp rpc sem1 sem2 shm1 shm2 text datetime tzf dict dtd env file filesystem list lock1 lock2 mem random signal string strbuf handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient vbuf xmls xmld
	$(RM) test.log
	$(RMTREE) .libs
	$(RM) testfile
	$(MAKE) -C certificates clean

rebuild: clean all

.C.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -g -c $< -o $@

svr: svr.C svr.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ svr.lo -all-static $(LIBS)

sslsvr: sslsvr.C sslsvr.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslsvr.lo -all-static $(LIBS)

lsnr: lsnr.C lsnr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lsnr.lo -all-static $(LIBS)

inetclnt: inetclnt.C inetclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetclnt.lo -all-static $(LIBS)

sslclnt: sslclnt.C sslclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslclnt.lo -all-static $(LIBS)

unixclnt: unixclnt.C unixclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ unixclnt.lo -all-static $(LIBS)

dmn: dmn.C dmn.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ dmn.lo -all-static $(LIBS)

err: err.C err.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ err.lo -all-static $(LIBS)

cmdline: cmdline.C cmdline.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ cmdline.lo -all-static $(LIBS)

paramstr: paramstr.C paramstr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ paramstr.lo -all-static $(LIBS)

log: log.C log.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ log.lo -all-static $(LIBS)

env: env.C env.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ env.lo -all-static $(LIBS)

file: file.C file.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ file.lo -all-static $(LIBS)

filesystem: filesystem.C filesystem.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ filesystem.lo -all-static $(LIBS)

group: group.C group.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ group.lo -all-static $(LIBS)

host: host.C host.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ host.lo -all-static $(LIBS)

list: list.C list.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ list.lo -all-static $(LIBS)

lock1: lock1.C lock1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lock1.lo -all-static $(LIBS)

lock2: lock2.C lock2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lock2.lo -all-static $(LIBS)

mem: mem.C mem.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ mem.lo -all-static $(LIBS)

oneshotclnt: oneshotclnt.C oneshotclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ oneshotclnt.lo -all-static $(LIBS)

oneshotsvr: oneshotsvr.C oneshotsvr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ oneshotsvr.lo -all-static $(LIBS)

passwd: passwd.C passwd.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ passwd.lo -all-static $(LIBS)

service: service.C service.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ service.lo -all-static $(LIBS)

shadow: shadow.C shadow.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shadow.lo -all-static $(LIBS)

permissions: permissions.C permissions.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ permissions.lo -all-static $(LIBS)

protocol: protocol.C protocol.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ protocol.lo -all-static $(LIBS)

regexp: regexp.C regexp.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ regexp.lo -all-static $(LIBS)

rpc: rpc.C rpc.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ rpc.lo -all-static $(LIBS)

sem1: sem1.C sem1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sem1.lo -all-static $(LIBS)

sem2: sem2.C sem2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sem2.lo -all-static $(LIBS)

shm1: shm1.C shm1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shm1.lo -all-static $(LIBS)

shm2: shm2.C shm2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shm2.lo -all-static $(LIBS)

text: text.C text.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ text.lo -all-static $(LIBS)

datetime: datetime.C datetime.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ datetime.lo -all-static $(LIBS)

tzf: tzf.C tzf.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ tzf.lo -all-static $(LIBS)

dict: dict.C dict.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dict.lo -all-static $(LIBS)

dtd: dtd.C dtd.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dtd.lo -all-static $(LIBS)

random: random.C random.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ random.lo -all-static $(LIBS)

signal: signal.C signal.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ signal.lo -all-static $(LIBS)

string: string.C string.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ string.lo -all-static $(LIBS)

strbuf: strbuf.C strbuf.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ strbuf.lo -all-static $(LIBS)

handoff1: handoff1.C handoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoff1.lo -all-static $(LIBS)

handoff2: handoff2.C handoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoff2.lo -all-static $(LIBS)

handoffclient: handoffclient.C handoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoffclient.lo -all-static $(LIBS)

nonhandoff1: nonhandoff1.C nonhandoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoff1.lo -all-static $(LIBS)

nonhandoff2: nonhandoff2.C nonhandoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoff2.lo -all-static $(LIBS)

nonhandoffclient: nonhandoffclient.C nonhandoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoffclient.lo -all-static $(LIBS)

vbuf: vbuf.C vbuf.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ vbuf.lo -all-static $(LIBS)

xmls: xmls.C xmls.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmls.lo -all-static $(LIBS)

xmld: xmld.C xmld.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmld.lo -all-static $(LIBS)

server.pem:
	$(MAKE) -C certificates
	cp certificates/certificates/ca.pem .
	cp certificates/certificatechains/server.pem .
	cp certificates/certificatechains/client.pem .
