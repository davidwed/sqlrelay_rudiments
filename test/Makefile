top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include

.SUFFIXES: .lo

all: svr sslsvr lsnr inetclnt sslclnt unixclnt dmn err group cmdline paramstr host log oneshotclnt oneshotsvr passwd service shadow permissions protocol regexp rpc sem2 shm2 text datetime tzf dict dtd env file filesystem list lock1 lock2 mem random sem1 shm1 signal string strbuf handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient vbuf xmls xmld

clean:
	$(LIBTOOL) --mode=clean $(RM) *.lo
	$(LIBTOOL) --mode=clean $(RM) *.o
	$(LIBTOOL) --mode=clean $(RM) lsnr svr sslsvr inetclnt sslclnt unixclnt dmn err group cmdline paramstr host log oneshotclnt oneshotsvr passwd service shadow permissions protocol regexp rpc sem1 sem2 shm1 shm2 text datetime tzf dict dtd env file filesystem list lock1 lock2 mem random signal string strbuf handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient vbuf xmls xmld
	$(RM) test.log
	$(RMTREE) .libs
	$(RM) testfile

rebuild: clean all

.C.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -g -c $< -o $@

svr: svr.C svr.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ svr.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

sslsvr: sslsvr.C sslsvr.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslsvr.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

lsnr: lsnr.C lsnr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lsnr.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

inetclnt: inetclnt.C inetclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetclnt.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

sslclnt: sslclnt.C sslclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslclnt.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

unixclnt: unixclnt.C unixclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ unixclnt.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

dmn: dmn.C dmn.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ dmn.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

err: err.C err.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ err.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

cmdline: cmdline.C cmdline.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ cmdline.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

paramstr: paramstr.C paramstr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ paramstr.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

log: log.C log.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ log.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

env: env.C env.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ env.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

file: file.C file.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ file.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

filesystem: filesystem.C filesystem.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ filesystem.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

group: group.C group.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ group.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

host: host.C host.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ host.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

list: list.C list.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ list.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

lock1: lock1.C lock1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lock1.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

lock2: lock2.C lock2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lock2.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

mem: mem.C mem.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ mem.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

oneshotclnt: oneshotclnt.C oneshotclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ oneshotclnt.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

oneshotsvr: oneshotsvr.C oneshotsvr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ oneshotsvr.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

passwd: passwd.C passwd.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ passwd.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

service: service.C service.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ service.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

shadow: shadow.C shadow.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shadow.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

permissions: permissions.C permissions.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ permissions.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

protocol: protocol.C protocol.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ protocol.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

regexp: regexp.C regexp.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ regexp.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

rpc: rpc.C rpc.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ rpc.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

sem1: sem1.C sem1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sem1.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

sem2: sem2.C sem2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sem2.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

shm1: shm1.C shm1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shm1.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

shm2: shm2.C shm2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shm2.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

text: text.C text.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ text.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

datetime: datetime.C datetime.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ datetime.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

tzf: tzf.C tzf.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ tzf.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

dict: dict.C dict.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dict.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

dtd: dtd.C dtd.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dtd.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

random: random.C random.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ random.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

signal: signal.C signal.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ signal.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

string: string.C string.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ string.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

strbuf: strbuf.C strbuf.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ strbuf.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

handoff1: handoff1.C handoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoff1.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

handoff2: handoff2.C handoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoff2.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

handoffclient: handoffclient.C handoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoffclient.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

nonhandoff1: nonhandoff1.C nonhandoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoff1.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

nonhandoff2: nonhandoff2.C nonhandoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoff2.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

nonhandoffclient: nonhandoffclient.C nonhandoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoffclient.lo -all-static -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB) $(SSLLIBS)

vbuf: vbuf.C vbuf.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ vbuf.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

xmls: xmls.C xmls.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmls.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)

xmld: xmld.C xmld.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmld.lo -all-static -L../src -lrudiments $(PTHREADLIB) $(SSLLIBS)
